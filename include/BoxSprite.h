#pragma once

#include "Sprite.h"
#include "Vector2D.h"

namespace JadeEngine
{
  struct BoxSpriteParams : public SpriteParams
  {
    int32_t cornerSize;
    Vector2D_i32 size;
  };

  class BoxSprite : public Sprite
  {
  public:
    BoxSprite(const BoxSpriteParams& params);
    void Render(SDL_Renderer* renderer) override;

    void SetScale(const int32_t x, const int32_t y);
    bool HasHitTest() const override { return false; }
  private:
    int32_t _cornerSize;
    int32_t _scaleX;
    int32_t _scaleY;
  };
}
