#include "Tooltip.h"

#include "BoxSprite.h"
#include "Game.h"
#include "TextBox.h"
#include "Transform.h"

namespace JadeEngine
{
  Tooltip::Tooltip(const TooltipParams& params)
    : _textPreloaded(false)
    , _padding(params.padding)
  {
    TextBoxParams textParams;
    textParams.layer = params.layer;
    textParams.fontSize = params.fontSize;
    textParams.fontName = params.fontName;
    textParams.text = params.text;
    textParams.width = params.width;
    textParams.color = params.textColor;
    textParams.z = params.z;

    _textBox = GGame.Create<TextBox>(textParams);

    BoxSpriteParams boxParams;
    boxParams.layer = params.layer;
    boxParams.textureName = params.textureName;
    boxParams.z = params.z - 1;
    boxParams.spriteSheet = false;
    boxParams.spriteSheetName = "";
    boxParams.cornerSize = params.cornerSize;
    boxParams.size = { static_cast<int32_t>(params.width + _padding * 2), static_cast<int32_t>(50 + _padding * 2) };

    _boxSprite = GGame.Create<BoxSprite>(boxParams);

    SetPosition(0, 0);
    Show(false);
  }

  void Tooltip::Update()
  {
    if (!_textPreloaded && _textBox->GetLoadState() == kLoadState_Done)
    {
      _textPreloaded = true;
      const auto previousHeight = _boxSprite->transform->GetHeight();
      _boxSprite->transform->SetHeight(_textBox->GetHeight() + _padding * 2);
      SetPosition(GetX(), GetY() - (GetHeight() - previousHeight));
    }
  }

  void Tooltip::SetPosition(int32_t x, int32_t y)
  {
    _boxSprite->transform->SetPosition(x, y);
    _textBox->SetPosition(x + _padding, y + _padding);
  }

  void Tooltip::SetCenterPosition(int32_t x, int32_t y)
  {
    _boxSprite->transform->SetCenterPosition(x, y);
    SetPosition(_boxSprite->transform->GetX(), _boxSprite->transform->GetY());
  }

  void Tooltip::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _boxSprite->Show(isShown);
    _textBox->Show(isShown);
  }

  int32_t Tooltip::GetX() const { return _boxSprite->transform->GetX(); }
  int32_t Tooltip::GetY() const { return _boxSprite->transform->GetY(); }
  int32_t Tooltip::GetCenterX() const { return _boxSprite->transform->GetCenterX(); }
  int32_t Tooltip::GetCenterY() const { return _boxSprite->transform->GetCenterY(); }
  int32_t Tooltip::GetWidth() const { return _boxSprite->transform->GetWidth(); }
  int32_t Tooltip::GetHeight() const { return _boxSprite->transform->GetHeight(); }
}