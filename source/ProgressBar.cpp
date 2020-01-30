#include "ProgressBar.h"

#include "EngineTime.h"
#include "Game.h"
#include "Sprite.h"
#include "Transform.h"
#include "Utils.h"

#include <cassert>

namespace JadeEngine
{
  ProgressBar::ProgressBar(const ProgressBarParams& params)
    : _backgroundWidth(params.width)
    , _foregroundWidth(_backgroundWidth)
    , _t(1.0f)
    , _maxValue(100)
    , _currentValue(_maxValue)
    , _displayedValue(_maxValue)
    , _valueUpdateDuration(params.fullBarUpdateDuration)
    , _valueUpdateSpeed(_maxValue / _valueUpdateDuration)
  {
    assert(params.colors.size() >= 1);

    SpriteParams spriteParams;
    spriteParams.layer = params.layer;
    spriteParams.spriteSheet = params.spriteSheet;
    spriteParams.spriteSheetName = params.spriteSheetName;

    for (const auto& color : params.colors)
    {
      Foreground foreground;
      spriteParams.textureName = color.textureLeft;
      spriteParams.z = params.z;
      foreground.left = GGame.Create<Sprite>(spriteParams);
      spriteParams.textureName = color.textureMiddle;
      spriteParams.z = params.z - 1;
      foreground.middle = GGame.Create<Sprite>(spriteParams);
      spriteParams.textureName = color.textureRight;
      spriteParams.z = params.z;
      foreground.right = GGame.Create<Sprite>(spriteParams);

      foreground.middle->transform->SetWidth(_foregroundWidth - foreground.left->transform->GetWidth() - foreground.right->transform->GetWidth());
      foreground.threshold = color.threshold;

      _foregrounds.push_back(foreground);
    }

    std::sort(std::begin(_foregrounds), std::end(_foregrounds), [](const Foreground& a, const Foreground& b)
    {
      return a.threshold < b.threshold;
    });

    UpdateForegroundsVisibility();

    spriteParams.z = params.z - 2;
    spriteParams.textureName = params.backgroundTextureLeft;
    _backgroundLeft = GGame.Create<Sprite>(spriteParams);

    spriteParams.z = params.z - 3;
    spriteParams.textureName = params.backgroundTextureMiddle;
    _backgroundMiddle = GGame.Create<Sprite>(spriteParams);

    spriteParams.z = params.z - 2;
    spriteParams.textureName = params.backgroundTextureRight;
    _backgroundRight = GGame.Create<Sprite>(spriteParams);

    for (const auto& foreground : _foregrounds)
    {
      assert(foreground.left->transform->GetWidth() == _backgroundLeft->transform->GetWidth());
      assert(foreground.right->transform->GetWidth() == _backgroundRight->transform->GetWidth());
      assert(_backgroundWidth > foreground.left->transform->GetWidth() + foreground.right->transform->GetWidth());
    }

    _backgroundMiddle->transform->SetWidth(_backgroundWidth - _backgroundLeft->transform->GetWidth() - _backgroundRight->transform->GetWidth());

    SetPosition(0, 0);
    Show(true);
  }

  void ProgressBar::SetCenterPosition(int32_t x, int32_t y)
  {
    _backgroundLeft->transform->SetPosition(x - (_backgroundWidth / 2), 0);
    _backgroundLeft->transform->SetCenterPosition(_backgroundLeft->transform->GetCenterX(), y);
    _backgroundMiddle->transform->SetPosition(_backgroundLeft->transform->GetX() + _backgroundLeft->transform->GetWidth(), _backgroundLeft->transform->GetY());
    _backgroundRight->transform->SetPosition(_backgroundMiddle->transform->GetX() + _backgroundMiddle->transform->GetWidth(), _backgroundMiddle->transform->GetY());

    for (const auto& foreground : _foregrounds)
    {
      foreground.left->transform->SetCenterPosition(_backgroundLeft->transform->GetCenterX(), _backgroundLeft->transform->GetCenterY());
      foreground.middle->transform->SetPosition(foreground.left->transform->GetX() + foreground.left->transform->GetWidth(), foreground.left->transform->GetY());
      foreground.right->transform->SetPosition(foreground.middle->transform->GetX() + foreground.middle->transform->GetWidth(), foreground.middle->transform->GetY());
    }
  }

  void ProgressBar::SetPosition(int32_t x, int32_t y)
  {
    _backgroundLeft->transform->SetPosition(x, y);
    _backgroundMiddle->transform->SetPosition(x + _backgroundLeft->transform->GetWidth(), y);
    _backgroundRight->transform->SetPosition(x + _backgroundWidth - _backgroundRight->transform->GetWidth(), y);

    for (auto& foreground : _foregrounds)
    {
      foreground.left->transform->SetPosition(x, y);
      foreground.middle->transform->SetPosition(x + foreground.left->transform->GetWidth(), y);
      foreground.right->transform->SetPosition(foreground.middle->transform->GetX() + foreground.middle->transform->GetWidth(), y);
    }
  }

  void ProgressBar::Update()
  {
    if (_valueUpdateDuration > 0.0f && _displayedValue != _currentValue)
    {
        _displayedValue = MoveTowards(_displayedValue, _currentValue,
          _valueUpdateSpeed * GTime.deltaTime);
        SetT(_displayedValue / _maxValue);
    }
  }
  int32_t ProgressBar::GetHeight() const
  {
    return _backgroundLeft->transform->GetHeight();
  }

  int32_t ProgressBar::GetWidth() const
  {
    return _backgroundWidth;
  }

  void ProgressBar::UpdateForegroundsVisibility()
  {
    bool found = false;
    for (int32_t i = 0; i < _foregrounds.size(); ++i)
    {
      auto& foreground = _foregrounds[i];
      auto visible = false;
      if (!found && (_t < foreground.threshold || i == _foregrounds.size()-1))
      {
        found = true;
        visible = true;
      }

      foreground.left->Show(_shown && _t > 0.0f && visible);
      foreground.middle->Show(_shown && _t > 0.0f && visible);
      foreground.right->Show(_shown && _t > 0.0f && visible);
    }
  }

  void ProgressBar::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    UpdateForegroundsVisibility();

    _backgroundLeft->Show(isShown);
    _backgroundMiddle->Show(isShown);
    _backgroundRight->Show(isShown);
  }

  void ProgressBar::SetT(const float value)
  {
    _t = Clamp(value, 0.0f, 1.0f);
    _foregroundWidth = static_cast<int32_t>(_t * _backgroundWidth);
    const auto middleWidth = Clamp(_foregroundWidth - _backgroundLeft->transform->GetWidth() - _backgroundRight->transform->GetWidth(), 0, _backgroundWidth);

    for (auto& foreground : _foregrounds)
    {
      foreground.middle->transform->SetWidth(middleWidth);
    }

    SetPosition(_backgroundLeft->transform->GetX(), _backgroundLeft->transform->GetY());
    UpdateForegroundsVisibility();
  }

  void ProgressBar::InitValue(float maxValue, float initValue)
  {
    assert(maxValue > 0.0f);
    _maxValue = maxValue;
    _currentValue = std::min(initValue, maxValue);
    _displayedValue = _currentValue;
    if (_valueUpdateDuration > 0.0f)
    {
      _valueUpdateSpeed = _maxValue / _valueUpdateDuration;
    }
    SetT(_currentValue / _maxValue);
  }

  void ProgressBar::ChangeValue(float amount)
  {
    _currentValue = Clamp(_currentValue + amount, 0.0f, _maxValue);
    if (_valueUpdateDuration <= 0.0f)
    {
      _displayedValue = _currentValue;
    }
  }
}