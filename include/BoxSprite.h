#pragma once

#include "Sprite.h"

namespace JadeEngine
{
  struct BoxSpriteParams : public SpriteParams
  {
    int32_t cornerSize;
    int32_t width;
    int32_t height;
  };

  class BoxSprite : public Sprite
  {
  public:
    BoxSprite(const BoxSpriteParams& params);
    void DoRender(SDL_Renderer* renderer) override;

    void SetWidth(int32_t width) override;
    void SetHeight(int32_t height) override;

    void SetScale(const int32_t x, const int32_t y);

    void SetBoundingBox(const SDL_Rect& box);
    const SDL_Rect& GetBoundingBox() const override;
    bool HasHitTest() const override { return false; }
  private:
    SDL_Rect _boundingBox;
    bool _boundingBoxOverwritten;
    int32_t _cornerSize;
    int32_t _scaleX;
    int32_t _scaleY;
  };
}
