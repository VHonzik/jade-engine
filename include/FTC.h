#pragma once

#include "ITextObject.h"
#include "ObjectLayer.h"
#include "Text.h"

#include <SDL_ttf.h>
#include <vector>

namespace JadeEngine
{
  struct FTCParams
  {
    ObjectLayer   layer;
    TTF_Font*     font;
    std::string   fontName;
    uint32_t      fontSize;
    std::string   format;
    SDL_Color     defaultColor;
    int32_t       z;
  };

  class FTC : public ITextObject
  {
  public:
    FTC(const FTCParams& params);

    void Update() override;
    void Show(bool shown) override;
    void SetHorizontalAlign(const HorizontalAlignment align) override;
    void SetVerticalAlign(const VerticalAlignment align) override;

    void SetIntValue(const uint32_t index, const int32_t value);
    void SetStringValue(const uint32_t index, const std::string& value);
    void SetFloatValue(const uint32_t index, const float value);
    void SetValueColor(const uint32_t index, const SDL_Color& color);

    void SetPositon(int32_t x, int32_t y);
    int32_t GetX() const { return _x; }
    int32_t GetY() const { return _y; }

    int32_t GetCenterX() const { return _x + Width / 2; }
    int32_t GetCenterY() const { return _y + Height / 2; }

    int32_t GetWidth() const { return Width; }
    int32_t GetHeight() const { return Height; }

    int32_t Width;
    int32_t Height;
  private:
    void Rebuild();

    TTF_Font* _font;
    std::string _format;
    SDL_Color _defaultColor;

    TextParams _subParams;

    std::vector<SDL_Color> _valueColors;
    std::vector<size_t> _valueIndex;

    std::vector<Text*> _texts;

    HorizontalAlignment _align;

    int32_t _x;
    int32_t _y;

    ObjectLayer _layer;
  };
}
