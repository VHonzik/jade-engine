#pragma once

#include "EngineDataTypes.h"
#include "IGameObject.h"
#include "ObjectLayer.h"

#include <cstdint>
#include <SDL_pixels.h>


namespace JadeEngine
{
  class LineStrip;

  struct LineBoxParams
  {
    ObjectLayer   layer;
    Vector2D_i32  size;
    SDL_Color     color;
    int32_t       z;
  };

  class LineBox : public IGameObject
  {
  public:
    LineBox(const LineBoxParams& params);

    virtual void Update() override;
    virtual void Show(const bool shown) override;

  private:
    void UpdatePoints();

    LineStrip* _strip;
  };
}
