#include "PoweredByJadeEngineScene.h"

#include "EngineConstants.h"
#include "EngineTime.h"
#include "Game.h"
#include "Text.h"
#include "TextSprite.h"
#include "Transform.h"
#include "Utils.h"

namespace JadeEngine
{
  const auto kAppearDuration = 1.5f;
  const auto kStayDurection = 2.0f;

  PoweredByJadeEngineScene::PoweredByJadeEngineScene()
    : _text(nullptr)
    , _nextSceneId(kScene_MainMenu)
    , _nextSceneTimer(kAppearDuration + kStayDurection)
  {
  }

  void PoweredByJadeEngineScene::SetNextScene(const int32_t nextScene)
  {
    _nextSceneId = nextScene;
  }

  void PoweredByJadeEngineScene::Start()
  {
    TextParams textParams;
    textParams.layer = kObjectLayer_ui;
    textParams.fontName = kVeraFontBold;
    textParams.fontSize = 32;
    textParams.text = "Powered by Jade Engine";
    textParams.color = kJadeGreenColor;
    textParams.color.a = 0;
    textParams.z = 0;

    _text = GGame.Create<Text>(textParams);
    _text->SetHorizontalAlign(kHorizontalAlignment_Center);
    _text->SetVerticalAlign(kVerticalAlignment_Bottom);
    _text->SetPosition(GGame.GetHalfWidth(), GGame.GetHeight() - 50);

    TextSpriteParams textSpriteParams;
    textSpriteParams.layer = kObjectLayer_ui;
    textSpriteParams.textureName = kJadeEngineLogoTexture;
    textSpriteParams.z = 0;
    textSpriteParams.spriteSheet = false;
    textSpriteParams.spriteSheetName = "";
    textSpriteParams.fontName = kVeraFont;
    textSpriteParams.fontSize = 16;
    textSpriteParams.text = "JadeEngine";

    _logo = GGame.Create<TextSprite>(textSpriteParams);
    _logo->transform->SetCenterPosition(GGame.GetHalfWidth(), GGame.GetHalfHeight());
  }

  void PoweredByJadeEngineScene::Update()
  {
    auto color = _text->GetColor();
    if (color.a < 255)
    {
      color.a = static_cast<uint8_t>(Clamp(color.a + (255.0f / kAppearDuration) * GTime.deltaTime, 0.0f, 255.0f));
      // Setting color invalidates the text texture and doing so every frame in Update would mean it's never active hence PreUpdate
      _text->SetColor(color);
    }

    _nextSceneTimer -= GTime.deltaTime;
    if (_nextSceneTimer < 0.0f)
    {
      GGame.PlayScene(_nextSceneId);
    }
  }
}