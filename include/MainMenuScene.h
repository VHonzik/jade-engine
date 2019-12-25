#pragma once

#include "IScene.h"

#include <string>

namespace JadeEngine
{
  class Button;

  class MainMenuScene : public IScene
  {
  public:
    void Start() override;
    void Update() override;

    void SetGameScene(const std::string& gameSceneName) { _gameSceneName = gameSceneName; }
  private:
    void ApplyDisplayModeFromSettings();

    Button* _playButton;
    Button* _optionsButton;
    Button* _quitButton;

    std::string _gameSceneName;
  };
}
