#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"

#include <cstdint>
#include <SDL_pixels.h>
#include <SDL_rect.h>

namespace JadeEngine
{
  class LineStrip;

  struct LineBoxParams
  {
    ObjectLayer   layer;
    int32_t       width;
    int32_t       height;
    SDL_Color     color;
    int32_t       z;
  };

  class LineBox : public IGameObject
  {
  public:
    LineBox(const LineBoxParams& params);

    virtual void Update() override;
    virtual void Show(const bool shown) override;

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    int32_t GetX() const { return _rect.x; }
    int32_t GetY() const { return _rect.y; }

    int32_t GetCenterX() const { return _rect.x + _rect.w / 2; }
    int32_t GetCenterY() const { return _rect.x + _rect.h / 2; }

    int32_t GetWidth() const { return _rect.w; }
    int32_t GetHeight() const { return _rect.h; }

    void SetWidth(const int32_t width);
    void SetHeight(const int32_t height);

    void SetStartEnd(const int32_t startX, const int32_t startY, const int32_t endX, const int32_t endY);

  private:
    void UpdatePoints();

    LineStrip* _strip;
    SDL_Rect _rect;
  };
}
