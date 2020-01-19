
#include "EngineConstants.h"
#include "Game.h"
#include "GameInitParams.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "OptionsMenuScene.h"
#include "PoweredByJadeEngineScene.h"
#include "SampleConstants.h"
#include "SampleInitParams.h"

using namespace JadeEngine;
using namespace MatchThree;

int32_t main(int32_t argc, char* argv[])
{
  auto& game = GGame;
  if (game.Initialize(kSampleInitParams, argv))
  {
    const auto poweredByJadeEngineScene = std::make_shared<PoweredByJadeEngineScene>();
    poweredByJadeEngineScene->SetNextScene(kScene_MainMenu);
    game.AddScene(kScene_PoweredByJadeEngine, poweredByJadeEngineScene);
    const auto mainMenuScene = std::make_shared<MainMenuScene>();
    mainMenuScene->SetPlayButtonScene(kSampleScene_GameScene);
    game.AddScene(kScene_MainMenu, mainMenuScene);
    game.AddScene(kScene_OptionsMenu, std::make_shared<OptionsMenuScene>());
    game.AddScene(kSampleScene_GameScene, std::make_shared<GameScene>());
    game.SetCursor(kCursorPointer);

    //game.PlayScene(kScene_PoweredByJadeEngine);
    game.PlayScene(kScene_MainMenu);
    game.Start();
  }
  game.CleanUp();

  return 0;
}