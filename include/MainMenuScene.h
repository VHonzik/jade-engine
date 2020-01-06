#pragma once

#include "IScene.h"

#include <string>

namespace JadeEngine
{
  class Button;

  class MainMenuScene : public IScene
  {
  public:
    MainMenuScene();
    void Start() override;
    void Update() override;

    void SetPlayButtonScene(const int32_t playSceneId);
  private:
    void ApplyDisplayModeFromSettings();

    Button* _playButton;
    Button* _optionsButton;
    Button* _quitButton;

    int32_t _playSceneId;
    bool    _playSceneSet;
  };
}
