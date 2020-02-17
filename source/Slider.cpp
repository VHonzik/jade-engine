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
    , _value (Clamp01(params.initialValue))
  {
    SpriteParams spriteParams;
    spriteParams.layer = params.layer;
    spriteParams.textureName = params.axisTexture;
    spriteParams.z = params.z - 2;
    spriteParams.spriteSheet = params.spriteSheet;
    spriteParams.spriteSheetName = params.spriteSheetName;

    _axis = GGame.Create<Sprite>(spriteParams);
    _axis->transform->SetWidth(params.width);
    transform->Attach(_axis->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    TextParams textParams;
    textParams.layer = params.layer;
    textParams.fontName = params.font;
    textParams.fontSize = params.fontSize;
    textParams.text = params.minTitle;
    textParams.color = params.fontColor;
    textParams.z = params.z;

    _minTitle = GGame.Create<Text>(textParams);
    _axis->transform->Attach(_minTitle->transform, { 0, _minMaxYMargin }, kAnchor_LeftCenter, kAnchor_Center);

    textParams.text = params.maxTitle;
    _maxTitle = GGame.Create<Text>(textParams);
    _axis->transform->Attach(_maxTitle->transform, { 0, _minMaxYMargin }, kAnchor_RightCenter, kAnchor_Center);

    spriteParams.textureName = params.axisEndTexture;
    spriteParams.z = params.z - 1;
    _axisLEnd = GGame.Create<Sprite>(spriteParams);
    _axis->transform->Attach(_axisLEnd->transform, kZeroVector2D_i32, kAnchor_LeftCenter, kAnchor_Center);
    _axisREnd = GGame.Create<Sprite>(spriteParams);
    _axis->transform->Attach(_axisREnd->transform, kZeroVector2D_i32, kAnchor_RightCenter, kAnchor_Center);

    spriteParams.textureName = params.pointerTexture;
    spriteParams.z = params.z;
    _pointer = GGame.Create<Sprite>(spriteParams);
    _axis->transform->Attach(_pointer->transform, { static_cast<int32_t>(_axis->transform->GetWidth() * _value), 0}, kAnchor_LeftCenter, kAnchor_Center);

    const auto width = _minTitle->transform->GetWidth() / 2 + _axis->transform->GetWidth() + _maxTitle->transform->GetWidth() / 2;
    const auto height = _minMaxYMargin + std::max(_minTitle->transform->GetHeight(), _maxTitle->transform->GetHeight()) / 2 + _axis->transform->GetHeight() / 2;
    transform->Initialize(kZeroVector2D_i32, { width, height });

    transform->SetPosition(0, 0);
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
      const auto pos = Clamp(GInput.GetMouseX() - _slidingOffset, _axis->transform->GetX(), _axis->transform->GetX() + _axis->transform->GetWidth());
      const auto prevValue = _value;

      _value = static_cast<float>(Clamp01(static_cast<float>(pos - _axis->transform->GetX()) / static_cast<float>(_axis->transform->GetWidth())));

      _valueChanged = prevValue != _value;

      _pointer->transform->SetLocalPosition({ static_cast<int32_t>(_axis->transform->GetWidth() * _value), 0 });
    }
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