#pragma once

#include <chrono>
#include <vector>

namespace JadeEngine
{
  class ITimeBoundTaskListener
  {
  public:
    virtual bool DoTimeBoundTask() = 0;
  };

  namespace details
  {
    struct TimeBoundTask
    {
      ITimeBoundTaskListener* listener;
      float maxDurationPerFrame;
    };
  }

  class Time
  {
  public:
    void Start();
    void Tick();

    void SetLastSaveTime();
    int64_t GetLastSaveTime();
    float TimeElapsedSinceSave(const int64_t saveTime) const;

    void RegisterTimeBoundTask(ITimeBoundTaskListener* task, const float maxDurationPerFrame);

    float deltaTime;
  private:
    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _previousTick;
    std::chrono::steady_clock::time_point _now;

    std::chrono::system_clock::time_point _lastSaveSystemTime;
    int64_t _lastSaveSystemTimeSinceEpoch;

    std::vector<details::TimeBoundTask> _timeBoundTasks;
  };

  extern Time GTime;
}