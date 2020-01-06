#pragma once

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

  class LineStrip
  {
  public:
    LineStrip(const LineStripParams& params);
    void Render(SDL_Renderer* renderer);
    void DoRender(SDL_Renderer* renderer);

    void SetPoints(const std::vector<SDL_Point>& points);

    void Show(const bool shown) { _shown = shown; }
    bool IsShown() const { return _shown; }

    int GetZ() const { return _z; }
  private:
    SDL_Color _color;
    std::vector<SDL_Point> _points;
    int _z;
    bool _shown;
    ObjectLayer _layer;
  };
}
