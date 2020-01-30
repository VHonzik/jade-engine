#include "Vector.h"

#include <algorithm>

namespace JadeEngine
{
  Vector::Vector()
    : x(0)
    , y(0)
  {
  }

  Vector::Vector(const int32_t xPositionWSize, const int32_t yPositionHSize)
  {
    x = xPositionWSize;
    y = yPositionHSize;
  }

  Vector::Vector(const Vector& other)
  {
    x = other.x;
    y = other.y;
  }

  Vector::Vector(Vector&& other)
    : x(std::move(other.x))
    , y(std::move(other.y))
  {
  }

  Vector& Vector::operator=(const Vector& other)
  {
    if (this != &other)
    {
      x = other.x;
      y = other.y;
    }

    return *this;
  }

  Vector& Vector::operator=(Vector&& other)
  {
    if (this != &other)
    {
      x = std::move(other.x);
      y = std::move(other.y);
    }

    return *this;
  }

  Vector Vector::operator+(const Vector& other) const
  {
    return { x + other.x, y + other.y };
  }

  Vector Vector::operator-(const Vector& other) const
  {
    return { x - other.x, y - other.y };
  }

  Vector Vector::operator/(const int32_t number) const
  {
    return { x / number, y / number };
  }

  bool Vector::operator==(const Vector& other) const
  {
    return x == other.x && y == other.y;
  }

  bool Vector::operator!=(const Vector& other) const
  {
    return !(*this == other);
  }

  Box::Box()
    : Box(0, 0, 0, 0)
  {
  }

  Box::Box(const SDL_Rect& other)
    : Box(other.x, other.y, other.w, other.h)
  {
  }

  Box::Box(const Vector& position, const Vector& size)
    : Box(position.x, position.y, size.w, size.h)
  {
  }

  Box::Box(const int32_t x, const int32_t y, const int32_t w, const int32_t h)
  {
    position = { x, y };
    size = { w, h };
  }

  Box::Box(const Box& other)
    : Box(other.position, other.size)
  {
  }

  Box::Box(Box&& other)
    : Box(std::move(other.position), std::move(other.size))
  {
  }

  Box& Box::operator=(const Box& other)
  {
    if (this != &other)
    {
      position = other.position;
      size = other.size;
    }
    return *this;
  }

  Box& Box::operator=(Box&& other)
  {
    if (this != &other)
    {
      position = std::move(other.position);
      size = std::move(other.size);
    }
    return *this;
  }

  Box::operator SDL_Rect() const
  {
    return { position.x, position.y, size.w, size.h };
  }
}