#include "Animations.h"

#include "EngineTime.h"
#include "Utils.h"

#include <algorithm>
#include <cassert>

namespace JadeEngine
{

  PulseAnimation::PulseAnimation()
    : _state(kPulseAnimationState_uinitialized)
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
      if (cycleStart && entry.state == kPulseAnimationState_waitingForSynchronization)
      {
        entry.state = state;
      }
      else if (entry.state != kPulseAnimationState_waitingForSynchronization)
      {
        entry.state = state;
      }

      if (state == kPulseAnimationState_stopped)
      {
        entry.gameObject->transform->SetSize(entry.startingSize.Convert<int32_t>());
      }
    }
  }

  void PulseAnimation::Initialize(const PulseAnimationParams& params)
  {
    assert(params.relativeFinalSize != 1.0f);

    _params = params;
    SetState(kPulseAnimationState_stopped);
    SetStateEntries(kPulseAnimationState_stopped, false);

    const bool startContracting = _params.relativeFinalSize < 1.0f;
    _initialState = startContracting ? kPulseAnimationState_contracting : kPulseAnimationState_expanding;
    _minRelativeSize = _initialState == kPulseAnimationState_contracting ? _params.relativeFinalSize : 1.0f;
    _maxRelativeSize = _initialState == kPulseAnimationState_contracting ? 1.0f : _params.relativeFinalSize;

    if (_params.startImmidiatelly)
    {
      Start();
    }
  }

  void PulseAnimation::Start()
  {
    assert(_state != kPulseAnimationState_uinitialized);

    if (_state == kPulseAnimationState_stopped)
    {
      SetState(_initialState);
      SetStateEntries(_initialState, true);
      _currentSpeed = _initialState == kPulseAnimationState_contracting ? _params.contractStartingSpeed : _params.expandStartingSpeed;
      _currentRelativeSize = 1.0f;
    }
    else
    {
      assert(_state == kPulseAnimationState_paused);
      SetState(_beforePauseState);
      SetStateEntries(_beforePauseState, false);
    }
  }

  void PulseAnimation::Pause()
  {
    assert(_state != kPulseAnimationState_uinitialized);
    _beforePauseState = _state;
    SetState(kPulseAnimationState_paused);
    SetStateEntries(kPulseAnimationState_paused, false);
  }

  void PulseAnimation::Stop()
  {
    assert(_state != kPulseAnimationState_uinitialized);
    _waitingForStop = true;
  }

  void PulseAnimation::StopImmidiately()
  {
    assert(_state != kPulseAnimationState_uinitialized);
    SetState(kPulseAnimationState_stopped);
    SetStateEntries(kPulseAnimationState_stopped, false);
  }

  void PulseAnimation::Update()
  {
    if (_state == kPulseAnimationState_uinitialized) return;
    if (_entries.size() == 0) return;

    if (_state == kPulseAnimationState_expanding)
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
        const bool newCycle = _initialState == kPulseAnimationState_contracting;

        if (_waitingForStop && newCycle)
        {
          SetState(kPulseAnimationState_stopped);
          SetStateEntries(kPulseAnimationState_stopped, false);
        }
        else
        {
          _currentSpeed = _params.contractStartingSpeed;
          SetState(kPulseAnimationState_contracting);
          SetStateEntries(kPulseAnimationState_contracting, newCycle);
        }
      }
    }
    else if (_state == kPulseAnimationState_contracting)
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
        const bool newCycle = _initialState == kPulseAnimationState_expanding;

        if (_waitingForStop && newCycle)
        {
          SetState(kPulseAnimationState_stopped);
          SetStateEntries(kPulseAnimationState_stopped, false);
        }
        else
        {
          _currentSpeed = _params.expandStartingSpeed;
          SetState(kPulseAnimationState_expanding);
          SetStateEntries(kPulseAnimationState_expanding, newCycle);
        }
      }
    }

    for (auto& entry : _entries)
    {
      if (entry.state == kPulseAnimationState_contracting || entry.state == kPulseAnimationState_expanding)
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

    auto state = kPulseAnimationState_waitingForSynchronization;
    if (_state == kPulseAnimationState_uinitialized)
    {
      state = kPulseAnimationState_uinitialized;
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