#include "GameScene.h"

#include "Game.h"
#include "EngineConstants.h"
#include "ObjectLayer.h"
#include "Text.h"

namespace HelloJadeEngine
{
  void GameScene::Start()
  {
    TextParams params;
    params.layer = kObjectLayer_ui;
    params.font = nullptr;
    params.fontName = kVeraFont;
    params.fontSize = 64;
    params.text = "Hello Jade Engine!";
    params.color = kWhiteColor;
    params.z = 0;

    _helloJadeEngineText = GGame.Create<Text>(params);
    _helloJadeEngineText->SetHorizontalAlign(kHorizontalAlignment_Center);
    _helloJadeEngineText->SetVerticalAlign(kVerticalAlignment_Center);
    _helloJadeEngineText->SetPosition(GGame.GetHalfWidth(), GGame.GetHalfHeight());
  }
}