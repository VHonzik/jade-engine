#include "EngineTime.h"

#include <algorithm>
#include <cassert>

using namespace JadeEngine::details;

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

    for (auto iter = std::begin(_timeBoundTasks); iter != std::end(_timeBoundTasks);)
    {
      auto& task = *iter;
      assert(task.listener != nullptr);

      auto startTime = std::chrono::steady_clock::now();
      auto duration = std::chrono::milliseconds::zero().count();
      auto done = false;

      while (duration < task.maxDurationPerFrame && !done)
      {
        done = task.listener->DoTimeBoundTask();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
      }

      if (done)
      {
        iter = _timeBoundTasks.erase(iter);
      }
      else
      {
        ++iter;
      }
    }
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

  void Time::RegisterTimeBoundTask(ITimeBoundTaskListener* task, const float maxDurationPerFrame)
  {
    assert(std::find_if(std::cbegin(_timeBoundTasks), std::cend(_timeBoundTasks), [&](const TimeBoundTask& timeBoundTask) { return timeBoundTask.listener == task; }) == std::cend(_timeBoundTasks));

    _timeBoundTasks.emplace_back(TimeBoundTask{
      //ITimeBoundTaskListener* listener;
      task,
      //float maxDurationPerFrame;
      maxDurationPerFrame * 1000.0f
    });
  }
}
