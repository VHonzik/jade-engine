#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "TextureSampling.h"

#include <memory>
#include <SDL.h>
#include <vector>

namespace JadeEngine
{
  struct Texture;
  struct SpriteSheetDescription;

  /**
  Parameters required to create a Sprite %game object.

  @see Game::Create

  Template to copy:
  @code
  SpriteParams spriteParams;
  spriteParams.layer = kObjectLayer_ui;
  spriteParams.textureName = "someTexture.png";
  spriteParams.z = 0;
  spriteParams.spriteSheet = false;
  spriteParams.spriteSheetName = "";
  @endcode
  */
  struct SpriteParams
  {
    /**
    Layer that will the Sprite belong to.

    @see ObjectLayer
    */
    ObjectLayer               layer;
    /**
    Name of the texture that will be rendered by the sprite.

    In the case of `spriteSheet` being false, the `textureName` must corresponds to `assetName` in `GameInitParams::textures` that was used when initializing the game.
    In the case of `spriteSheet` being true, the `textureName` must be the name of one of the files that were packed into sprite-sheet - it must be present in sprite-sheet JSON.

    @note If the texture is not found a default bright pink fall-back texture is used.

    @see SpriteParams::spriteSheet
    */
    std::string               textureName;
    /**
    Z coordinate of the Sprite.

    The game objects with higher Z coordinate will be drawn over the ones with lower one.
    */
    int32_t                   z;
    /**
    Whether the texture is part of a sprite-sheet file or stand-alone texture file.

    Affects the meaning of `textureName`.

    If true `spriteSheetName` must be specified.

    @see SpriteParams::textureName
    */
    bool                      spriteSheet;
    /**
    Name of the sprite-sheet corresponding to `GameInitParamsSpriteSheetEntry::assetName` that was used when initializing the game.

    Has no effect if `spriteSheet` is false.

    @see SpriteParams::spriteSheet
    */
    std::string               spriteSheetName;
  };

  /**
  %Game object that renders a texture on screen.

  @see SpriteParams
  */
  class Sprite : public IGameObject
  {
  public:
    /**
    Constructor for Sprite that accepts SpriteParams.

    @warning One should use `Game::Create` function to create Sprites instead of constructing the object directly.

    @see Game::Create, SpriteParams
    */
    Sprite(const SpriteParams& params);

    /**
    Utility constructor for Sprites where their texture is already known and created elsewhere.

    @warning One should use `Game::Create` function to create Sprites instead of constructing the object directly.

    Used internally by the Jade Engine and most likely of limited use otherwise.
    */
    Sprite(const ObjectLayer layer, std::shared_ptr<Texture> texture, const int32_t z);

    void Render(SDL_Renderer* renderer) override;

    virtual void Tint(const SDL_Color& tintColor);

    virtual void SetAlpha(float alpha);
    virtual float GetAlpha() const;

    void SetRotation(const double angle);

    virtual const SDL_Rect& GetBoundingBox() const;
    SDL_Rect GetCollisionBox() const;
    virtual bool HitTest(uint32_t x, uint32_t y) const;
    virtual bool HasHitTest() const;

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

    const SDL_Rect& GetTransform() const { return _transform; }
    const std::string& GetTextureName() const;
    const std::shared_ptr<Texture>& GetTextureDescription() const { return _textureDescription; }
    void SetSampling(const TextureSampling sampling);
  protected:
    std::shared_ptr<Texture> _textureDescription;
    const SpriteSheetDescription* _spriteSheetDescription;

    SDL_Rect _transform;
    SDL_Rect _boundingBox;

    SDL_Texture* _texture;

    bool _spriteSheetMasked;
    SDL_Rect _spriteSheetMask;
    std::string _textureName;

    bool _rotated;

    float _alpha;
    double _rotationAngle;

    ObjectLayer _layer;
  };
}
