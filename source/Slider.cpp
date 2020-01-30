#include "Slider.h"

#include "Game.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"
#include "Utils.h"

namespace JadeEngine
{
  Slider::Slider(const SliderParams& params)
    : _sliding(false)
    , _width(params.width)
    , _minMaxYMargin(params.minMaxYMargin)
    , _slidingOffset(0)
    , _slidingReleased(false)
  {
    TextParams textParams;
    textParams.layer = params.layer;
    textParams.fontName = params.font;
    textParams.fontSize = params.fontSize;
    textParams.text = params.minTitle;
    textParams.color = params.fontColor;
    textParams.z = params.z;

    _minTitle = GGame.Create<Text>(textParams);
    _minTitle->SetVerticalAlign(kVerticalAlignment_Top);
    _minTitle->SetHorizontalAlign(kHorizontalAlignment_Center);

    textParams.text = params.maxTitle;
    _maxTitle = GGame.Create<Text>(textParams);
    _maxTitle->SetVerticalAlign(kVerticalAlignment_Top);
    _maxTitle->SetHorizontalAlign(kHorizontalAlignment_Center);

    SpriteParams spriteParams;
    spriteParams.layer = params.layer;
    spriteParams.textureName = params.axisTexture;
    spriteParams.z = params.z;
    spriteParams.spriteSheet = params.spriteSheet;
    spriteParams.spriteSheetName = params.spriteSheetName;

    _axis = GGame.Create<Sprite>(spriteParams);
    _axis->transform->SetWidth(params.width);

    spriteParams.textureName = params.axisEndTexture;
    spriteParams.z = params.z + 1;
    _axisLEnd = GGame.Create<Sprite>(spriteParams);
    _axisREnd = GGame.Create<Sprite>(spriteParams);

    spriteParams.textureName = params.pointerTexture;
    spriteParams.z = params.z + 2;
    _pointer = GGame.Create<Sprite>(spriteParams);

    _value = Clamp01(params.initialValue);

    SetPosition(0, 0);
  }

  void Slider::Update()
  {
    if (_valueChanged)
    {
      _valueChanged = false;
    }

    if (_slidingReleased)
    {
      _slidingReleased = false;
    }

    if (!_sliding && GGame.GetHoveredSprite() == _pointer && GInput.MouseButtonDown(SDL_BUTTON_LEFT))
    {
      _sliding = true;
      _slidingOffset = GInput.GetMouseX() - _pointer->transform->GetCenterX();
    }
    else if (_sliding && !GInput.MouseButtonDown(SDL_BUTTON_LEFT))
    {
      _sliding = false;
      _slidingReleased = true;
      _slidingOffset = 0;
    }

    if (_sliding)
    {
      const auto pos = Clamp(GInput.GetMouseX() - _slidingOffset, _axisLEnd->transform->GetCenterX(), _axisREnd->transform->GetCenterX());
      const auto prevValue = _value;

      _value = static_cast<float>(Clamp01(static_cast<float>(pos - _axisLEnd->transform->GetCenterX()) / static_cast<float>(_axisREnd->transform->GetCenterX() - _axisLEnd->transform->GetCenterX())));

      _valueChanged = prevValue != _value;

      _pointer->transform->SetCenterPosition(pos, _pointer->transform->GetCenterY());
    }
  }

  int32_t Slider::GetX() const
  {
    return _axis->transform->GetX();
  }

  int32_t Slider::GetY() const
  {
    return _axis->transform->GetCenterY();
  }

  int32_t Slider::GetCenterX() const
  {
    return _axis->transform->GetCenterX();
  }

  int32_t Slider::GetCenterY() const
  {
    return _axis->transform->GetCenterY();
  }

  int32_t Slider::GetWidth() const
  {
    return (_maxTitle->GetX() + _maxTitle->GetWidth()) - _minTitle->GetX();
  }

  int32_t Slider::GetHeight() const
  {
    return (_maxTitle->GetY() + _maxTitle->GetHeight()) - _pointer->transform->GetY();
  }

  void Slider::SetPosition(uint32_t x, uint32_t y)
  {
    _axis->transform->SetPosition(x, y);
    _axisLEnd->transform->SetCenterPosition(x, _axis->transform->GetCenterY());
    _axisREnd->transform->SetCenterPosition(x + _width, _axis->transform->GetCenterY());
    _pointer->transform->SetCenterPosition(x + static_cast<int32_t>(_value * _width), _axis->transform->GetCenterY());
    _minTitle->SetPosition(x, y + _minMaxYMargin);
    _maxTitle->SetPosition(x + _width, y + _minMaxYMargin);
  }

  void Slider::SetCenterPosition(uint32_t x, uint32_t y)
  {
    _pointer->transform->SetCenterPosition(x + static_cast<int32_t>((_value - 0.5f) * _width), y);
    _axis->transform->SetCenterPosition(x, y);
    _axisLEnd->transform->SetCenterPosition(x - _width / 2, y);
    _axisREnd->transform->SetCenterPosition(x + _width / 2, y);
    _minTitle->SetPosition(x - _width / 2, y + _minMaxYMargin);
    _maxTitle->SetPosition(x + _width / 2, y + _minMaxYMargin);
  }

  void Slider::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _pointer->Show(isShown);
    _axis->Show(isShown);
    _axisLEnd->Show(isShown);
    _axisREnd->Show(isShown);

    _minTitle->Show(isShown);
    _maxTitle->Show(isShown);
  }
}