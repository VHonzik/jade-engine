#pragma once

#include "Sprite.h"

#include <string>

typedef struct _TTF_Font TTF_Font;

namespace JadeEngine
{
  struct TextSpriteParams
  {
    ObjectLayer   layer;
    std::string   textureName;
    int32_t       z;
    bool          spriteSheet;
    std::string   spriteSheetName;
    std::string   fontName;
    uint32_t      fontSize;
    std::string   text;
  };

  class TextBox;

  class TextSprite : public Sprite
  {
  public:
    TextSprite(const TextSpriteParams& params);
    LoadState Load(SDL_Renderer* renderer) override;
    void Render(SDL_Renderer* renderer) override;
  private:
    std::string _text;
    TTF_Font* _font;
    SDL_Texture* _cachedTextTexture;
    SDL_Texture* _finalTexture;
    Rectangle _textDimensions;
  };
}
