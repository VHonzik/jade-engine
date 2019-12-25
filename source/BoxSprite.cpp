#include "BoxSprite.h"

#include "Texture.h"
#include "Game.h"

namespace JadeEngine
{
  BoxSprite::BoxSprite(const BoxSpriteParams& params)
    : Sprite(params)
    , _cornerSize(params.cornerSize)
    , _boundingBox{0, 0, params.width , params.height}
    , _boundingBoxOverwritten(false)
  {
    _transform.w = params.width;
    _transform.h = params.height;
  }

  void BoxSprite::DoRender(SDL_Renderer* renderer)
  {
    if (_shown)
    {
      // TL corner
      auto destination = SDL_Rect{ _transform.x, _transform.y, _cornerSize, _cornerSize };
      auto maskedRect = _spriteSheetMasked ? _spriteSheetMask : SDL_Rect{ 0, 0, _textureDescription->width, _textureDescription->height };
      auto source = SDL_Rect{ maskedRect.x, maskedRect.y, _cornerSize, _cornerSize };
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // TR corner
      source.x = maskedRect.x + maskedRect.w - _cornerSize;
      destination.x = _transform.x + _transform.w - _cornerSize;
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // BR corner
      source.y = maskedRect.y + maskedRect.h - _cornerSize;
      destination.y = _transform.y + _transform.h - _cornerSize;
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // BL corner
      source.x = maskedRect.x;
      destination.x = _transform.x;
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // T line
      destination = { _transform.x + _cornerSize, _transform.y,
        _transform.w - 2 * _cornerSize, _cornerSize };
      source = SDL_Rect{ maskedRect.x + _cornerSize, maskedRect.y, maskedRect.w - 2 * _cornerSize, _cornerSize };
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // B line
      destination.y = _transform.y + _transform.h - _cornerSize;
      source.y = maskedRect.y + maskedRect.h - _cornerSize;
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // L line
      destination = { _transform.x, _transform.y + _cornerSize,
        _cornerSize, _transform.h - 2 * _cornerSize, };
      source = SDL_Rect{ maskedRect.x, maskedRect.y + _cornerSize, _cornerSize, maskedRect.h - 2 * _cornerSize };
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // R line
      destination.x = _transform.x + _transform.w - _cornerSize;
      source.x = maskedRect.x + maskedRect.w - _cornerSize;
      SDL_RenderCopy(renderer, _texture, &source, &destination);

      // Center
      destination = { _transform.x + _cornerSize, _transform.y + _cornerSize,
        _transform.w - 2 * _cornerSize, _transform.h - 2 * _cornerSize };
      source = SDL_Rect{ maskedRect.x + _cornerSize, maskedRect.y + _cornerSize,
        maskedRect.w - 2 * _cornerSize, maskedRect.h - 2 * _cornerSize };
      SDL_RenderCopy(renderer, _texture, &source, &destination);
    }
  }

  void BoxSprite::SetBoundingBox(const SDL_Rect& box)
  {
    _boundingBox = box;
    _boundingBoxOverwritten = true;
  }

  const SDL_Rect& BoxSprite::GetBoundingBox() const
  {
    return _boundingBox;
  }

  void BoxSprite::SetWidth(int32_t width)
  {
    Sprite::SetWidth(width);
    if (!_boundingBoxOverwritten)
    {
      _boundingBox.w = width;
    }
  }

  void BoxSprite::SetHeight(int32_t height)
  {
    Sprite::SetHeight(height);
    if (!_boundingBoxOverwritten)
    {
      _boundingBox.h = height;
    }
  }
}