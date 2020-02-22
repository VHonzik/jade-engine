#include "MainMenuScene.h"

#include "Audio.h"
#include "BoxSprite.h"
#include "Button.h"
#include "EngineConstants.h"
#include "EngineTemplateParams.h"
#include "Game.h"
#include "Persistence.h"
#include "TransformGroup.h"

#include <sstream>

namespace JadeEngine
{
  MainMenuScene::MainMenuScene()
    : _playSceneId(0)
    , _playButton(nullptr)
    , _optionsButton(nullptr)
    , _quitButton(nullptr)
    , _playSceneSet(false)
  {
  }

  void MainMenuScene::ApplyDisplayModeFromSettings()
  {
    auto width = GPersistence.GetSettingTyped<Setting::ResolutionWidth>();
    auto height = GPersistence.GetSettingTyped<Setting::ResolutionHeight>();

    const auto& modes = GGame.GetDisplayModes();
    for (int32_t i = 0; i < modes.size(); i++)
    {
      const auto& mode = modes[i];
      if (mode.width == width && mode.height == height)
      {
        GGame.SetDisplayMode(i);
        break;
      }
    }

    auto fullscreen = GPersistence.GetSettingTyped<Setting::FullScreen>();
    GGame.SetFullscreen(fullscreen);
  }

  void MainMenuScene::Start()
  {
    ApplyDisplayModeFromSettings();

    std::stringstream stream;
    stream << "Copyright " << GGame.GetAuthor() << ", " << GGame.GetCopyrightYear() <<
      ", build " << GGame.GetMajorVersion() << kBuildSeperator << GGame.GetMinorVersion() <<
      kBuildSeperator << GGame.GetHashVersion();

    auto copyrightParams = kVera16LightGrey;
    copyrightParams.text = stream.str();

    auto copyright = GGame.Create<Text>(copyrightParams);
    copyright->transform->SetPositionAnchor(GGame.GetWidth() - 5, GGame.GetHeight() - 5, kAnchor_RightBottom);

    auto buttonParams = kMainMenuBlueButton;

    buttonParams.text = "PLAY";
    buttonParams.height = 70;
    _playButton = GGame.Create<Button>(buttonParams);

    buttonParams.text = "OPTIONS";
    buttonParams.height = 50;
    _optionsButton = GGame.Create<Button>(buttonParams);

    buttonParams.text = "EXIT";
    buttonParams.height = 50;
    _quitButton = GGame.Create<Button>(buttonParams);

    TransformGroupParams groupParams;
    groupParams.layer = kObjectLayer_UI;
    groupParams.spacing = 30;
    groupParams.direction = kGroupDirection_Vertical;
    groupParams.alignment = kVerticalAlignment_Center;

    _buttonGroup = GGame.Create<TransformGroup>(groupParams);
    _buttonGroup->Add({ _playButton, _optionsButton, _quitButton });
    _buttonGroup->transform->SetCenterPosition(GGame.GetMiddlePoint());

    GGame.SetCursor("cursorGauntlet");
  }

  void MainMenuScene::Update()
  {
    if (_quitButton->Released())
    {
      GGame.End();
    }

    if (_optionsButton->Released())
    {
      GGame.PlayScene(kScene_OptionsMenu);
    }

    if (_playSceneSet && _playButton->Released())
    {
      GGame.PlayScene(_playSceneId);
    }
  }

  void MainMenuScene::SetPlayButtonScene(const int32_t playSceneId)
  {
    _playSceneId = playSceneId;
    _playSceneSet = true;
  }
}
