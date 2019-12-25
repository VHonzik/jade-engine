#include "EngineTime.h"

namespace JadeEngine
{
  Time GTime;

  void Time::Start()
  {
    _start = std::chrono::steady_clock::now();
    _previousTick = std::chrono::steady_clock::now();
    _now = std::chrono::steady_clock::now();
    deltaTime = 0;
  }

  void Time::Tick()
  {
    _now = std::chrono::steady_clock::now();
    deltaTime = std::chrono::duration<float>(_now - _previousTick).count();
    _previousTick = _now;
  }
}
