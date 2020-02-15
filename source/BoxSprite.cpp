#include "BoxSprite.h"

#include "Game.h"
#include "Texture.h"
#include "Transform.h"

namespace JadeEngine
{
  BoxSprite::BoxSprite(const BoxSpriteParams& params)
    : Sprite(params)
    , _cornerSize(params.cornerSize)
    , _scaleX(1)
    , _scaleY(1)
  {
    transform->SetSize(params.size);
  }

  void BoxSprite::Render(SDL_Renderer* renderer)
  {
    // TL corner
    auto destination = SDL_Rect{ transform->GetX(), transform->GetY(), _cornerSize * _scaleX, _cornerSize * _scaleY };
    auto maskedRect = _spriteSheetMasked ? _spriteSheetMask : SDL_Rect{ 0, 0, _textureDescription->width, _textureDescription->height };
    auto source = SDL_Rect{ maskedRect.x, maskedRect.y, _cornerSize, _cornerSize };
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // TR corner
    source.x = maskedRect.x + maskedRect.w - _cornerSize;
    destination.x = transform->GetX() + transform->GetWidth() - _cornerSize * _scaleX;
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // BR corner
    source.y = maskedRect.y + maskedRect.h - _cornerSize;
    destination.y = transform->GetY() + transform->GetHeight() - _cornerSize * _scaleX;
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // BL corner
    source.x = maskedRect.x;
    destination.x = transform->GetX();
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // T line
    destination = { transform->GetX() + _cornerSize * _scaleX, transform->GetY(),
      transform->GetWidth() - 2 * _cornerSize * _scaleX, _cornerSize * _scaleY };
    source = SDL_Rect{ maskedRect.x + _cornerSize, maskedRect.y, maskedRect.w - 2 * _cornerSize, _cornerSize };
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // B line
    destination.y = transform->GetY() + transform->GetHeight() - _cornerSize * _scaleY;
    source.y = maskedRect.y + maskedRect.h - _cornerSize;
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // L line
    destination = { transform->GetX(), transform->GetY() + _cornerSize * _scaleY,
      _cornerSize * _scaleX, transform->GetHeight() - 2 * _cornerSize * _scaleY };
    source = SDL_Rect{ maskedRect.x, maskedRect.y + _cornerSize, _cornerSize, maskedRect.h - 2 * _cornerSize };
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // R line
    destination.x = transform->GetX() + transform->GetWidth() - _cornerSize * _scaleX;
    source.x = maskedRect.x + maskedRect.w - _cornerSize;
    SDL_RenderCopy(renderer, _texture, &source, &destination);

    // Center
    destination = { transform->GetX() + _cornerSize * _scaleX, transform->GetY() + _cornerSize * _scaleY,
      transform->GetWidth() - 2 * _cornerSize * _scaleX, transform->GetHeight() - 2 * _cornerSize * _scaleY };
    source = SDL_Rect{ maskedRect.x + _cornerSize, maskedRect.y + _cornerSize,
      maskedRect.w - 2 * _cornerSize, maskedRect.h - 2 * _cornerSize };
    SDL_RenderCopy(renderer, _texture, &source, &destination);
  }

  void BoxSprite::SetScale(int32_t x, int32_t y)
  {
    _scaleX = x;
    _scaleY = y;
  }
}