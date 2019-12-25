#include "Button.h"

#include "Game.h"
#include "Input.h"
#include "Audio.h"
#include "Text.h"

#include "BoxSprite.h"

namespace JadeEngine
{
  Button::Button(const ButtonParams& params)
    : _pressed(false)
    , _down(false)
    , _released(false)
    , _disabled(false)
    , _normalTextColor(params.textColor)
    , _disabledTextColor(params.disabledTextColor)
    , _clickSound(params.clickSound)
  {
    TextParams textParams;
    textParams.layer = params.layer;
    textParams.font = nullptr;
    textParams.fontName = params.fontName;
    textParams.fontSize = params.fontSize;
    textParams.text = params.text;
    textParams.color = _normalTextColor;
    textParams.z = params.z;

    _text = GGame.Create<Text>(textParams);
    _text->SetHorizontalAlign(kHorizontalAlignment_Center);
    _text->SetVerticalAlign(kVerticalAlignment_Center);

    BoxSpriteParams boxParams;
    boxParams.layer = params.layer;
    boxParams.textureName = params.textureNormal;
    boxParams.z = params.z-1;
    boxParams.spriteSheet = params.spriteSheet;
    boxParams.spriteSheetName = params.spriteSheetName;
    boxParams.cornerSize = params.cornerSize;
    boxParams.width = params.width;
    boxParams.height = params.height;

    _normalSprite = GGame.Create<BoxSprite>(boxParams);

    boxParams.textureName = params.textureDisabled;
    _disabledSprite = GGame.Create<BoxSprite>(boxParams);
    _disabledSprite->Show(false);

    boxParams.textureName = params.texturePressed;
    _pressedSprite = GGame.Create<BoxSprite>(boxParams);
    _pressedSprite->Show(false);

    _pressedOffset = _normalSprite->GetTextureDescription()->height
      - _pressedSprite->GetTextureDescription()->height;

    _disabledOffset = _normalSprite->GetTextureDescription()->height
      - _disabledSprite->GetTextureDescription()->height;

    _pressedSprite->SetHeight(_pressedSprite->GetHeight() - _pressedOffset);
    _disabledSprite->SetHeight(_disabledSprite->GetHeight() - _disabledOffset);

    _pressedSprite->SetBoundingBox({ 0, -_pressedOffset * 2,
      _pressedSprite->GetWidth(), _pressedSprite->GetHeight() + _pressedOffset * 2 });
    _disabledSprite->SetBoundingBox({ 0, -_disabledOffset * 2,
      _disabledSprite->GetWidth(), _disabledSprite->GetHeight() + _disabledOffset * 2 });

    SetPosition(0, 0);
  }

  void Button::SetPosition(uint32_t x, uint32_t y)
  {
    _normalSprite->SetPosition(x, y);
    _pressedSprite->SetPosition(x, y + _pressedOffset);
    _disabledSprite->SetPosition(x, y + _disabledOffset);
    AdjustTextPosition();
  }

  int32_t Button::GetWidth() const { return _normalSprite->GetWidth(); }
  int32_t Button::GetHeight() const { return _normalSprite->GetHeight(); }
  int32_t Button::GetX() const { return _normalSprite->GetX(); }
  int32_t Button::GetY() const { return _normalSprite->GetY(); }
  int32_t Button::GetCenterX() const { return _normalSprite->GetCenterX(); }
  int32_t Button::GetCenterY() const { return _normalSprite->GetCenterY(); }

  void Button::AdjustTextPosition()
  {
    int offset = 0;
    if (_down)
    {
      offset = _pressedOffset;
    }
    else if (_disabled)
    {
      offset = _disabledOffset;
    }
    _text->SetPosition(_normalSprite->GetCenterX(), _normalSprite->GetCenterY() + offset);
  }

  void Button::SetCenterPosition(uint32_t x, uint32_t y)
  {
    _normalSprite->SetCenterPosition(x, y);
    _pressedSprite->SetCenterPosition(x, y + _pressedOffset);
    _disabledSprite->SetCenterPosition(x, y + _disabledOffset);
    AdjustTextPosition();
  }

  void Button::Update()
  {
    if (_pressed)
    {
      _pressed = false;
    }

    if (_released)
    {
      _released = false;
    }
    const auto hoveredSprite = GGame.GetHoveredSprite();
    if (!_disabled && !_down && hoveredSprite == _normalSprite
      && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
    {
      _down = true;
      _pressed = true;
      AdjustTextPosition();
      if (!_clickSound.empty())
      {
        GAudio.PlaySound(_clickSound.c_str());
      }
    }
    else if (!_disabled && !_pressed && _down && hoveredSprite != _pressedSprite)
    {
      _down = false;
      _pressed = false;
      AdjustTextPosition();
    }
    else if (!_disabled && _down && !GInput.MouseButtonDown(SDL_BUTTON_LEFT))
    {
      _down = false;
      _pressed = false;
      _released = true;
      AdjustTextPosition();
    }

    _pressedSprite->Show(_shown && _down && !_disabled);
    _normalSprite->Show(_shown && !_down && !_disabled);
    _disabledSprite->Show(_shown && _disabled);
  }

  void Button::Disable(bool disabled)
  {
    if (disabled != _disabled)
    {
      _pressedSprite->Show(disabled);
      _normalSprite->Show(!disabled);
      _disabledSprite->Show(disabled);

      _text->SetColor(disabled ? _disabledTextColor : _normalTextColor);

      _disabled = disabled;
      AdjustTextPosition();
    }
  }

  void Button::SetText(const std::string& text)
  {
    _text->SetText(text);
  }

  void Button::Show(bool shown)
  {
    _shown = shown;
    _text->Show(shown);
    _normalSprite->Show(shown);
    _pressedSprite->Show(shown);
    _disabledSprite->Show(shown);
  }
}