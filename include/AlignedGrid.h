#pragma once

#include "Aligment.h"

#include <cstdint>

using namespace JadeEngine::detail;

namespace JadeEngine
{
  class AlignedGrid
  {
  public:
    AlignedGrid();
    AlignedGrid(const uint32_t rows, const uint32_t columns, const int32_t width, const int32_t height);

    template<typename T>
    bool AddToPosition(T* element, const size_t x, const size_t y)
    {
      if (!CheckPosition(x, y)) return false;
      const auto index = PositionToIndex(x, y);

      AddSingle(element, index);
      Align();
      return true;
    }

    template<class InputIt>
    void AddAll(InputIt first, InputIt last)
    {
      size_t index = 0;
      for (auto it = first; it != last; ++it)
      {
        AddSingle(*it, index);
        index++;
      }
      Align();
    }

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

  private:
    void Align();

    template<typename T>
    void AddSingle(T* element, const size_t index)
    {
      _elements[index] = make_container_element<T>(element);
    }

    bool CheckPosition(const size_t x, const size_t y) const { return x < _columns && y < _rows; }
    size_t PositionToIndex(const size_t x, const size_t y) const { return y * _columns + x; }
    std::pair<uint32_t, uint32_t> IndexToPosition(const size_t index) const
    {
      return std::make_pair(static_cast<uint32_t>(index / _columns), static_cast<uint32_t>(index % _columns));
    }

    uint32_t _rows;
    uint32_t _columns;

    int32_t _x;
    int32_t _y;
    int32_t _width;
    int32_t _height;

    std::vector<ContainerElement> _elements;
  };
}


