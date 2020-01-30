#pragma once

#include <cstdint>
#include <SDL_rect.h>

namespace JadeEngine
{
  /**
  Vector represents a two dimensional integral point or vector in the screen or world coordinate system.
  */
  class Vector
  {
  public:
    Vector();
    Vector(const int32_t xPositionWSize, const int32_t yPositionHSize);
    Vector(const Vector& other);
    Vector(Vector&& other);
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);

    union { int32_t x; int32_t w; };
    union { int32_t y; int32_t h; };

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator/(const int32_t number) const;
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
  };

  class Box
  {
  public:
    Box();
    Box(const SDL_Rect& other);
    Box(const Vector& position, const Vector& size);
    Box(const int32_t x, const int32_t y, const int32_t w, const int32_t h);

    Box(const Box& other);
    Box(Box&& other);
    Box& operator=(const Box& other);
    Box& operator=(Box&& other);

    operator SDL_Rect() const;

    Vector position;
    Vector size;
  };
}