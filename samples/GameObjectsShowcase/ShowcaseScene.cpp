#include "ShowcaseScene.h"

#include "BoxSprite.h"
#include "Button.h"
#include "Checkbox.h"
#include "EngineTemplateParams.h"
#include "Game.h"
#include "LineBox.h"
#include "Text.h"
#include "TransformGroup.h"

namespace GameObjectsShowcase
{
  void ShowcaseScene::Start()
  {
    ButtonParams buttonParams = kMainMenuBlueButton;
    buttonParams.text = "Button";
    _button = GGame.Create<Button>(buttonParams);

    TextParams textParams = kVera1650Grey;
    textParams.color = kWhiteColor;
    textParams.fontSize = 32;
    textParams.text = "Text";
    _text = GGame.Create<Text>(textParams);

    BoxSpriteParams boxSpriteParams = kOptionsGreyPanel;
    boxSpriteParams.size = { 200, 100 };
    _boxSprite = GGame.Create<BoxSprite>(boxSpriteParams);

    textParams.fontSize = 32;
    textParams.text = "BoxSprite";
    textParams.color = kBlackColor;
    _boxSpriteTitle = GGame.Create<Text>(textParams);
    _boxSprite->transform->Attach(_boxSpriteTitle->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    CheckboxParams checkboxParams = kBlueCheckbox;
    _checkbox = GGame.Create<Checkbox>(checkboxParams);

    textParams.text = "Checkbox";
    textParams.color = kWhiteColor;
    _checkboxTitle = GGame.Create<Text>(textParams);
    _checkbox->transform->Attach(_checkboxTitle->transform, {10, 0}, kAnchor_RightCenter, kAnchor_LeftCenter);

    TransformGroupParams transformGroupParams;
    transformGroupParams.layer = kObjectLayer_UI;
    transformGroupParams.direction = kGroupDirection_Vertical;
    transformGroupParams.spacing = 50;
    transformGroupParams.alignment = kHorizontalAlignment_Center;

    _leftColumnGroup = GGame.Create<TransformGroup>(transformGroupParams);
    _leftColumnGroup->Add({ _button, _text, _boxSprite, _checkbox });
    _leftColumnGroup->transform->SetPosition(100, 80);

    LineBoxParams lineBoxParams;
    lineBoxParams.layer = kObjectLayer_UI;
    lineBoxParams.size = _leftColumnGroup->transform->GetSize() + Vector2D_i32{ 150, 50 };
    lineBoxParams.color = kWhiteColor;
    lineBoxParams.z = -10;

    _leftColumnBox = GGame.Create<LineBox>(lineBoxParams);
    _leftColumnGroup->transform->Attach(_leftColumnBox->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    textParams.fontSize = 16;
    textParams.text = "TransformGroup";
    textParams.color = kWhiteColor;
    _leftColumnTitle = GGame.Create<Text>(textParams);
    _leftColumnBox->transform->Attach(_leftColumnTitle->transform, { 0, -5 }, kAnchor_LeftTop, kAnchor_LeftBottom);


   }

  void ShowcaseScene::Update()
  {

  }
}