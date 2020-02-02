#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "Vector2D.h"

#include <SDL_rect.h>
#include <vector>

struct SDL_Renderer;

namespace JadeEngine
{
  /**
  Parameters required to create a LineStrip %game object.

  @see Game::Create

  Template to copy:
  @code
  LineStripParams lineStripParams;
  lineStripParams.layer = kObjectLayer_ui;
  lineStripParams.initialPoints = {kZeroVector2D_i32, {1.0f, 0.0f}};
  lineStripParams.z = 0;
  lineStripParams.color = kLighterDarkGreyColor;
  @endcode
  */
  struct LineStripParams
  {
    /**
    Layer that will the LineStrip belong to.

    @see ObjectLayer
    */
    ObjectLayer               layer;

    /**
    Points defining the LineStrip. The LineScript will consist of `initialPoints.size()-1` lines.

    The points are relative to the LineStrip's `transform` center position (`LineStrip::transform->GetCenterPosition()`). This means the whole strip can be moved around by changing the `transform`.

    Each point is a both a start and an end of a line segment apart from the very first and very last points which are only start and end points respectively.
    Assuming three different points are passed: A, B, C in that order than the LineStrip will look like A-B-C.
    In order to create a closed loop, a copy of the very first point should the very last point.

    @see LineStrip::SetPoints
    */
    std::vector<Vector2D_i32> initialPoints;

    /**
    Color that all the lines in LineStrip will be rendered with.
    */
    SDL_Color                 color;

    /**
    Z coordinate of the Sprite.

    The %game objects with higher Z coordinate will be drawn over the ones with lower one.
    */
    int32_t                   z;
  };

  /**
  %Game object that renders a sequence of connected line segments on screen, each 1 pixel thick.

  @see LineStripParams
  */
  class LineStrip : public IGameObject
  {
  public:
    /**
    Constructor for LineStrip that accepts LineStripParams.

    @warning One should use `Game::Create` function to create LineStrips instead of constructing the object directly.

    @see Game::Create, LineStripParams
    */
    LineStrip(const LineStripParams& params);

    void Render(SDL_Renderer* renderer) override;
    void Update() override;

    /**
    Changed the points that define the LineStrip. The LineScript will now consist of `points.size()-1` lines.

    The points are relative to the LineStrip's `transform` center position (`LineStrip::transform->GetCenterPosition()`). This means the whole strip can be moved around by changing the `transform`.

    Each point is a both a start and an end of a line segment apart from the very first and very last points which are only start and end points respectively.
    Assuming three different points are passed: A, B, C in that order than the LineStrip will look like A-B-C.
    In order to create a closed loop, a copy of the very first point should the very last point.

    @see Transform
    */
    void SetPoints(const std::vector<Vector2D_i32>& points);
  private:
    void UpdatePoints();

    SDL_Color _color;
    std::vector<SDL_Point> _points;
    std::vector<Vector2D_i32> _localPoints;
    ObjectLayer _layer;
  };
}
