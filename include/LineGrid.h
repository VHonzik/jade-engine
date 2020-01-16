#pragma once

#include "Aligment.h"
#include "IGameObject.h"
#include "ObjectLayer.h"

#include <SDL_pixels.h>
#include <vector>

namespace JadeEngine
{
  class LineStrip;

  struct LineGridParams
  {
    ObjectLayer           layer;
    int32_t               width;
    int32_t               height;
    int32_t               gridSize;
    HorizontalAlignment   horizontalAlignment;
    VerticalAlignment     verticalAlignment;
    SDL_Color             color;
    int32_t               z;
  };

  class LineGrid : public IGameObject
  {
  public:
    LineGrid(const LineGridParams& params);
    virtual void Update() override;
    virtual void Show(const bool shown) override;

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    int32_t GetX() const { return _x; }
    int32_t GetY() const { return _y; }

    int32_t GetCenterX() const { return _x + _width / 2; }
    int32_t GetCenterY() const { return _y + _height / 2; }

    int32_t GetWidth() const { return _width; }
    int32_t GetHeight() const { return _height; }
  private:
    std::vector<LineStrip*> _strips;
    int32_t _width;
    int32_t _height;
    int32_t _x;
    int32_t _y;
    int32_t _gridSize;
    HorizontalAlignment _horizontalAlignment;
    VerticalAlignment _verticalAlignment;
  };
}
