#pragma once

#include <chrono>

namespace JadeEngine
{
  class Time
  {
  public:
    void Start();
    void Tick();

    void SetLastSaveTime();
    int64_t GetLastSaveTime();
    float TimeElapsedSinceSave(const int64_t saveTime) const;

    float deltaTime;
  private:
    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _previousTick;
    std::chrono::steady_clock::time_point _now;

    std::chrono::system_clock::time_point _lastSaveSystemTime;
    int64_t _lastSaveSystemTimeSinceEpoch;
  };

  extern Time GTime;
}