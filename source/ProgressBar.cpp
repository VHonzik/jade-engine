#include "ProgressBar.h"

#include "Game.h"
#include "Sprite.h"
#include "Utils.h"
#include "EngineTime.h"

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
    spriteParams.texture = nullptr;
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

      foreground.middle->SetWidth(_foregroundWidth - foreground.left->GetWidth() - foreground.right->GetWidth());
      foreground.threshold = color.threshold;

      _foregrounds.push_back(foreground);
    }

    std::sort(std::begin(_foregrounds), std::end(_foregrounds),
      [](const Foreground& a, const Foreground& b)
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
      assert(foreground.left->GetWidth() == _backgroundLeft->GetWidth());
      assert(foreground.right->GetWidth() == _backgroundRight->GetWidth());
      assert(_backgroundWidth > foreground.left->GetWidth() + foreground.right->GetWidth());
    }

    _backgroundMiddle->SetWidth(_backgroundWidth - _backgroundLeft->GetWidth() - _backgroundRight->GetWidth());

    SetPosition(0, 0);
    Show(true);
  }

  void ProgressBar::SetCenterPosition(int32_t x, int32_t y)
  {
    _backgroundLeft->SetPosition(x - (_backgroundWidth / 2), 0);
    _backgroundLeft->SetCenterPosition(_backgroundLeft->GetCenterX(), y);
    _backgroundMiddle->SetPosition(_backgroundLeft->GetX() + _backgroundLeft->GetWidth(), _backgroundLeft->GetY());
    _backgroundRight->SetPosition(_backgroundMiddle->GetX() + _backgroundMiddle->GetWidth(),
      _backgroundMiddle->GetY());

    for (const auto& foreground : _foregrounds)
    {
      foreground.left->SetCenterPosition(_backgroundLeft->GetCenterX(), _backgroundLeft->GetCenterY());
      foreground.middle->SetPosition(foreground.left->GetX() + foreground.left->GetWidth(), foreground.left->GetY());
      foreground.right->SetPosition(foreground.middle->GetX() + foreground.middle->GetWidth(),
        foreground.middle->GetY());
    }
  }

  void ProgressBar::SetPosition(int32_t x, int32_t y)
  {
    _backgroundLeft->SetPosition(x, y);
    _backgroundMiddle->SetPosition(x + _backgroundLeft->GetWidth(), y);
    _backgroundRight->SetPosition(x + _backgroundWidth - _backgroundRight->GetWidth(), y);

    for (auto& foreground : _foregrounds)
    {
      foreground.left->SetPosition(x, y);
      foreground.middle->SetPosition(x + foreground.left->GetWidth(), y);
      foreground.right->SetPosition(foreground.middle->GetX() + foreground.middle->GetWidth(), y);
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
    return _backgroundLeft->GetHeight();
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
    _shown = shown;

    UpdateForegroundsVisibility();

    _backgroundLeft->Show(shown);
    _backgroundMiddle->Show(shown);
    _backgroundRight->Show(shown);
  }

  void ProgressBar::SetT(const float value)
  {
    _t = Clamp(value, 0.0f, 1.0f);
    _foregroundWidth = static_cast<int32_t>(_t * _backgroundWidth);
    const auto middleWidth = Clamp(_foregroundWidth - _backgroundLeft->GetWidth() - _backgroundRight->GetWidth(),
      0, _backgroundWidth);

    for (auto& foreground : _foregrounds)
    {
      foreground.middle->SetWidth(middleWidth);
    }

    SetPosition(_backgroundLeft->GetX(), _backgroundLeft->GetY());
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