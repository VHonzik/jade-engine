#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"

#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <vector>

struct SDL_Renderer;

namespace JadeEngine
{
  struct LineStripParams
  {
    ObjectLayer             layer;
    std::vector<SDL_Point>  initialPoints;
    SDL_Color               color;
    int32_t                 z;
  };

  class LineStrip : public IGameObject
  {
  public:
    LineStrip(const LineStripParams& params);
    void Render(SDL_Renderer* renderer) override;

    void SetPoints(const std::vector<SDL_Point>& points);
  private:
    SDL_Color _color;
    std::vector<SDL_Point> _points;
    ObjectLayer _layer;
  };
}
