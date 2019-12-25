#include "LineGrid.h"

#include "LineStrip.h"

#include "Game.h"

namespace JadeEngine
{
  LineGrid::LineGrid(const LineGridParams& params)
    : _width(params.width)
    , _height(params.height)
    , _gridSize(params.gridSize)
    , _horizontalAlignment(params.horizontalAlignment)
    , _verticalAlignment(params.verticalAlignment)
  {
    const auto count = (_width / _gridSize) + (_height / _gridSize)
      + (_verticalAlignment != kVerticalAlignment_Center ? 1 : 0) + (_horizontalAlignment != kHorizontalAlignment_Center ? 1 : 0);
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
    ICompositeObject::Show(shown);
    for (auto strip : _strips)
    {
      strip->Show(shown);
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
      SDL_Point origin = {x, y};
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
      const SDL_Point start = { origin.x, origin.y + i * _gridSize };
      const SDL_Point end = { origin.x + _width, origin.y + i * _gridSize };
      _strips[i]->SetPoints({ start, end });
    }

    for (int32_t i = 0; i < columns; i++)
    {
      SDL_Point origin = { x, y };
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

      const SDL_Point start = { origin.x + i * _gridSize, origin.y };
      const SDL_Point end = { origin.x + i * _gridSize, origin.y + _height };
      _strips[rows + i]->SetPoints({ start, end });
    }
  }

  void LineGrid::SetCenterPosition(int32_t x, int32_t y)
  {
    SetPosition(x - _width / 2, y - _height / 2);
  }
}