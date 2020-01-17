#pragma once

#include "Aligment.h"
#include "IGameObject.h"
#include "ObjectLayer.h"

#include <SDL_ttf.h>
#include <string>

namespace JadeEngine
{
  struct TextParams
  {
    ObjectLayer   layer;
    std::string   fontName;
    uint32_t      fontSize;
    std::string   text;
    SDL_Color     color;
    int32_t       z;
  };

  class Text : public IGameObject
  {
  public:
    Text(const TextParams& params);

    LoadState Load(SDL_Renderer* renderer) override;
    void Render(SDL_Renderer* renderer) override;

    void SetText(const std::string& text);
    void SetTextAndColor(const std::string& text, const SDL_Color& color);
    void SetColor(const SDL_Color& color);
    void SetMask(const SDL_Rect& mask);

    void SetHorizontalAlign(const HorizontalAlignment align) { _textHorizontalAlign = align; }
    HorizontalAlignment GetHorizontalAlign() const { return _textHorizontalAlign; }

    void SetVerticalAlign(const VerticalAlignment align) { _textVerticalAlign = align; }
    VerticalAlignment GetVerticalAlign() const { return _textVerticalAlign; }

    void Clean() override;

    TTF_Font* GetFont() const { return _font; }

    const std::string& GetText() const { return _text; }

    int32_t GetWidth() const { return _width; };
    int32_t GetHeight() const { return _height; };

    SDL_Color GetColor() const { return _color; }

    void SetPosition(int32_t x, int32_t y);
    int32_t GetX() const { return _x; }
    int32_t GetY() const { return _y; }

  protected:
    std::string _text;

  private:
    void RemoveCache();

    bool _masked;
    SDL_Rect _mask;

    int32_t _width;
    int32_t _height;

    TTF_Font* _font;
    SDL_Texture* _cachedTexture;
    SDL_Color _color;

    HorizontalAlignment _textHorizontalAlign;
    VerticalAlignment _textVerticalAlign;

    int32_t _x;
    int32_t _y;
  };
}