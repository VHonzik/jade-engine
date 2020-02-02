#pragma once

#include "EngineDataTypes.h"

#include <cstdint>
#include <functional>
#include <SDL.h>

namespace JadeEngine
{
  struct DisplayModeInfo
  {
    int32_t width;
    int32_t height;

    int32_t displaysSupported;

    bool native;

    std::string name;

    Rectangle scaledRect;

    Rectangle windowRect;

    int32_t distanceFromNative;

    bool operator==(const DisplayModeInfo& other) const
    {
      return width == other.width && height == other.height;
    }
  };

  using DisplayModeInfoKey = size_t;
}

namespace std
{
  template<>
  struct hash<JadeEngine::DisplayModeInfo>
  {
    size_t operator()(const JadeEngine::DisplayModeInfo& obj) const
    {
      return static_cast<size_t>(std::pow(10,6)) * obj.width + obj.height;
    }
  };
}




