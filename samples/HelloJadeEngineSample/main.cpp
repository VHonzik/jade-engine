
#include "EngineConstants.h"
#include "Game.h"
#include "GameInitParams.h"
#include "SampleGameScene.h"
#include "SampleInitParams.h"
#include "PoweredByJadeEngineScene.h"

using namespace JadeEngine;
using namespace HelloJadeEngine;

int32_t main(int32_t argc, char* argv[])
{
  auto& game = GGame;
  if (game.Initialize(kSampleInitParams, argv))
  {
    game.AddScene(kSampleScene_GameScene, std::make_shared<GameScene>());

    const auto poweredByJadeEngineScene = std::make_shared<PoweredByJadeEngineScene>();
    poweredByJadeEngineScene->SetNextScene(kSampleScene_GameScene);
    game.AddScene(kScene_PoweredByJadeEngine, poweredByJadeEngineScene);

    game.PlayScene(kScene_PoweredByJadeEngine);
    game.Start();
  }
  game.CleanUp();

  return 0;
}