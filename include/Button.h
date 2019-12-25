#pragma once

#include "ICompositeObject.h"
#include "ObjectLayer.h"

#include <SDL.h>
#include <string>

namespace JadeEngine
{
  class BoxSprite;
  class Text;

  struct ButtonParams
  {
    ObjectLayer   layer;

    std::string   textureNormal;
    std::string   texturePressed;
    std::string   textureDisabled;

    bool          spriteSheet;
    std::string   spriteSheetName;

    int32_t       width;
    int32_t       height;
    int32_t       z;
    int32_t       cornerSize;

    std::string   text;
    std::string   fontName;
    uint32_t      fontSize;
    SDL_Color     textColor;
    SDL_Color     disabledTextColor;

    std::string   clickSound;
  };

  class Button : public ICompositeObject
  {
  public:
    Button(const ButtonParams& params);

    void Disable(bool disabled);
    bool Disabled() const { return _disabled; };

    void Show(bool shown) override;

    void Update() override;

    void SetPosition(uint32_t x, uint32_t y);
    void SetCenterPosition(uint32_t x, uint32_t y);

    int32_t GetWidth() const;
    int32_t GetHeight() const;

    int32_t GetX() const;
    int32_t GetY() const;

    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    void SetText(const std::string& text);

    bool Pressed() { return _pressed; }
    bool Down() { return _down; }
    bool Released() { return _released; }

  private:
    void AdjustTextPosition();

    Text* _text;
    BoxSprite* _normalSprite;
    BoxSprite* _pressedSprite;
    BoxSprite* _disabledSprite;

    int32_t _pressedOffset;
    int32_t _disabledOffset;

    SDL_Color _normalTextColor;
    SDL_Color _disabledTextColor;

    std::string _clickSound;

    bool _disabled;

    bool _pressed;
    bool _released;
    bool _down;
  };
}