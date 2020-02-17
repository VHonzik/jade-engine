#pragma once

#include "IGameObject.h"
#include "Vector2D.h"

#include <cstdint>
#include <vector>
#include <unordered_map>

namespace JadeEngine
{
  struct PulseAnimationParams
  {
    float expandStartingSpeed;
    float expandMinMaxSpeed;
    float expandAcceleration;
    float contractStartingSpeed;
    float contractMinMaxSpeed;
    float contractAcceleration;
    float relativeFinalSize;
    bool  startImmidiatelly;
  };

  enum PulseAnimationState
  {
    kPulseAnimationState_Uinitialized,
    kPulseAnimationState_Expanding,
    kPulseAnimationState_Contracting,
    kPulseAnimationState_Stopped,
    kPulseAnimationState_Paused,
    kPulseAnimationState_WaitingForSynchronization,
  };

  struct PulseAnimationEntry
  {
    PulseAnimationState state;
    IGameObject* gameObject;
    Vector2D_f32 startingSize;
    Vector2D_f32 finalSize;
  };

  class PulseAnimation
  {
  public:
    PulseAnimation();
    void Initialize(const PulseAnimationParams& params);
    void Start();
    void Pause();
    void Stop();
    void StopImmidiately();
    void Update();
    void Register(IGameObject* gameObject);
    void Unregister(IGameObject* gameObject);
    PulseAnimationState GetState() const { return _state; };
  private:
    void SetState(const PulseAnimationState state);
    void SetStateEntries(const PulseAnimationState state, const bool cycleStart);

    float _currentSpeed;
    float _currentRelativeSize;
    float _minRelativeSize;
    float _maxRelativeSize;

    bool _waitingForStop;
    PulseAnimationState _initialState;
    PulseAnimationState _beforePauseState;
    PulseAnimationState _state;
    PulseAnimationParams _params;
    std::vector<PulseAnimationEntry> _entries;
  };
}