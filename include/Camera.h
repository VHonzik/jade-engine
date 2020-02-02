#pragma once

#include "EngineDataTypes.h"
#include "Transform.h"
#include "Vector2D.h"

#include <SDL_rect.h>

namespace JadeEngine
{
  class Sprite;

  class UICamera
  {
  public:
    UICamera();

    void SetResolution(int32_t width, int32_t height);
    void SetDisplayMode(const Rectangle& scaledRect, const Rectangle& windowRect);

    int32_t WindowToRenderX(const int32_t x) const;
    int32_t WindowToRenderY(const int32_t y) const;

    bool IsMouseInside(const Sprite* sprite, bool preciseTest);

  private:
    int32_t _resolutionWidth;
    int32_t _resolutionHeight;

    int32_t _scaledResolutionWidth;
    int32_t _scaledResolutionHeight;

    int32_t _windowResolutionWidth;
    int32_t _windowResolutionHeight;
  };

  class WorldCamera
  {
  public:
    WorldCamera();

    Box_i32 WorldToScreen(const std::shared_ptr<Transform>& transform);
    Vector2D_i32 WorldToScreen(const Vector2D_i32& vector);
    Rectangle WorldToScreen(const Rectangle& rect);
    SDL_Point WorldToScreen(const SDL_Point& point);

    Rectangle ScreenToWorld(const Rectangle& rect);
    SDL_Point ScreenToWorld(const SDL_Point& point);

    void SetPosition(int x, int y);
    int GetX() const { return _x; }
    int GetY() const { return _y; }

    void UpdateScreenBorderPan();

  private:
    int _x;
    int _y;
  };

  extern WorldCamera GWorldCamera;
  extern UICamera GUICamera;
}
