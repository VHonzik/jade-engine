#pragma once

#include "EngineDataTypes.h"
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
    void Clean() override;

    /**
    Tint the sprite with a single color.

    @warning The first time Sprite is tinted or its transparency changed a deep copy of the texture is created which is potentially expensive operation.
    @param tintColor RGBA 0-255 color that will be used to multiply each pixel of the sprite. Alpha component is ignored.
    @see Game::CopyTexture, Sprite::SetAlpha
    */
    virtual void Tint(const SDL_Color& tintColor);

    /**
    Changed the transparency of the sprite.

    @warning The first time Sprite is tinted or its transparency changed a deep copy of the texture is created which is potentially expensive operation.
    @param alpha Transparency alpha value to set. 0 will make the sprite completely invisible, 1 fully opaque. It will be clamped to [0.0f, 1.0f] range.
    @see Game::CopyTexture, Sprite::Tint
    */
    virtual void SetAlpha(float alpha);

    /**
    Return the current transparency alpha value.
    */
    virtual float GetAlpha() const;

    /**
    Set the rotation the sprite will rendered with.

    The rotation center will be the center of the sprite, i.e. GetWidth() / 2 and GetHeight() / 2. This only affects the rendering of the sprite and not its transform.

    @param angle Rotational angle in degrees in a clockwise direction.
    */
    void SetRotation(const double angle);

    /**
    Return the bounding box of the sprite. A bounding box position elements (`x` and `y`) are always zero. The `w` and `h` elements are the width and height of the sprite respectively.
    */
    virtual const Rectangle& GetBoundingBox() const;

    /**
    Return the collision box of the sprite. The `x` and `y` elements are the current sprite's position. The `w` and `h` elements are the width and height of the sprite respectively.
    */
    Rectangle GetCollisionBox() const;

    /**
    Perform a hit test, returning whether a pixel of the sprite is not empty (not completely transparent, alpha > 0).

    @pre The texture used by the sprite was initialized with hit map. You can use HasHitTest() to confirm so.
    @see GameInitParamsTextureEntry::generateHitMap, Sprite::HasHitTest
    @returns True if the pixel is opaque, false if completely transparent.
    */
    virtual bool HitTest(const int32_t x, const int32_t y) const;

    /**
    Return whether the sprite's texture was initialized with hit map.

    @see GameInitParamsTextureEntry::generateHitMap, Sprite::HitTest
    */
    virtual bool HasHitTest() const;

    /**
    Change the sampling of the texture.

    @warning Changing sampling requires a deep copy of the texture which is potentially expensive operation.

    @see TextureSampling, GameInitParamsTextureEntry::sampling, GameInitParamsSpriteSheetEntry::sampling
    */
    void SetSampling(const TextureSampling sampling);

    /**
    Change the sprite's texture to a different texture that is part of the same sprite-sheet.

    Useful for simple frame-based animations of sprites. Nothing happens if the new texture is not found.

    @pre The sprite was initialized with sprite-sheet.
    @param sprite Name of the new texture as defined by the sprite-sheet. Usually original file name of the texture before packing into sprite-sheet.
    @see SpriteParams::spriteSheet
    */
    void SetSpriteSheetSprite(const std::string& sprite);

    /**
    Change the what rectangle of the sprite-sheet is used for rendering the sprite.

    As sprite-sheets are collections of texture it is recommended to use Sprite::SetSpriteSheetSprite instead of directly specifying the rectangle.

    @pre The sprite was initialized with sprite-sheet.
    @see SpriteParams::spriteSheet
    */
    void SetSpriteSheetMask(const Rectangle& mask);

    /**
    Return the texture name that the sprite is rendering. In the case of sprite-sheet sprite it is the texture name of one of the packed textures.
    @see SpriteParams::spriteSheet
    */
    const std::string& GetTextureName() const;

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

    const Rectangle& GetTransform() const { return _transform; }

  protected:
    std::shared_ptr<Texture> _textureDescription;
    const SpriteSheetDescription* _spriteSheetDescription;

    Rectangle _transform;
    Rectangle _boundingBox;

    SDL_Texture* _texture;

    bool _spriteSheetMasked;
    Rectangle _spriteSheetMask;
    std::string _textureName;

    bool _rotated;

    float _alpha;
    double _rotationAngle;

    ObjectLayer _layer;
  };
}
