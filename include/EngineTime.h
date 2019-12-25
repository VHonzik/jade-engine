#pragma once

#include <chrono>

namespace JadeEngine
{
  class Time
  {
  private:
    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _previousTick;
    std::chrono::steady_clock::time_point _now;
  public:
    void Start();
    void Tick();

    float deltaTime;
  };

  extern Time GTime;
}