#pragma once

#include <cstdint>
#include <SDL_rect.h>
#include <type_traits>

namespace JadeEngine
{
  /**
  Vector2F represents a two dimensional integral point or vector in the screen or world coordinate system.
  */
  template<typename ValueType>
  class Vector2D
  {
  public:
    Vector2D()
      : Vector2D(0, 0)
    {
    }

    Vector2D(const ValueType xPositionWSize, const ValueType yPositionHSize)
      : x(xPositionWSize)
      , y(yPositionHSize)
    {
    }

    Vector2D(const Vector2D& other)
    {
      x = other.x;
      y = other.y;
    }

    Vector2D(Vector2D&& other)
      : x(std::move(other.x))
      , y(std::move(other.y))
    {
    }

    Vector2D& operator=(const Vector2D& other)
    {
      if (this != &other)
      {
        x = other.x;
        y = other.y;
      }
      return *this;
    }

    Vector2D& operator=(Vector2D&& other)
    {
      if (this != &other)
      {
        x = std::move(other.x);
        y = std::move(other.y);
      }

      return *this;
    }

    union { ValueType x; ValueType w; };
    union { ValueType y; ValueType h; };

    Vector2D operator+(const Vector2D& other) const
    {
      return { x + other.x, y + other.y };
    }

    Vector2D operator-(const Vector2D& other) const
    {
      return { x - other.x, y - other.y };
    }

    Vector2D operator/(const ValueType number) const
    {
      return { x / number, y / number };
    }

    Vector2D operator*(const ValueType number) const
    {
      return { x * number, y * number };
    }

    Vector2D operator*(const Vector2D& other) const
    {
      return { x * other.x, y * other.y };
    }

    Vector2D operator/(const Vector2D& other) const
    {
      return { x / other.x, y / other.y };
    }

    bool operator==(const Vector2D& other) const
    {
      return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2D& other) const
    {
      return !(*this == other);
    }

    bool operator<(const Vector2D& other) const
    {
      return x < other.x && y < other.y;
    }

    ValueType SizeSq() const
    {
      return x * x + y * y;
    }

    float Size() const
    {
      return std::sqrt(SizeSq());
    }

    Vector2D Normalized() const
    {
      static_assert(std::is_floating_point<ValueType>::value);
      return *this / Size();
    }

    template<typename OtherValueType>
    Vector2D<OtherValueType> Convert() const
    {
      static_assert(!std::is_same<ValueType, OtherValueType>::value);
      return { static_cast<OtherValueType>(x),  static_cast<OtherValueType>(y) };
    }
  };

  using Vector2D_i32 = Vector2D<int32_t>;
  using Vector2D_f32 = Vector2D<float>;

  const auto kZeroVector2D_i32 = Vector2D_i32(0, 0);
  const auto kOneOneVector2D_i32 = Vector2D_i32(1, 1);
  const auto kOneOneVector2D_f32 = Vector2D_f32(1.0f, 1.0f);

  template<typename ValueType>
  class Box
  {
  public:
    Box()
      : Box(0, 0, 0, 0)
    {
    }

    Box(const SDL_Rect& other)
      : Box(other.x, other.y, other.w, other.h)
    {
    }

    Box(const Vector2D<ValueType>& position, const Vector2D<ValueType>& size)
      : Box(position.x, position.y, size.w, size.h)
    {
    }

    Box(const int32_t x, const int32_t y, const int32_t w, const int32_t h)
      : position{ x, y }
      , size{ w, h }
    {
    }

    Box(const Box& other)
      : Box(other.position, other.size)
    {
    }

    Box(Box&& other)
      : position(std::move(other.position))
      , size(std::move(other.size))
    {
    }

    Box& operator=(const Box& other)
    {
      if (this != &other)
      {
        position = other.position;
        size = other.size;
      }
      return *this;
    }

    Box& operator=(Box&& other)
    {
      if (this != &other)
      {
        position = std::move(other.position);
        size = std::move(other.size);
      }
      return *this;
    }


    operator SDL_Rect() const
    {
      return { position.x, position.y, size.w, size.h };
    }

    Vector2D<ValueType> position;
    Vector2D<ValueType> size;
  };

  using Box_i32 = Box<int32_t>;
}