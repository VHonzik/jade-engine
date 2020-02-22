#include "Checkbox.h"

#include "Game.h"
#include "Input.h"
#include "Sprite.h"
#include "Transform.h"

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
    spriteParams.z = params.z;
    spriteParams.spriteSheet = params.spriteSheet;
    spriteParams.spriteSheetName = params.spriteSheetName;

    _checkedSprite = GGame.Create<Sprite>(spriteParams);

    spriteParams.textureName = params.emptyTexture;
    _emptySprite = GGame.Create<Sprite>(spriteParams);

    Show(IsShown());
    transform->Initialize(kZeroVector2D_i32, _checkedSprite->transform->GetSize());
    transform->Attach(_checkedSprite->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);
    transform->Attach(_emptySprite->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);
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
}