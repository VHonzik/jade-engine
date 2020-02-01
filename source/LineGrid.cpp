#include "LineGrid.h"

#include "Game.h"
#include "LineStrip.h"
#include "Transform.h"

namespace JadeEngine
{
  LineGrid::LineGrid(const LineGridParams& params)
    : _width(params.width)
    , _height(params.height)
    , _gridSize(params.gridSize)
    , _horizontalAlignment(params.horizontalAlignment)
    , _verticalAlignment(params.verticalAlignment)
  {
    const auto count = (_width / _gridSize) + (_height / _gridSize) + (_verticalAlignment != kVerticalAlignment_Center ? 1 : 0) + (_horizontalAlignment != kHorizontalAlignment_Center ? 1 : 0);
    _strips.resize(count);

    LineStripParams stripParams;
    stripParams.layer = params.layer;
    stripParams.initialPoints = {};
    stripParams.color = params.color;
    stripParams.z = params.z;

    for (size_t i = 0; i < count; i++)
    {
      _strips[i] = GGame.Create<LineStrip>(stripParams);
    }
    SetPosition(0, 0);
  }

  void LineGrid::Update()
  {

  }

  void LineGrid::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    for (auto strip : _strips)
    {
      strip->Show(isShown);
    }
  }

  void LineGrid::SetPosition(int32_t x, int32_t y)
  {
    _x = x;
    _y = y;
    const auto rows = _height / _gridSize + (_verticalAlignment == kVerticalAlignment_Center ? 0 : 1);
    const auto columns = _width / _gridSize + (_horizontalAlignment == kHorizontalAlignment_Center ? 0 : 1);

    for (int32_t i = 0; i < rows; i++)
    {
      Vector2D_i32 origin = {x, y};
      switch (_verticalAlignment)
      {
      case kVerticalAlignment_Top:
        origin.y = y;
        break;
      case kVerticalAlignment_Center:
        origin.y = y + _gridSize / 2;
        break;
      case kVerticalAlignment_Bottom:
        origin.y = y + (_height % _gridSize) - 1;
        break;
      }
      const Vector2D_i32 start = { 0, i * _gridSize };
      const Vector2D_i32 end = { _width, i * _gridSize };
      _strips[i]->transform->SetPosition(origin);
      _strips[i]->SetPoints({ start, end });
    }

    for (int32_t i = 0; i < columns; i++)
    {
      Vector2D_i32 origin = { x, y };
      switch (_horizontalAlignment)
      {
      case kHorizontalAlignment_Left:
        origin.x = x;
        break;
      case kHorizontalAlignment_Center:
        origin.x = x + _gridSize / 2;
        break;
      case kHorizontalAlignment_Right:
        origin.x = x + (_width % _gridSize) - 1;
        break;
      }

      const Vector2D_i32 start = { i * _gridSize, 0 };
      const Vector2D_i32 end = { i * _gridSize, _height };
      _strips[i]->transform->SetPosition(origin);
      _strips[rows + i]->SetPoints({ start, end });
    }
  }

  void LineGrid::SetCenterPosition(int32_t x, int32_t y)
  {
    SetPosition(x - _width / 2, y - _height / 2);
  }
}