#include "Text.h"

#include "Game.h"
#include "Transform.h"
#include "Utils.h"

#include <cassert>

namespace JadeEngine
{
  Text::Text(const TextParams& params)
    : _text(params.text)
    , _cachedTexture(nullptr)
    , _color(params.color)
    , _masked(false)
    , _mask{0, 0, 0, 0}
  {
    _z = params.z;
    SetLoadState(kLoadState_wanted);
    _font = GGame.FindFont(params.fontName, params.fontSize);

    transform->Initialize(kZeroVector2D_i32, kZeroVector2D_i32);

    assert(_font != nullptr);
  }

  LoadState Text::Load(SDL_Renderer* renderer)
  {
    if (_text.size() == 0)
    {
      transform->SetSize(0, 0);
      return kLoadState_done;
    }

    auto surface = TTF_RenderText_Blended(_font, _text.c_str(), _color);

    if (surface != nullptr)
    {
      if (_color.a < 255)
      {
        SDL_ASSERT_SUCCESS(SDL_SetSurfaceAlphaMod(surface, _color.a));
      }

      transform->SetSize(surface->w, surface->h);

      _cachedTexture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_ASSERT_SUCCESS(SDL_SetTextureBlendMode(_cachedTexture, SDL_BLENDMODE_BLEND));
      SDL_FreeSurface(surface);

      return _cachedTexture != nullptr ? kLoadState_done : kLoadState_abandoned;
    }
    else
    {
      transform->SetSize(0, 0);
      return kLoadState_abandoned;
    }
  }

  void Text::SetMask(const Rectangle& mask)
  {
    _masked = true;
    _mask = mask;
  }

  void Text::Render(SDL_Renderer* renderer)
  {
    if (_cachedTexture != nullptr)
    {
      SDL_Rect destination = transform->GetBox();

      if (_masked)
      {
        SDL_Rect interesection;
        if (SDL_IntersectRect(&destination, &_mask, &interesection) != SDL_FALSE)
        {
          SDL_Rect source = { interesection.x - destination.x, interesection.y - destination.y, interesection.w, interesection.h };
          SDL_RenderCopy(renderer, _cachedTexture, &source, &interesection);
        }
      }
      else
      {
        SDL_RenderCopy(renderer, _cachedTexture, nullptr, &destination);
      }
    }
  }

  void Text::SetTextFast(const std::string& text)
  {
    _text = text;
    RemoveCache();
  }

  void Text::SetText(const std::string& text)
  {
    if (_text != text)
    {
      SetTextFast(text);
    }
  }

  void Text::SetTextAndColor(const std::string& text, const SDL_Color& color)
  {
    _color = color;
    _text = text;
    RemoveCache();
  }

  void Text::SetColor(const SDL_Color& color)
  {
    _color = color;
    RemoveCache();
  }

  void Text::RemoveCache()
  {
    SDL_DestroyTexture(_cachedTexture);
    _cachedTexture = nullptr;
    SetLoadState(kLoadState_wanted);
  }

  void Text::Clean()
  {
    SDL_DestroyTexture(_cachedTexture);
  }
}