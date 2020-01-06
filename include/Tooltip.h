#pragma once

#include "ICompositeObject.h"
#include "ObjectLayer.h"

#include <SDL_ttf.h>
#include <string>

namespace JadeEngine
{
  class BoxSprite;
  class TextBox;

  struct TooltipParams
  {
    ObjectLayer   layer;
    std::string   text;
    std::string   fontName;
    SDL_Color     textColor;
    uint32_t      fontSize;
    int32_t       cornerSize;
    std::string   textureName;
    uint32_t      width;
    uint32_t      padding;
    int32_t       z;
  };

  class Tooltip : public ICompositeObject
  {
  public:
    Tooltip(const TooltipParams& params);

    void Update() override;

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    void Show(const bool shown) override;

    int32_t GetX() const;
    int32_t GetY() const;

    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    int32_t GetWidth() const;
    int32_t GetHeight() const;

  private:
    BoxSprite* _boxSprite;
    TextBox* _textBox;
    bool _textPreloaded;
    uint32_t _padding;
  };
}
