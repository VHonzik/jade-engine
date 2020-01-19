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
    , _hovered(false)
    , _normalTextColor(params.textColor)
    , _disabledTextColor(params.disabledTextColor)
    , _clickSound(params.clickSound)
    , _disabledSprite(nullptr)
    , _hoveredSprite(nullptr)
  {
    TextParams textParams;
    textParams.layer = params.layer;
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

    boxParams.textureName = params.texturePressed;
    _pressedSprite = GGame.Create<BoxSprite>(boxParams);
    _pressedSprite->Show(false);

    if (!params.textureDisabled.empty())
    {
      boxParams.textureName = params.textureDisabled;
      _disabledSprite = GGame.Create<BoxSprite>(boxParams);
      _disabledSprite->Show(false);
    }

    if (!params.textureHovered.empty())
    {
      boxParams.textureName = params.textureHovered;
      _hoveredSprite = GGame.Create<BoxSprite>(boxParams);
      _hoveredSprite->Show(false);
    }

    _pressedOffset = params.pressedDepth;
    _disabledOffset = params.pressedDepth;

    _pressedSprite->SetHeight(_pressedSprite->GetHeight() - _pressedOffset);
    _pressedSprite->SetBoundingBox({ 0, -_pressedOffset * 2, _pressedSprite->GetWidth(), _pressedSprite->GetHeight() + _pressedOffset * 2 });

    if (_disabledSprite != nullptr)
    {
      _disabledSprite->SetHeight(_disabledSprite->GetHeight() - _disabledOffset);
      _disabledSprite->SetBoundingBox({ 0, -_disabledOffset * 2, _disabledSprite->GetWidth(), _disabledSprite->GetHeight() + _disabledOffset * 2 });
    }

    SetPosition(0, 0);
  }

  void Button::SetPosition(uint32_t x, uint32_t y)
  {
    _normalSprite->SetPosition(x, y);
    _pressedSprite->SetPosition(x, y + _pressedOffset);
    if (_disabledSprite != nullptr)
    {
      _disabledSprite->SetPosition(x, y + _disabledOffset);
    }
    if (_hoveredSprite != nullptr)
    {
      _hoveredSprite->SetPosition(x, y);
    }
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

    if (_disabledSprite != nullptr)
    {
      _disabledSprite->SetCenterPosition(x, y + _disabledOffset);
    }

    if (_hoveredSprite != nullptr)
    {
      _hoveredSprite->SetCenterPosition(x, y);
    }

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
    _hovered = hoveredSprite == _normalSprite || hoveredSprite == _hoveredSprite;

    if (!_disabled && !_down && _hovered && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
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

    Show(IsShown());
  }

  void Button::Disable(bool disabled)
  {
    if (disabled != _disabled && _disabledSprite != nullptr)
    {
      _text->SetColor(disabled ? _disabledTextColor : _normalTextColor);

      _disabled = disabled;
      Show(IsShown());
      AdjustTextPosition();
    }
  }

  void Button::SetText(const std::string& text)
  {
    _text->SetText(text);
  }

  void Button::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _text->Show(isShown);
    _normalSprite->Show(isShown && !_down && !_disabled);
    _pressedSprite->Show(isShown && _down && !_disabled);

    if (_disabledSprite != nullptr)
    {
      _disabledSprite->Show(isShown && _disabled);
    }

    if (_hoveredSprite != nullptr)
    {
      _hoveredSprite->Show(isShown && _hovered && !_down && !_disabled);
    }
  }
}