#pragma once

#include "ObjectLayer.h"

#include <memory>
#include <SDL.h>
#include <vector>

namespace JadeEngine
{
  struct Texture;
  struct SpriteSheetDescription;

  struct SpriteParams
  {
    ObjectLayer               layer;
    std::string               textureName;
    std::shared_ptr<Texture>  texture;
    int32_t                   z;
    bool                      spriteSheet;
    std::string               spriteSheetName;
  };

  class Sprite
  {
  public:
    Sprite(const SpriteParams& params);
    virtual void DoRender(SDL_Renderer* renderer);
    virtual bool DoPreload(SDL_Renderer* renderer) { return true; };
    virtual void Update() {};
    virtual bool RequiresPreload() const { return !_preloaded; };
    virtual void Clean();
    virtual void Tint(const SDL_Color& tintColor);

    virtual void SetAlpha(float alpha);
    virtual float GetAlpha() const;

    void SetRotation(const double angle);

    virtual const SDL_Rect& GetBoundingBox() const;
    SDL_Rect GetCollisionBox() const;
    virtual bool HitTest(uint32_t x, uint32_t y) const;
    virtual bool HasHitTest() const;

    void Preload(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);

    void Show(bool shown);
    bool IsShown() const { return _shown; }

    void SetPosition(uint32_t x, uint32_t y);
    void SetCenterPosition(uint32_t x, uint32_t y);

    int32_t GetX() const { return _transform.x; }
    int32_t GetY() const { return _transform.y; }

    int32_t GetCenterX() const { return _transform.x + _transform.w / 2; }
    int32_t GetCenterY() const { return _transform.y + _transform.h / 2; }

    int32_t GetWidth() const { return _transform.w; }
    int32_t GetHeight() const { return _transform.h; }

    virtual void SetWidth(int32_t width);
    virtual void SetHeight(int32_t height);

    void SetWidthHeight(int32_t width, int32_t height);

    void SetSpriteSheetSprite(const std::string& sprite);
    void SetSpriteSheetMask(const SDL_Rect& mask);

    int GetZ() const { return _z; }
    const SDL_Rect& GetTransform() const { return _transform; }
    const std::string& GetTextureName() const;
    const std::shared_ptr<Texture>& GetTextureDescription() const { return _textureDescription; }

  protected:
    std::shared_ptr<Texture> _textureDescription;
    const SpriteSheetDescription* _spriteSheetDescription;

    SDL_Rect _transform;
    SDL_Rect _boundingBox;

    SDL_Texture* _texture;

    bool _spriteSheetMasked;
    SDL_Rect _spriteSheetMask;
    std::string _textureName;

    bool _preloaded;
    bool _shown;
    bool _rotated;

    int _z;
    float _alpha;
    double _rotationAngle;

    ObjectLayer _layer;
  };
}
