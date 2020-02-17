#include "Button.h"

#include "Audio.h"
#include "BoxSprite.h"
#include "Game.h"
#include "Input.h"
#include "Text.h"
#include "Transform.h"

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
    transform->Initialize(kZeroVector2D_i32, { params.width, params.height });

    BoxSpriteParams boxParams;
    boxParams.layer = params.layer;
    boxParams.textureName = params.textureNormal;
    boxParams.z = params.z-1;
    boxParams.spriteSheet = params.spriteSheet;
    boxParams.spriteSheetName = params.spriteSheetName;
    boxParams.cornerSize = params.cornerSize;
    boxParams.size = {params.width, params.height};

    _normalSprite = GGame.Create<BoxSprite>(boxParams);
    transform->Attach(_normalSprite->transform);

    boxParams.textureName = params.texturePressed;
    _pressedSprite = GGame.Create<BoxSprite>(boxParams);
    _pressedSprite->Show(false);
    transform->Attach(_pressedSprite->transform);

    if (!params.textureDisabled.empty())
    {
      boxParams.textureName = params.textureDisabled;
      _disabledSprite = GGame.Create<BoxSprite>(boxParams);
      _disabledSprite->Show(false);
      transform->Attach(_disabledSprite->transform);
    }

    if (!params.textureHovered.empty())
    {
      boxParams.textureName = params.textureHovered;
      _hoveredSprite = GGame.Create<BoxSprite>(boxParams);
      _hoveredSprite->Show(false);
      transform->Attach(_hoveredSprite->transform);
    }

    _pressedOffset = params.pressedDepth;
    _disabledOffset = params.pressedDepth;

    _pressedSprite->transform->SetLocalPosition(0, _pressedOffset);
    _pressedSprite->transform->SetHeight(_pressedSprite->transform->GetHeight() - _pressedOffset);
    _pressedSprite->transform->SetBoundingBox({ 0, -_pressedOffset * 2, _pressedSprite->transform->GetWidth(), _pressedSprite->transform->GetHeight() + _pressedOffset * 2 });

    if (_disabledSprite != nullptr)
    {
      _disabledSprite->transform->SetLocalPosition(0, _disabledOffset);
      _disabledSprite->transform->SetHeight(_disabledSprite->transform->GetHeight() - _disabledOffset);
      _disabledSprite->transform->SetBoundingBox({ 0, -_disabledOffset * 2, _disabledSprite->transform->GetWidth(), _disabledSprite->transform->GetHeight() + _disabledOffset * 2 });
    }

    TextParams textParams;
    textParams.layer = params.layer;
    textParams.fontName = params.fontName;
    textParams.fontSize = params.fontSize;
    textParams.text = params.text;
    textParams.color = _normalTextColor;
    textParams.z = params.z;

    _text = GGame.Create<Text>(textParams);
    transform->Attach(_text->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    SetPosition(0, 0);
  }

  void Button::SetCenterPosition(uint32_t x, uint32_t y)
  {
    transform->SetCenterPosition(x, y);
    AdjustTextPosition();
  }

  void Button::SetPosition(uint32_t x, uint32_t y)
  {
    transform->SetPosition(x, y);
    AdjustTextPosition();
  }

  int32_t Button::GetWidth() const { return _normalSprite->transform->GetWidth(); }
  int32_t Button::GetHeight() const { return _normalSprite->transform->GetHeight(); }
  int32_t Button::GetX() const { return _normalSprite->transform->GetX(); }
  int32_t Button::GetY() const { return _normalSprite->transform->GetY(); }
  int32_t Button::GetCenterX() const { return _normalSprite->transform->GetCenterX(); }
  int32_t Button::GetCenterY() const { return _normalSprite->transform->GetCenterY(); }

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

    _text->transform->SetLocalPosition(0, offset);
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
    _hovered = hoveredSprite == _normalSprite || (_hoveredSprite != nullptr && hoveredSprite == _hoveredSprite);

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