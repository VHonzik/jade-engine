#include "Game.h"
#include "PoweredByJadeEngineScene.h"
#include "SampleConstants.h"
#include "SampleInitParams.h"
#include "ShowcaseScene.h"

using namespace JadeEngine;
using namespace GameObjectsShowcase;

int32_t main(int32_t argc, char* argv[])
{
  auto& game = GGame;
  if (game.Initialize(kSampleInitParams, argv))
  {
    const auto poweredByJadeEngineScene = std::make_shared<PoweredByJadeEngineScene>();
    poweredByJadeEngineScene->SetNextScene(kSampleScene_ShowcaseScene);
    game.AddScene(kScene_PoweredByJadeEngine, poweredByJadeEngineScene);
    game.AddScene(kSampleScene_ShowcaseScene, std::make_shared<ShowcaseScene>());
    game.SetCursor(kCursorPointer);

    game.PlayScene(kScene_PoweredByJadeEngine);
    game.Start();
  }
  game.CleanUp();

  return 0;
}