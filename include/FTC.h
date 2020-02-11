#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "Text.h"

#include <SDL_ttf.h>
#include <vector>

namespace JadeEngine
{
  struct FTCParams
  {
    ObjectLayer   layer;
    std::string   fontName;
    uint32_t      fontSize;
    std::string   format;
    SDL_Color     defaultColor;
    int32_t       z;
  };

  class FTC : public IGameObject
  {
  public:
    FTC(const FTCParams& params);

    void Show(const bool shown) override;

    void SetIntValue(const uint32_t index, const int32_t value);
    void SetStringValue(const uint32_t index, const std::string& value);
    void SetFloatValue(const uint32_t index, const float value);
    void SetValueColor(const uint32_t index, const SDL_Color& color);

  private:
    void Rebuild();
    void Recalculate();

    TTF_Font* _font;
    std::string _format;
    SDL_Color _defaultColor;

    TextParams _subParams;

    std::vector<SDL_Color> _valueColors;
    std::vector<size_t> _valueIndex;

    std::vector<Text*> _texts;

    ObjectLayer _layer;
  };
}
