#pragma once

#include <functional>
#include <SDL_render.h>

namespace JadeEngine
{
  struct Renderable
  {
    void* data;
    int z;
    std::function<void(void*, SDL_Renderer*)> render;

    static bool Compare(const Renderable& a, const Renderable& b)
    {
      return a.z < b.z;
    }
  };

  template<typename T>
  Renderable make_renderable(T* object);
}