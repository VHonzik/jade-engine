#include "AlignedGroup.h"

#include "Button.h"
#include "Slider.h"
#include "BoxSprite.h"

#include <cassert>
#include <numeric>
#include <algorithm>

namespace JadeEngine
{
  AlignedGroup::AlignedGroup()
    : AlignedGroup(kGroupDirection_Vertical,
      kHorizontalAlignment_Center, kVerticalAlignment_Center,
      0, 0, 0)
  {
  }

  AlignedGroup::AlignedGroup(const GroupDirection direction,
    const HorizontalAlignment horizontalAlignment, const VerticalAlignment verticalAlignment,
    const int32_t spacing)
    : AlignedGroup(direction, horizontalAlignment, verticalAlignment,
      spacing, 0, 0)
  {
  }

  AlignedGroup::AlignedGroup(const GroupDirection direction,
    const HorizontalAlignment horizontalAlignment, const VerticalAlignment verticalAlignment,
    const int32_t spacing, const int32_t x, const int32_t y)
    : _horizontalAlignment(horizontalAlignment)
    , _verticalAlignment(verticalAlignment)
    , _spacing(spacing)
    , _height(0)
    , _width(0)
    , _x(x)
    , _y(y)
    , _direction(direction)
  {
  }

  void AlignedGroup::SetParameters(const HorizontalAlignment horizontalAlignment,
    const VerticalAlignment verticalAlignment, const int32_t spacing)
  {
    _horizontalAlignment = horizontalAlignment;
    _verticalAlignment = verticalAlignment;
    _spacing = spacing;
    Align();
  }

  void AlignedGroup::SetPosition(const int32_t x, const int32_t y)
  {
    _x = x;
    _y = y;
    Align();
  }


  void AlignedGroup::CalculateDimensions()
  {
    switch (_direction)
    {
    case kGroupDirection_Vertical:
    {
      _height = std::accumulate(std::cbegin(_elements), std::cend(_elements), 0,
        [](const int32_t& partial, const ContainerElement& next)
      {
        return partial + next.GetHeight();
      });

      _height += std::max(0, static_cast<int32_t>(_elements.size() - 1)) * _spacing;

      if (_elements.size())
      {
        _width = std::max_element(std::cbegin(_elements), std::cend(_elements),
          [](const ContainerElement& a, const ContainerElement& b)
        {
          return a.GetWidth() < b.GetWidth();
        })->GetWidth();
      }
      else
      {
        _width = 0;
      }

    }
    break;
    case kGroupDirection_Horizontal:
    {
      _width = std::accumulate(std::cbegin(_elements), std::cend(_elements), 0,
        [](const int32_t& partial, const ContainerElement& next)
      {
        return partial + next.GetWidth();
      });

      _height += std::max(0, static_cast<int32_t>(_elements.size() - 1)) * _spacing;

      if (_elements.size())
      {
        _height = std::max_element(std::cbegin(_elements), std::cend(_elements),
          [](const ContainerElement& a, const ContainerElement& b)
        {
          return a.GetHeight() < b.GetHeight();
        })->GetHeight();
      }
      else
      {
        _height = 0;
      }
    }
    break;
    default:
      assert(false); // Unknown direction type
      break;
    }
  }

  void AlignedGroup::AlignVerticalX()
  {
    for (auto& element : _elements)
    {
      switch (_horizontalAlignment)
      {
      case kHorizontalAlignment_Left:
        element.SetElementPosition(_x, element.GetY());
        break;
      case kHorizontalAlignment_Center:
        element.SetElementCenterPosition(_x, element.GetY());
        break;
      case kHorizontalAlignment_Right:
        element.SetElementPosition(_x - element.GetWidth(), element.GetY());
        break;
      default:
        assert(false); // Unknown horizontal alignment
        break;
      }
    }
  }

  void AlignedGroup::AlignVerticalY()
  {
    int32_t currentY;
    switch (_verticalAlignment)
    {
    case kVerticalAlignment_Center:
      currentY = _y - _height / 2;
      break;
    case kVerticalAlignment_Bottom:
      currentY = _y - _height;
      break;
    case kVerticalAlignment_Top:
      currentY = _y;
      break;
    default:
      assert(false); // Unknown vertical alignment
      break;
    }

    for (auto& element : _elements)
    {
      element.SetElementPosition(element.GetX(), currentY);
      currentY += element.GetHeight() + _spacing;
    }
  }

  void AlignedGroup::AlignHorizontalX()
  {
    int32_t currentX;
    switch (_horizontalAlignment)
    {
    case kHorizontalAlignment_Left:
      currentX = _x;
    case kHorizontalAlignment_Center:
      currentX = _x - _width / 2;
      break;
    case kHorizontalAlignment_Right:
      currentX = _x - _width;
      break;
    default:
      assert(false); // Unknown horizontal alignment
      break;
    }

    for (auto& element : _elements)
    {
      element.SetElementPosition(currentX, element.GetY());
      currentX += element.GetWidth() + _spacing;
    }
  }

  void AlignedGroup::AlignHorizontalY()
  {
    for (auto& element : _elements)
    {
      switch (_verticalAlignment)
      {
      case kVerticalAlignment_Top:
        element.SetElementPosition(element.GetX(), _y);
        break;
      case kVerticalAlignment_Center:
        element.SetElementCenterPosition(element.GetCenterX(), _y);
        break;
      case kVerticalAlignment_Bottom:
        element.SetElementPosition(element.GetX(), _y - element.GetHeight());
        break;
      default:
        assert(false); // Unknown vertical alignment
        break;
      }
    }
  }


  void AlignedGroup::Align()
  {
    CalculateDimensions();
    switch (_direction)
    {
    case kGroupDirection_Vertical:
      AlignVerticalX();
      AlignVerticalY();
      break;
    case kGroupDirection_Horizontal:
      AlignHorizontalX();
      AlignHorizontalY();
      break;
    default:
      assert(false); // Unknown direction
      break;
    }

  }
}