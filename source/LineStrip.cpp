#include "LineStrip.h"

#include "Camera.h"

#include <algorithm>
#include <SDL.h>

namespace JadeEngine
{
  LineStrip::LineStrip(const LineStripParams& params)
    : _color(params.color)
    , _layer(params.layer)
  {
    transform->Initialize(0, 0, 0, 0);
    _z = params.z;
    SetPoints(params.initialPoints);
  }


  void LineStrip::Render(SDL_Renderer* renderer)
  {
    SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
    if (_layer == kObjectLayer_world)
    {
      std::vector<SDL_Point> transformedPoints;
      std::transform(std::cbegin(_points), std::cend(_points), std::back_inserter(transformedPoints),
        [](const SDL_Point& point)
      {
        return GWorldCamera.WorldToScreen(point);
      });
      SDL_RenderDrawLines(renderer, transformedPoints.data(), static_cast<int32_t>(transformedPoints.size()));
    }
    else
    {
      SDL_RenderDrawLines(renderer, _points.data(), static_cast<int32_t>(_points.size()));
    }
  }

  void LineStrip::UpdatePoints()
  {
    _points.clear();
    const auto centerPosition = transform->GetCenterPosition();
    std::transform(std::cbegin(_localPoints), std::cend(_localPoints), std::back_inserter(_points), [&](const Vector& point)
    {
      const auto position = centerPosition + point;
      return SDL_Point{ position.x, position.y };
    });
  }

  void LineStrip::SetPoints(const std::vector<Vector>& points)
  {
    _localPoints = points;
    UpdatePoints();
  }

  void LineStrip::Update()
  {
    if (transform->IsDirty(kDirtyFlag_centerPosition))
    {
      UpdatePoints();
    }
  }
}