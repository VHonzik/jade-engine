#include "ShowcaseScene.h"

#include "Button.h"
#include "EngineTemplateParams.h"
#include "Game.h"

namespace GameObjectsShowcase
{
  void ShowcaseScene::Start()
  {
    ButtonParams buttonParams = kMainMenuBlueButton;
    buttonParams.text = "Button";
    _button = GGame.Create<Button>(buttonParams);
    _button->SetPosition(50, 50);

  }

  void ShowcaseScene::Update()
  {

  }
}