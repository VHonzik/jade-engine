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

  void Time::SetLastSaveTime()
  {
    _lastSaveSystemTime = std::chrono::system_clock::now();
    _lastSaveSystemTimeSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(_lastSaveSystemTime.time_since_epoch()).count();
  }

  int64_t Time::GetLastSaveTime()
  {
    return _lastSaveSystemTimeSinceEpoch;
  }

  float Time::TimeElapsedSinceSave(const int64_t saveTime) const
  {
    const auto now = std::chrono::system_clock::now();
    const auto saveSinceEpoch = std::chrono::milliseconds{ saveTime };
    const auto saveTimePoint = std::chrono::time_point<std::chrono::system_clock>{} + saveSinceEpoch;
    const auto elapsedMiliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - saveTimePoint).count();
    return elapsedMiliseconds / 1000.0f;
  }
}
