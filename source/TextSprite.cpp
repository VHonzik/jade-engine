#include "TextSprite.h"

#include "Camera.h"
#include "EngineConstants.h"
#include "Game.h"
#include "Transform.h"

#include <cassert>
#include <sstream>

namespace JadeEngine
{
  TextSprite::TextSprite(const TextSpriteParams& params)
    : Sprite({
        //ObjectLayer               layer;
        params.layer,
        //std::string               textureName;
        params.textureName,
        //int32_t                   z;
        params.z,
        //bool                      spriteSheet;
        params.spriteSheet,
        //std::string               spriteSheetName;
        params.spriteSheetName
      })
    , _cachedTextTexture(nullptr)
  {
    SetLoadState(kLoadState_wanted);

    _font = GGame.FindFont(params.fontName, params.fontSize);
    assert(_font != nullptr);

    int32_t textWidth;
    int32_t textHeight;
    TTF_SizeText(_font, params.text.c_str(), &textWidth, &textHeight);
    textHeight = TTF_FontLineSkip(_font);

    const auto repeatPerLine = _textureDescription->width / textWidth;
    const auto repeatLines = _textureDescription->height / textHeight;

    std::stringstream ss;
    for (int32_t l = 0; l < repeatLines; l++)
    {
      for (int32_t pr = 0; pr < repeatPerLine; pr++)
      {
        ss << params.text;
      }
      ss << " ";
    }

    _text = ss.str();
  }

  LoadState TextSprite::Load(SDL_Renderer* renderer)
  {
    if (_text.size() == 0)
    {
      return kLoadState_done;
    }

    auto surface = TTF_RenderText_Blended_Wrapped(_font, _text.c_str(), kWhiteColor, transform->GetWidth());

    if (surface != nullptr)
    {
      _cachedTextTexture = SDL_CreateTextureFromSurface(renderer, surface);

      _textDimensions = { 0, 0, surface->w, surface->h };

      SDL_FreeSurface(surface);

      if (_cachedTextTexture != nullptr)
      {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

        const auto wantedWidth = _textureDescription->width;
        const auto wantedHeight = _textureDescription->height;

        // Create texture with the size of the image that will contain the final result and made it blend when rendering
        _finalTexture = SDL_CreateTexture(renderer, GGame.GetNativeTextureFormats(), SDL_TEXTUREACCESS_TARGET, wantedWidth, wantedHeight);
        SDL_SetTextureBlendMode(_finalTexture, SDL_BLENDMODE_BLEND);

        // We will use Render-To-Texture
        SDL_SetRenderTarget(renderer, _finalTexture);

        // Fill it with completely transparent pixels
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // Render the text texture on it => the final texture is know white text on transparent background
        SDL_SetTextureBlendMode(_cachedTextTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, _cachedTextTexture, nullptr, &_textDimensions);

        // Special sauce the achieve the desired effect of text masked by the image but with color from the image
        // src = image texture, dst = final texture currently with white text rendered
        // dstRGB = srcRGB * [1,1,1] + dstRGB * [0,0,0] (throwaway existing text color information and use image colors)
        // dstA = srcA * dstA + dstA * srcA (render only pixels that have non-zero alpha in both src and dst)
        const auto customBlendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
        SDL_SetTextureBlendMode(_texture, customBlendMode);

        // Render image texture to the Render-To-Texture target with the above special sauce blend mode
        SDL_RenderCopy(renderer, _texture, nullptr, nullptr);

        SDL_SetRenderTarget(renderer, nullptr);

        return kLoadState_done;
      }
      else
      {
        return kLoadState_abandoned;
      }
    }
    else
    {
      return kLoadState_abandoned;
    }
  }

  void TextSprite::Render(SDL_Renderer* renderer)
  {
    SDL_Rect destination = _layer == kObjectLayer_world ? GWorldCamera.WorldToScreen(transform) : transform->GetBox();

    SDL_Rect* source = _spriteSheetMasked ? &_spriteSheetMask : nullptr;
    if (_rotated)
    {
      SDL_RenderCopyEx(renderer, _finalTexture, source, &destination, _rotationAngle, nullptr, SDL_FLIP_NONE);
    }
    else
    {
      SDL_RenderCopy(renderer, _finalTexture, source, &destination);
    }
  }
}