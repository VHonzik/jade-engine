#include "SampleGameScene.h"

#include "Game.h"
#include "EngineConstants.h"
#include "ObjectLayer.h"
#include "Text.h"

namespace HelloJadeEngine
{
  void GameScene::Start()
  {
    TextParams params;
    params.layer = kObjectLayer_UI;
    params.fontName = kVeraFont;
    params.fontSize = 64;
    params.text = "Hello Jade Engine!";
    params.color = kWhiteColor;
    params.z = 0;

    _helloJadeEngineText = GGame.Create<Text>(params);
    _helloJadeEngineText->transform->SetCenterPosition(GGame.GetHalfWidth(), GGame.GetHalfHeight());
  }
}