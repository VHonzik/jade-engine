#include "Checkbox.h"
#include "Sprite.h"
#include "Game.h"
#include "Input.h"

#include <algorithm>

namespace JadeEngine
{
  Checkbox::Checkbox(const CheckboxParams& params)
    : _checked(params.checked)
    , _checkedChanged(false)
  {
    SpriteParams spriteParams;
    spriteParams.layer = params.layer;
    spriteParams.textureName = params.checkedTexture;
    spriteParams.texture = nullptr;
    spriteParams.z = params.z;
    spriteParams.spriteSheet = params.spriteSheet;
    spriteParams.spriteSheetName = params.spriteSheetName;

    _checkedSprite = GGame.Create<Sprite>(spriteParams);

    spriteParams.textureName = params.emptyTexture;
    _emptySprite = GGame.Create<Sprite>(spriteParams);
    _checkedSprite->Show(_checked);
    _emptySprite->Show(!_checked);
    SetPosition(0, 0);
  }

  void Checkbox::Update()
  {
    _checkedChanged = false;

    if (_shown)
    {
      const auto hoverSprite = GGame.GetHoveredSprite();
      if ((hoverSprite == _checkedSprite || hoverSprite == _emptySprite) &&
        GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
      {
        _checked = !_checked;
        _checkedChanged = true;

        _checkedSprite->Show(_checked);
        _emptySprite->Show(!_checked);
      }
    }
  }

  void Checkbox::Show(bool shown)
  {
    _shown = shown;
    _checkedSprite->Show(shown && _checked);
    _emptySprite->Show(shown && !_checked);
  }

  int32_t Checkbox::GetX() const
  {
    return _checkedSprite->GetX();
  }

  int32_t Checkbox::GetY() const
  {
    return _checkedSprite->GetCenterY();
  }

  int32_t Checkbox::GetCenterX() const
  {
    return _checkedSprite->GetCenterX();
  }

  int32_t Checkbox::GetCenterY() const
  {
    return _checkedSprite->GetCenterY();
  }

  int32_t Checkbox::GetWidth() const
  {
    return _checkedSprite->GetWidth();
  }

  int32_t Checkbox::GetHeight() const
  {
    return _checkedSprite->GetHeight();
  }

  void Checkbox::SetPosition(int32_t x, int32_t y)
  {
    _checkedSprite->SetPosition(x, y);
    _emptySprite->SetCenterPosition(_checkedSprite->GetCenterX(),
      _checkedSprite->GetCenterY());
  }

  void Checkbox::SetCenterPosition(int32_t x, int32_t y)
  {
    _checkedSprite->SetCenterPosition(x, y);
    _emptySprite->SetCenterPosition(_checkedSprite->GetCenterX(),
      _checkedSprite->GetCenterY());
  }
}