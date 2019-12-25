#pragma once

#include <SDL.h>

namespace JadeEngine
{
  class Sprite;

  class UICamera
  {
  public:
    UICamera();

    void SetResolution(int32_t width, int32_t height);
    void SetDisplayMode(const SDL_Rect& scaledRect, const SDL_Rect& windowRect);

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

    SDL_Rect WorldToScreen(const SDL_Rect& rect);
    SDL_Point WorldToScreen(const SDL_Point& point);

    SDL_Rect ScreenToWorld(const SDL_Rect& rect);
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
