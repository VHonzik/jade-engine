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
  Sprite::Sprite(const SpriteParams& params)
    : _preloaded(false)
    , _shown(true)
    , _z(params.z)
    , _alpha(1.0f)
    , _spriteSheetDescription(nullptr)
    , _layer(params.layer)
    , _rotated(false)
    , _rotationAngle(0)
  {
    if (params.texture)
    {
      _textureDescription = params.texture;
    }
    else if (params.spriteSheet)
    {
      _spriteSheetDescription = GGame.GetSpriteSheetDescription(params.spriteSheetName);
      if (_spriteSheetDescription != nullptr && _spriteSheetDescription->sprites.count(params.textureName) > 0)
      {
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
      _transform = { 0, 0, _spriteSheetMask.w, _spriteSheetMask.h };
      _boundingBox = { 0, 0, _spriteSheetMask.w, _spriteSheetMask.h };
    }
    else
    {
      _transform = { 0, 0, _textureDescription->width, _textureDescription->height };
      _boundingBox = _textureDescription->boundingBox;
    }
  }

  void Sprite::Preload(SDL_Renderer* renderer)
  {
    _preloaded = DoPreload(renderer);
  }

  void Sprite::Render(SDL_Renderer* renderer)
  {
    if (_shown)
    {
      DoRender(renderer);
    }
  }

  void Sprite::DoRender(SDL_Renderer* renderer)
  {
    SDL_Rect destination = _layer == kObjectLayer_world ? GWorldCamera.WorldToScreen(_transform) : _transform;
    SDL_Rect* source = _spriteSheetMasked ? &_spriteSheetMask : nullptr;
    if (_rotated)
    {
      SDL_RenderCopyEx(renderer, _texture, source, &destination, _rotationAngle, nullptr, SDL_FLIP_NONE);
    }
    else
    {
      SDL_RenderCopy(renderer, _texture, source, &destination);
    }
  }

  void Sprite::SetPosition(uint32_t x, uint32_t y)
  {
    _transform.x = x;
    _transform.y = y;
  }

  void Sprite::SetCenterPosition(uint32_t x, uint32_t y)
  {
    _transform.x = x - _transform.w / 2;
    _transform.y = y - _transform.h / 2;
  }

  void Sprite::SetWidth(int32_t width)
  {
    if (_transform.w != width)
    {
      _transform.w = width;
      _boundingBox = { 0, 0, _transform.w, _transform.h };
    }
  }

  void Sprite::SetHeight(int32_t height)
  {
    if (_transform.h != height)
    {
      _transform.h = height;
      _boundingBox = { 0, 0, _transform.w, _transform.h };
    }
  }

  void Sprite::SetWidthHeight(int32_t width, int32_t height)
  {
    SetWidth(width);
    SetHeight(height);
  }

  void Sprite::Tint(const SDL_Color& tintColor)
  {
    if (!_textureDescription->isCopy)
    {
      _textureDescription = GGame.CopyTexture(_textureDescription);
      _texture = _textureDescription->texture;
    }

    SDL_SetTextureColorMod(_texture, tintColor.r, tintColor.g, tintColor.b);
  }

  void Sprite::Clean()
  {
  }

  bool Sprite::HasHitTest() const
  {
    return _textureDescription->hitArray.size() > 0;
  }

  bool Sprite::HitTest(uint32_t x, uint32_t y) const
  {
    assert(HasHitTest());
    return _textureDescription->hitArray[x + y * _transform.w];
  }

  const SDL_Rect& Sprite::GetBoundingBox() const
  {
    return _boundingBox;
  }

  SDL_Rect Sprite::GetCollisionBox() const
  {
    return { GetX(), GetY(), _boundingBox.w, _boundingBox.h };
  }

  const std::string& Sprite::GetTextureName() const
  {
    return _textureName;
  }

  void Sprite::SetSpriteSheetSprite(const std::string& sprite)
  {
    if (!_spriteSheetMasked) return;
    const auto spriteFound = _spriteSheetDescription->sprites.find(sprite);
    if (spriteFound == std::end(_spriteSheetDescription->sprites)) return;

    SetSpriteSheetMask(spriteFound->second.rect);
  }

  void Sprite::SetSpriteSheetMask(const SDL_Rect& mask)
  {
    _spriteSheetMasked = true;
    _spriteSheetMask = mask;
    _boundingBox = { 0, 0, _spriteSheetMask.w, _spriteSheetMask.h };
  }

  void Sprite::SetAlpha(float alpha)
  {
    if (_alpha != Clamp01(alpha))
    {
      if (!_textureDescription->isCopy)
      {
        _textureDescription = GGame.CopyTexture(_textureDescription);
        _texture = _textureDescription->texture;
      }

      _alpha = Clamp01(alpha);
      auto aplhaMod = static_cast<uint8_t>(_alpha * 255.0f);
      SDL_SetTextureAlphaMod(_texture, aplhaMod);
    }
  }

  float Sprite::GetAlpha() const
  {
    return _alpha;
  }

  void Sprite::Show(bool shown)
  {
    _shown = shown;
  }

  void Sprite::SetRotation(const double angle)
  {
    _rotated = true;
    _rotationAngle = angle;
  }
}