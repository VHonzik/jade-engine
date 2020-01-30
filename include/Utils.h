#pragma once

#include "EngineDataTypes.h"
#include "SDL.h"

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <iterator>
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

  constexpr uint32_t NumberOfBits(uint32_t x)
  {
    return x < 2 ? x : 1 + NumberOfBits(x >> 1);
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

  inline void Log(const char* format, ...)
  {
    va_list args;
    va_start(args, format);

    char buffer[128];
    std::string newLineFormat = format;
    newLineFormat += '\n';
    std::snprintf(buffer, 128, newLineFormat.c_str(), args);
    //OutputDebugString(buffer);
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
  inline T MoveTowards(const T& current, const T& wanted, const T& maxChange)
  {
    assert(maxChange >= T{0});

    const auto diff = std::abs(current - wanted);
    if (diff <= std::abs(maxChange))
    {
      return wanted;
    }
    else
    {
      return current + Sign(wanted - current) * maxChange;
    }
  }

  template<typename T>
  inline T Interpolate(const T& start, const T& end, const float t)
  {
    return static_cast<T>(start + (end - start) * Clamp01(t));
  }

  struct ClipperResult
  {
    bool intersect;
    SDL_Point first;
    SDL_Point second;
  };

  // Source: https://en.wikipedia.org/wiki/Liang%E2%80%93Barsky_algorithm
  inline ClipperResult LiangBarskyClipper(const Rectangle& rect, const SDL_Point& start, const SDL_Point& end)
  {
    ClipperResult result;

    float p1 = static_cast<float>(-(end.x - start.x));
    float p2 = static_cast<float>(-p1);
    float p3 = static_cast<float>(-(end.y - start.y));
    float p4 = static_cast<float>(-p3);

    float q1 = static_cast<float>(start.x - rect.x);
    float q2 = static_cast<float>((rect.x + rect.w) - start.x);
    float q3 = static_cast<float>(start.y - rect.y);
    float q4 = static_cast<float>((rect.y + rect.h) - start.y);

    std::vector<float> positives(5);
    std::vector<float> negatives(5);
    size_t positiveIndex = 1;
    size_t negativeIndex = 1;
    positives[0] = 1;
    negatives[0] = 0;

    if ((p1 == 0.0f && q1 < 0.0f) || (p3 == 0.0f && q3 < 0.0f))
    {
      result.intersect = false;
      return result;
    }

    if (p1 != 0.0f)
    {
      float r1 = q1 / p1;
      float r2 = q2 / p2;
      if (p1 < 0)
      {
        negatives[negativeIndex++] = r1;
        positives[positiveIndex++] = r2;
      }
      else
      {
        negatives[negativeIndex++] = r2;
        positives[positiveIndex++] = r1;
      }
    }

    if (p3 != 0)
    {
      float r3 = q3 / p3;
      float r4 = q4 / p4;
      if (p3 < 0) {
        negatives[negativeIndex++] = r3;
        positives[positiveIndex++] = r4;
      }
      else {
        negatives[negativeIndex++] = r4;
        positives[positiveIndex++] = r3;
      }
    }

    float rn1 = *std::max_element(std::begin(negatives), std::next(std::begin(negatives), negativeIndex));
    float rn2 = *std::min_element(std::begin(positives), std::next(std::begin(positives), positiveIndex));

    if (rn1 > rn2)
    {
      result.intersect = false;
      return result;
    }

    float xn1 = start.x + p2 * rn1;
    float yn1 = start.y + p4 * rn1;

    float xn2 = start.x + p2 * rn2;
    float yn2 = start.y + p4 * rn2;

    result.intersect = true;
    result.first = { static_cast<int32_t>(xn1), static_cast<int32_t>(yn1) };
    result.second = { static_cast<int32_t>(xn2), static_cast<int32_t>(yn2) };
    return result;
  }
}
