#pragma once

#include "EngineDataTypes.h"
#include "SDL.h"
#include "Vector2D.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <iterator>
#include <utility>
#include <vector>

#ifdef NDEBUG
#define SDL_ASSERT_SUCCESS(sdlCall) sdlCall
#else
#define SDL_ASSERT_SUCCESS(sdlCall) assert(sdlCall == 0)
#endif

namespace JadeEngine
{
  inline bool IsInsideRect(int32_t x, int32_t y, const Rectangle& rect)
  {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
  }

  inline bool RectsIntersect(const Rectangle& a, const Rectangle& b)
  {
    SDL_Point aOrigin = { a.x, a.y };
    SDL_Point bOrigin = { b.x, b.y };
    SDL_Point aEnd = { a.x, a.y };
    SDL_Point bEnd = { b.x + b.w, b.y + b.h };

    if (SDL_PointInRect(&aOrigin, &b) != SDL_FALSE && SDL_PointInRect(&aEnd, &b))
    {
      return true;
    }

    if (SDL_PointInRect(&bOrigin, &a) != SDL_FALSE && SDL_PointInRect(&bEnd, &a))
    {
      return true;
    }

    if (SDL_HasIntersection(&a, &b) != SDL_FALSE)
    {
      return true;
    }

    return false;
  }

  template<typename T>
  int Sign(const T& val)
  {
    return (T{0} < val) - (val < T{ 0 });
  }

  inline int ClosestNumberToNDivisibleByM(int32_t n, int32_t m)
  {
    // find the quotient
    int32_t q = n / m;

    // 1st possible closest number
    int32_t n1 = m * q;

    // 2nd possible closest number
    int32_t n2 = (n * m) > 0 ? (m * (q + 1)) : (m * (q - 1));

    // if true, then n1 is the required closest number
    if (abs(n - n1) < abs(n - n2))
    {
      return n1;
    }

    // else n2 is the required closest number
    return n2;
  }

  template<typename T>
  inline T Clamp(const T& value, const T& minimum, const T& maximum)
  {
    return (std::min)(maximum, (std::max)(value, minimum));
  }

  template<typename T>
  inline T Clamp01(const T& value)
  {
    return Clamp(value, T{ 0 }, T{ 1 });
  }

  template<typename T>
  inline std::pair<T, bool> MoveTowardsDone(const T& current, const T& wanted, const T& maxChange)
  {
    assert(maxChange >= T{ 0 });

    const auto diff = std::abs(current - wanted);
    if (diff <= std::abs(maxChange))
    {
      return std::make_pair(wanted, true);
    }
    else
    {
      return std::make_pair(current + Sign(wanted - current) * maxChange, false);
    }
  }

  inline std::pair<Vector2D_f32, bool> MoveTowardsDone(const Vector2D_f32& current, const Vector2D_f32& wanted, const float& maxChange)
  {
    assert(maxChange >= 0.0f);
    const auto diffSq = (current - wanted).SizeSq();
    if (diffSq <= maxChange * maxChange)
    {
      return std::make_pair(wanted, true);
    }
    else
    {
      return std::make_pair(current +  (wanted - current).Normalized() * maxChange, false);
    }
  }

  template<typename T>
  inline T MoveTowards(const T& current, const T& wanted, const T& maxChange)
  {
    const auto result = MoveTowardsDone(current, wanted, maxChange);
    return result.first;
  }

  template<typename T>
  inline T Interpolate(const T& start, const T& end, const float t)
  {
    return static_cast<T>(start + (end - start) * Clamp01(t));
  }

  inline SDL_Color ChangedBrigthness(const SDL_Color& color, const float brightnessChange)
  {
    return {
      Clamp(static_cast<uint8_t>(color.r * brightnessChange), uint8_t{0}, uint8_t{255}),
      Clamp(static_cast<uint8_t>(color.g * brightnessChange), uint8_t{0}, uint8_t{255}),
      Clamp(static_cast<uint8_t>(color.b * brightnessChange), uint8_t{0}, uint8_t{255}),
      color.a
    };
  }

  inline bool operator==(const SDL_Color& lhs, const SDL_Color& rhs)
  {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
  }

  inline bool operator!=(const SDL_Color& lhs, const SDL_Color& rhs)
  {
    return !(lhs == rhs);
  }

  inline std::string CamelCaseToSnakeCase(const std::string& camelCase)
  {
    std::vector<char> titleSnakeCase;
    titleSnakeCase.reserve(camelCase.length());
    for (size_t i = 0; i < camelCase.length(); i++)
    {
      if (i == 0)
      {
        titleSnakeCase.push_back(std::toupper(camelCase[i]));
      }
      else if (std::isupper(camelCase[i]))
      {
        titleSnakeCase.push_back(' ');
        titleSnakeCase.push_back(camelCase[i]);
      }
      else
      {
        titleSnakeCase.push_back(camelCase[i]);
      }
    }
    titleSnakeCase.push_back('\0');

    return std::string{ titleSnakeCase.data() };
  }


}
