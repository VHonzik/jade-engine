#pragma once

#include <SDL_rect.h>

namespace JadeEngine
{
  /**
  A structure that represent a rectangle with positional elements `x` and `y` and size elements `w` (short for width) and `h` (short for height).

  Generally used to represent a rectangle on the screen in pixels with `x` and `y` representing the top-left of the rectangle and `w` and `h` representing the width and height respectively.
  */
  using Rectangle = SDL_Rect;
}
