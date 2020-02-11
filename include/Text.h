#pragma once

#include "Aligment.h"
#include "EngineDataTypes.h"
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
    void SetMask(const Rectangle& mask);

    void Clean() override;

    const SDL_Color& GetColor() const { return _color; }
    TTF_Font* GetFont() const { return _font; }
    const std::string& GetText() const { return _text; }
  protected:
    std::string _text;

  private:
    void RemoveCache();

    bool _masked;
    Rectangle _mask;

    TTF_Font* _font;
    SDL_Texture* _cachedTexture;
    SDL_Color _color;
  };
}