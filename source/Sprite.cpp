#include "Sprite.h"

#include "Camera.h"
#include "EngineConstants.h"
#include "Game.h"
#include "Texture.h"
#include "Utils.h"

#include <cassert>
#include <SDL.h>

namespace JadeEngine
{
  const std::string kScalingSDLHintNames[3] =
  {
    "nearest",
    "linear",
    "best",
  };

  Sprite::Sprite(const SpriteParams& params)
    : _alpha(1.0f)
    , _layer(params.layer)
    , _rotated(false)
    , _rotationAngle(0)
    , _spriteSheetDescription(nullptr)
  {
    _z = params.z;
    if (params.spriteSheet)
    {
      _spriteSheetDescription = GGame.GetSpriteSheetDescription(params.spriteSheetName);
      if (_spriteSheetDescription != nullptr && _spriteSheetDescription->sprites.count(params.textureName) > 0)
      {
        _spriteSheetMasked = true;
        _textureDescription = GGame.FindTexture(_spriteSheetDescription->textureName);
        SetSpriteSheetMask(_spriteSheetDescription->sprites.find(params.textureName)->second.rect);
      }
      else
      {
        _textureDescription = GGame.FindTexture(kDefaultTextureName);
      }
    }
    else
    {
      _textureDescription = GGame.FindTexture(params.textureName);
    }

    assert(_textureDescription);

    _texture = _textureDescription->texture;
    _textureName = _textureDescription->name;

    if (_spriteSheetMasked)
    {
      _textureName = params.textureName;
      transform->Initialize(0, 0, _spriteSheetMask.w, _spriteSheetMask.h);
    }
    else
    {
      transform->Initialize(0, 0, _textureDescription->width, _textureDescription->height);
      transform->SetBoundingBox(_textureDescription->boundingBox);
    }
  }

  Sprite::Sprite(const ObjectLayer layer, std::shared_ptr<Texture> texture, const int32_t z)
    : _alpha(1.0f)
    , _layer(layer)
    , _rotated(false)
    , _rotationAngle(0)
    , _spriteSheetDescription(nullptr)
    , _texture(texture->texture)
    , _textureDescription(texture)
    , _textureName(texture->name)
  {
    transform->Initialize(0, 0, texture->width, texture->height);
    transform->SetBoundingBox(texture->boundingBox);
    _z = z;
    SetLoadState(kLoadState_Done);
    assert(_textureDescription);
  }

  void Sprite::Render(SDL_Renderer* renderer)
  {
    SDL_Rect destination = _layer == kObjectLayer_World ? GWorldCamera.WorldToScreen(transform) : transform->GetBox();
    auto maskCopy = _spriteSheetMask;
    SDL_Rect* source = _spriteSheetMasked ? &maskCopy : nullptr;

    if (_rotated)
    {
      SDL_RenderCopyEx(renderer, _texture, source, &destination, _rotationAngle, nullptr, SDL_FLIP_NONE);
    }
    else
    {
      SDL_RenderCopy(renderer, _texture, source, &destination);
    }
  }

  void Sprite::Tint(const SDL_Color& tintColor)
  {
    MakeTextureUnique();
    SDL_SetTextureColorMod(_texture, tintColor.r, tintColor.g, tintColor.b);
  }

  bool Sprite::HasHitTest() const
  {
    return _textureDescription->hitArray.size() > 0;
  }

  bool Sprite::HitTest(const int32_t x, const int32_t y) const
  {
    assert(HasHitTest());
    return _textureDescription->hitArray[x + y * transform->GetWidth()];
  }

  const std::string& Sprite::GetTextureName() const
  {
    return _textureName;
  }

  void Sprite::SetSpriteSheetSprite(const std::string& sprite)
  {
    assert(_spriteSheetMasked);

    const auto spriteFound = _spriteSheetDescription->sprites.find(sprite);
    if (spriteFound == std::end(_spriteSheetDescription->sprites)) return;

    SetSpriteSheetMask(spriteFound->second.rect);
  }

  void Sprite::SetSpriteSheetMask(const Rectangle& mask)
  {
    assert(_spriteSheetMasked);
    _spriteSheetMask = mask;
    transform->SetBoundingBox({ 0, 0, _spriteSheetMask.w, _spriteSheetMask.h });
  }

  void Sprite::MakeTextureUnique()
  {
    if (!_textureDescription->isCopy)
    {
      _textureDescription = GGame.CopyTexture(_textureDescription, _textureDescription->sampling);
      _texture = _textureDescription->texture;
    }
  }

  void Sprite::SetAlpha(const float alpha)
  {
    if (_alpha != Clamp01(alpha))
    {
      MakeTextureUnique();

      _alpha = Clamp01(alpha);
      auto aplhaMod = static_cast<uint8_t>(_alpha * 255.0f);
      SDL_SetTextureAlphaMod(_texture, aplhaMod);
    }
  }

  float Sprite::GetAlpha() const
  {
    return _alpha;
  }

  void Sprite::SetRotation(const double angle)
  {
    _rotated = true;
    _rotationAngle = angle;
  }

  void Sprite::SetSampling(const TextureSampling sampling)
  {
    if (_textureDescription->sampling != sampling)
    {
      _textureDescription = GGame.CopyTexture(_textureDescription, sampling);
      _texture = _textureDescription->texture;
    }
  }

  void Sprite::Clean()
  {
    if (_textureDescription->isCopy)
    {
      GGame.DestroyCopyTexture(_textureDescription->texture);
    }
  }
}