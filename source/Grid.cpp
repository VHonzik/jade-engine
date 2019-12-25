#include "AlignedGrid.h"

#include <algorithm>
#include <limits>

namespace JadeEngine
{
  AlignedGrid::AlignedGrid(const uint32_t rows, const uint32_t columns, const int32_t width, const int32_t height)
    : _rows(rows)
    , _columns(columns)
    , _width(width)
    , _height(height)
    , _elements(rows * columns, ContainerElement())
    , _x(0)
    , _y(0)
  {
  }

  AlignedGrid::AlignedGrid()
    : AlignedGrid(0, 0, 0, 0)
  {
  }

  void AlignedGrid::SetPosition(int32_t x, int32_t y)
  {
    _x = x;
    _y = y;
    Align();
  }

  void AlignedGrid::SetCenterPosition(int32_t x, int32_t y)
  {
    _x = x - _width / 2;
    _y = y - _height / 2;
    Align();
  }

  void AlignedGrid::Align()
  {
    auto largestWidth = std::numeric_limits<int32_t>::min();
    auto largestHeight = std::numeric_limits<int32_t>::min();
    for (size_t i = 0; i < _rows; i++)
    {
      for (size_t j = 0; j < _columns; j++)
      {
        largestWidth = std::max(largestWidth, _elements[PositionToIndex(j, i)].GetWidth());
        largestHeight = std::max(largestHeight, _elements[PositionToIndex(j, i)].GetHeight());
      }
    }

    const auto remainingWidth = _width - _columns * largestWidth;
    const auto remainingHeight = _height - _rows * largestHeight;
    const auto numberOfHorizontalSpaces = _columns - 1;
    const auto numberOfVerticalSpaces = _rows - 1;
    const auto horizontalSpacing = remainingWidth / numberOfHorizontalSpaces;
    const auto verticalSpacing = remainingHeight / numberOfVerticalSpaces;

    for (size_t i = 0; i < _rows; i++)
    {
      for (size_t j = 0; j < _columns; j++)
      {
        const auto x = _x + largestWidth / 2 + static_cast<int32_t>(j) * (largestWidth / 2 + horizontalSpacing);
        const auto y = _y + largestHeight / 2 + static_cast<int32_t>(i) * (largestHeight / 2 + verticalSpacing);
        _elements[PositionToIndex(j, i)].SetElementCenterPosition(x, y);
      }
    }
  }
}