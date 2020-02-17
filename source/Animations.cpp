#include "Animations.h"

#include "EngineTime.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>

namespace JadeEngine
{

  PulseAnimation::PulseAnimation()
    : _state(kPulseAnimationState_Uinitialized)
    , _waitingForStop(false)
    , _currentSpeed(0.0f)
  {
  }

  void PulseAnimation::SetState(const PulseAnimationState state)
  {
    _state = state;
  }

  void PulseAnimation::SetStateEntries(const PulseAnimationState state, const bool cycleStart)
  {
    for (auto& entry : _entries)
    {
      if (cycleStart && entry.state == kPulseAnimationState_WaitingForSynchronization)
      {
        entry.state = state;
      }
      else if (entry.state != kPulseAnimationState_WaitingForSynchronization)
      {
        entry.state = state;
      }

      if (state == kPulseAnimationState_Stopped)
      {
        entry.gameObject->transform->SetSize(entry.startingSize.Convert<int32_t>());
      }
    }
  }

  void PulseAnimation::Initialize(const PulseAnimationParams& params)
  {
    assert(params.relativeFinalSize != 1.0f);

    _params = params;
    SetState(kPulseAnimationState_Stopped);
    SetStateEntries(kPulseAnimationState_Stopped, false);

    const bool startContracting = _params.relativeFinalSize < 1.0f;
    _initialState = startContracting ? kPulseAnimationState_Contracting : kPulseAnimationState_Expanding;
    _minRelativeSize = _initialState == kPulseAnimationState_Contracting ? _params.relativeFinalSize : 1.0f;
    _maxRelativeSize = _initialState == kPulseAnimationState_Contracting ? 1.0f : _params.relativeFinalSize;

    if (_params.startImmidiatelly)
    {
      Start();
    }
  }

  void PulseAnimation::Start()
  {
    assert(_state != kPulseAnimationState_Uinitialized);

    if (_state == kPulseAnimationState_Stopped)
    {
      SetState(_initialState);
      SetStateEntries(_initialState, true);
      _currentSpeed = _initialState == kPulseAnimationState_Contracting ? _params.contractStartingSpeed : _params.expandStartingSpeed;
      _currentRelativeSize = 1.0f;
    }
    else
    {
      assert(_state == kPulseAnimationState_Paused);
      SetState(_beforePauseState);
      SetStateEntries(_beforePauseState, false);
    }
  }

  void PulseAnimation::Pause()
  {
    assert(_state != kPulseAnimationState_Uinitialized);
    _beforePauseState = _state;
    SetState(kPulseAnimationState_Paused);
    SetStateEntries(kPulseAnimationState_Paused, false);
  }

  void PulseAnimation::Stop()
  {
    assert(_state != kPulseAnimationState_Uinitialized);
    _waitingForStop = true;
  }

  void PulseAnimation::StopImmidiately()
  {
    assert(_state != kPulseAnimationState_Uinitialized);
    SetState(kPulseAnimationState_Stopped);
    SetStateEntries(kPulseAnimationState_Stopped, false);
  }

  void PulseAnimation::Update()
  {
    if (_state == kPulseAnimationState_Uinitialized) return;
    if (_entries.size() == 0) return;

    if (_state == kPulseAnimationState_Expanding)
    {
      const auto moveResult = MoveTowardsDone(_currentRelativeSize, _maxRelativeSize, _currentSpeed * GTime.deltaTime);
      _currentRelativeSize = moveResult.first;
      if (_params.expandAcceleration > 0.0f)
      {
        _currentSpeed = std::min(_currentSpeed + _params.expandAcceleration * GTime.deltaTime, _params.expandMinMaxSpeed);
      }
      else if (_params.expandAcceleration < 0.0f)
      {
        _currentSpeed = std::max(_currentSpeed + _params.expandAcceleration * GTime.deltaTime, _params.expandMinMaxSpeed);
      }

      if (moveResult.second)
      {
        const bool newCycle = _initialState == kPulseAnimationState_Contracting;

        if (_waitingForStop && newCycle)
        {
          SetState(kPulseAnimationState_Stopped);
          SetStateEntries(kPulseAnimationState_Stopped, false);
        }
        else
        {
          _currentSpeed = _params.contractStartingSpeed;
          SetState(kPulseAnimationState_Contracting);
          SetStateEntries(kPulseAnimationState_Contracting, newCycle);
        }
      }
    }
    else if (_state == kPulseAnimationState_Contracting)
    {
      const auto moveResult = MoveTowardsDone(_currentRelativeSize, _minRelativeSize, _currentSpeed * GTime.deltaTime);
      _currentRelativeSize = moveResult.first;

      if (_params.contractAcceleration > 0.0f)
      {
        _currentSpeed = std::min(_currentSpeed + _params.contractAcceleration * GTime.deltaTime, _params.contractMinMaxSpeed);
      }
      else if (_params.contractAcceleration < 0.0f)
      {
        _currentSpeed = std::max(_currentSpeed + _params.contractAcceleration * GTime.deltaTime, _params.contractMinMaxSpeed);
      }

      if (moveResult.second)
      {
        const bool newCycle = _initialState == kPulseAnimationState_Expanding;

        if (_waitingForStop && newCycle)
        {
          SetState(kPulseAnimationState_Stopped);
          SetStateEntries(kPulseAnimationState_Stopped, false);
        }
        else
        {
          _currentSpeed = _params.expandStartingSpeed;
          SetState(kPulseAnimationState_Expanding);
          SetStateEntries(kPulseAnimationState_Expanding, newCycle);
        }
      }
    }

    for (auto& entry : _entries)
    {
      if (entry.state == kPulseAnimationState_Contracting || entry.state == kPulseAnimationState_Expanding)
      {
        const auto desiredSize = Interpolate(entry.startingSize, entry.finalSize, (_currentRelativeSize - _minRelativeSize) / (_maxRelativeSize - _minRelativeSize));
        entry.gameObject->transform->SetSize(desiredSize.Convert<int32_t>());
      }
    }
  }

  void PulseAnimation::Register(IGameObject* gameObject)
  {
    const auto found = std::find_if(std::cbegin(_entries), std::cend(_entries), [&](const PulseAnimationEntry& entry)
    {
      return entry.gameObject == gameObject;
    });

    if (found != std::cend(_entries)) return;

    auto state = kPulseAnimationState_WaitingForSynchronization;
    if (_state == kPulseAnimationState_Uinitialized)
    {
      state = kPulseAnimationState_Uinitialized;
    }

    const auto startingSize = gameObject->transform->GetSize().Convert<float>();
    const auto finalSize = startingSize * _params.relativeFinalSize;

    _entries.push_back({
      //PulseAnimationState state;
      state,
      //IGameObject* gameObject;
      gameObject,
      //Vector2D_f32 startingSize;
      startingSize,
      //Vector2D_f32 finalSize;
      finalSize
      });
  }

  void PulseAnimation::Unregister(IGameObject* gameObject)
  {
    _entries.erase(std::remove_if(std::begin(_entries), std::end(_entries), [&](const PulseAnimationEntry& entry)
    {
      if (entry.gameObject == gameObject)
      {
        entry.gameObject->transform->SetSize(entry.startingSize.Convert<int32_t>());
        return true;
      }
      return false;
    }));
  }


}