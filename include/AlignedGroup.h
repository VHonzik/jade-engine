#pragma once

#include "Aligment.h"

#include <cstdint>
#include <vector>

using namespace JadeEngine::detail;

namespace JadeEngine
{
  enum GroupDirection
  {
    kGroupDirection_Vertical,
    kGroupDirection_Horizontal
  };

  class AlignedGroup
  {
  public:
    AlignedGroup();
    AlignedGroup(const GroupDirection direction,
      const HorizontalAlignment horizontalAlignment, const VerticalAlignment verticalAlignment,
      const int32_t spacing);
    AlignedGroup(const GroupDirection direction,
      const HorizontalAlignment horizontalAlignment,const VerticalAlignment verticalAlignment,
      const int32_t spacing, const int32_t x, const int32_t y);

    template<typename... Args>
    void AddVA(Args... args)
    {
      AddRecurse(args...);
      Align();
    }

    template<class InputIt>
    void Add(InputIt first, InputIt last)
    {
      for (auto it = first; it != last; ++it)
      {
        AddRecurse(*it);
      }
      Align();
    }

    void Align();

    void SetParameters(const HorizontalAlignment horizontalAlignment,
      const VerticalAlignment verticalAlignment, const int32_t spacing);
    void SetPosition(const int32_t x, const int32_t y);

  private:
    template<typename T>
    void AddRecurse(T* element)
    {
      _elements.push_back(make_container_element<T>(element));
    }

    template<typename Type, typename... Args>
    void AddRecurse(Type* first, Args... args)
    {
      AddRecurse(first);
      AddRecurse(args...);
    }

    void CalculateDimensions();
    void AlignVerticalX();
    void AlignVerticalY();

    void AlignHorizontalX();
    void AlignHorizontalY();

    GroupDirection _direction;
    int32_t _spacing;
    HorizontalAlignment _horizontalAlignment;
    VerticalAlignment _verticalAlignment;
    std::vector<ContainerElement> _elements;

    int32_t _width;
    int32_t _height;

    int32_t _x;
    int32_t _y;
  };
}
