#include "Camera.h"

#include "Input.h"
#include "Game.h"
#include "Sprite.h"
#include "EngineTime.h"
#include "Utils.h"

namespace JadeEngine
{
  UICamera GUICamera;
  WorldCamera GWorldCamera;

  UICamera::UICamera()
    : _resolutionWidth(0)
    , _resolutionHeight(0)
  {
  }

  void UICamera::SetResolution(int32_t width, int32_t height)
  {
    _resolutionWidth = _scaledResolutionWidth = _windowResolutionWidth =  width;
    _resolutionHeight = _scaledResolutionHeight = _windowResolutionHeight = height;
  }

  void UICamera::SetDisplayMode(const Rectangle& scaledRect, const Rectangle& windowRect)
  {
    _scaledResolutionWidth = scaledRect.w;
    _scaledResolutionHeight = scaledRect.h;
    _windowResolutionWidth = windowRect.w;
    _windowResolutionHeight = windowRect.h;
  }

  int32_t UICamera::WindowToRenderX(const int32_t x) const
  {
    const auto diff = ((_windowResolutionWidth - _scaledResolutionWidth) * 0.5f);
    const auto minX = diff;
    const auto maxX = _windowResolutionWidth - diff;
    const auto clamped = Clamp(static_cast<float>(x), minX, maxX);
    const auto scalingFactor = static_cast<float>(_resolutionWidth) / _scaledResolutionWidth;
    return static_cast<int32_t>((clamped-diff) * scalingFactor);
  }

  int32_t UICamera::WindowToRenderY(const int32_t y) const
  {
    const auto diff = ((_windowResolutionHeight - _scaledResolutionHeight) * 0.5f);
    const auto minY = diff;
    const auto maxY = _windowResolutionWidth - diff;
    const auto clamped = Clamp(static_cast<float>(y), minY, maxY);
    const auto scalingFactor = static_cast<float>(_resolutionHeight) / _scaledResolutionHeight;
    return static_cast<int32_t>((clamped - diff) * scalingFactor);
  }

  bool UICamera::IsMouseInside(const Sprite* sprite, bool preciseTest)
  {
    if (preciseTest && !sprite->HasHitTest())
    {
      return true;
    }

    auto mouseX = GInput.GetMouseX();
    auto mouseY = GInput.GetMouseY();

    const auto transform = sprite->GetTransform();

    bool inside = false;

    if (preciseTest)
    {
      inside = IsInsideRect(mouseX, mouseY, transform);

      if (inside)
      {
        inside = sprite->HitTest(mouseX - transform.x, mouseY - transform.y);
      }
    }
    else
    {
      SDL_Rect boundingBox = sprite->GetBoundingBox();
      SDL_Rect testingBox = { transform.x + boundingBox.x,
        transform.y + boundingBox.y, boundingBox.w, boundingBox.h };
      inside = IsInsideRect(mouseX, mouseY, testingBox);
    }

    return inside;
  }

  WorldCamera::WorldCamera()
    : _x(0)
    , _y(0)
  {
  }

  Rectangle WorldCamera::WorldToScreen(const Rectangle& rect)
  {
    return { rect.x - _x, rect.y - _y, rect.w, rect.h };
  }

  SDL_Point WorldCamera::WorldToScreen(const SDL_Point& point)
  {
    return { point.x - _x, point.y - _y };
  }

  Rectangle WorldCamera::ScreenToWorld(const Rectangle& rect)
  {
    return { rect.x + _x, rect.y + _y, rect.w, rect.h };
  }

  SDL_Point WorldCamera::ScreenToWorld(const SDL_Point& point)
  {
    return { point.x + _x, point.y + _y };
  }

  void WorldCamera::SetPosition(int x, int y)
  {
    _x = x;
    _y = y;
  }

  void WorldCamera::UpdateScreenBorderPan()
  {
    const auto mouseX = GInput.GetMouseX();
    if (mouseX < 50)
    {
      _x -= static_cast<int32_t>(GTime.deltaTime * GGame.GetHalfWidth());
    }
    else if (mouseX > GGame.GetWidth() - 50)
    {
      _x += static_cast<int32_t>(GTime.deltaTime * GGame.GetHalfWidth());
    }

    const auto mouseY = GInput.GetMouseY();
    if (mouseY < 50)
    {
      _y -= static_cast<int32_t>(GTime.deltaTime * GGame.GetHalfHeight());
    }
    else if (mouseY > GGame.GetHeight() - 50)
    {
      _y += static_cast<int32_t>(GTime.deltaTime * GGame.GetHalfHeight());
    }
  }
}