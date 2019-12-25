#include "Tooltip.h"

#include "BoxSprite.h"
#include "Game.h"
#include "TextBox.h"

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
    textParams.font = nullptr;
    textParams.text = params.text;
    textParams.width = params.width;
    textParams.color = params.textColor;
    textParams.z = params.z;

    _textBox = GGame.Create<TextBox>(textParams);

    BoxSpriteParams boxParams;
    boxParams.layer = params.layer;
    boxParams.textureName = params.textureName;
    boxParams.texture = nullptr;
    boxParams.z = params.z - 1;
    boxParams.spriteSheet = false;
    boxParams.spriteSheetName = "";
    boxParams.cornerSize = params.cornerSize;
    boxParams.width = params.width + _padding * 2;
    boxParams.height = 50 + _padding * 2;

    _boxSprite = GGame.Create<BoxSprite>(boxParams);

    SetPosition(0, 0);
    Show(false);
  }

  void Tooltip::Update()
  {
    if (!_textPreloaded && !_textBox->RequiresPreload())
    {
      _textPreloaded = true;
      const auto previousHeight = _boxSprite->GetHeight();
      _boxSprite->SetHeight(_textBox->GetHeight() + _padding * 2);
      SetPosition(GetX(), GetY() - (GetHeight() - previousHeight));
    }
  }

  void Tooltip::SetPosition(int32_t x, int32_t y)
  {
    _boxSprite->SetPosition(x, y);
    _textBox->SetPosition(x + _padding, y + _padding);
  }

  void Tooltip::SetCenterPosition(int32_t x, int32_t y)
  {
    _boxSprite->SetCenterPosition(x, y);
    SetPosition(_boxSprite->GetX(), _boxSprite->GetY());
  }

  void Tooltip::Show(bool shown)
  {
    ICompositeObject::Show(shown);

    _boxSprite->Show(IsShown());
    _textBox->Show(IsShown());
  }

  int32_t Tooltip::GetX() const { return _boxSprite->GetX(); }
  int32_t Tooltip::GetY() const { return _boxSprite->GetY(); }
  int32_t Tooltip::GetCenterX() const { return _boxSprite->GetCenterX(); }
  int32_t Tooltip::GetCenterY() const { return _boxSprite->GetCenterY(); }
  int32_t Tooltip::GetWidth() const { return _boxSprite->GetWidth(); }
  int32_t Tooltip::GetHeight() const { return _boxSprite->GetHeight(); }
}