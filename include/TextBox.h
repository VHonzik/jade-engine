#pragma once

#include "EngineStyles.h"
#include "ITextObject.h"

#include <SDL_ttf.h>

namespace JadeEngine
{
  struct TextBoxParams
  {
    ObjectLayer   layer;
    uint32_t      fontSize;
    std::string   fontName;
    TTF_Font*     font;
    std::string   text;
    uint32_t      width;
    SDL_Color     color;
    int32_t       z;
  };

  class TextBox : public ITextObject
  {
  public:
    TextBox(const TextBoxParams& params);

    bool DoPreload(SDL_Renderer* renderer) override;
    void DoRender(SDL_Renderer* renderer) override;

    void Clean() override;

    void SetText(const std::string& text);

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    int32_t GetX() const;
    int32_t GetY() const;

    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    int32_t GetWidth() const;
    int32_t GetHeight() const;

  private:
    void RemoveCache();

    TTF_Font* _font;
    SDL_Color _color;

    std::string _text;

    int32_t _x;
    int32_t _y;
    int32_t _width;
    int32_t _height;

    SDL_Texture* _cachedTexture;
  };
}
