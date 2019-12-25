#include "LineBox.h"

#include "Game.h"
#include "LineStrip.h"

namespace JadeEngine
{
  LineBox::LineBox(const LineBoxParams& params)
    : _rect{0, 0, params.width, params.height}
  {
    LineStripParams stripParams;
    stripParams.layer = params.layer;
    stripParams.initialPoints = {};
    stripParams.color = params.color;
    stripParams.z = params.z;

    _strip = GGame.Create<LineStrip>(stripParams);

    Show(true);
  }

  void LineBox::Update()
  {

  }

  void LineBox::Show(bool shown)
  {
    ICompositeObject::Show(shown);
    _strip->Show(IsShown() && _rect.w >= 2 && _rect.h >= 2);
  }

  void LineBox::SetPosition(int32_t x, int32_t y)
  {
    _rect = { x, y, _rect.w, _rect.h };
    UpdatePoints();
  }

  void LineBox::SetCenterPosition(int32_t x, int32_t y)
  {
    _rect = { x - _rect.w / 2, y - _rect.h / 2, _rect.w, _rect.h };
    UpdatePoints();
  }

  void LineBox::SetWidth(const int32_t width)
  {
    _rect = { _rect.x, _rect.y, width, _rect.h };
    UpdatePoints();
  }

  void LineBox::SetHeight(const int32_t height)
  {
    _rect = { _rect.x, _rect.y, _rect.w, height };
    UpdatePoints();
  }

  void LineBox::SetStartEnd(const int32_t startX, const int32_t startY, const int32_t endX, const int32_t endY)
  {
    const auto minX = std::min(startX, endX);
    const auto minY = std::min(startY, endY);

    const auto maxX = std::max(startX, endX);
    const auto maxY = std::max(startY, endY);

    _rect = { minX, minY, maxX - minX, maxY - minY };
    UpdatePoints();
  }

  void LineBox::UpdatePoints()
  {
    if (_rect.w >= 2 && _rect.h >= 2)
    {
      _strip->SetPoints({{_rect.x, _rect.y}, {_rect.x + _rect.w, _rect.y}, {_rect.x + _rect.w, _rect.y + _rect.h},
        {_rect.x, _rect.y + _rect.h}, {_rect.x, _rect.y + _rect.h}, {_rect.x, _rect.y}});
    }
    else
    {
      _strip->SetPoints({});
    }
  }
}