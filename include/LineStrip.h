#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "Vector.h"

#include <SDL_rect.h>
#include <vector>

struct SDL_Renderer;

namespace JadeEngine
{
  struct LineStripParams
  {
    ObjectLayer             layer;
    std::vector<Vector>     initialPoints;
    SDL_Color               color;
    int32_t                 z;
  };

  class LineStrip : public IGameObject
  {
  public:
    LineStrip(const LineStripParams& params);
    void Render(SDL_Renderer* renderer) override;
    void SetPoints(const std::vector<Vector>& points);
    void Update() override;
  private:
    void UpdatePoints();

    SDL_Color _color;
    std::vector<SDL_Point> _points;
    std::vector<Vector> _localPoints;
    ObjectLayer _layer;
  };
}
