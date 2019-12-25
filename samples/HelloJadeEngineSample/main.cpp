#include "GameStyles.h"
#include "GameScene.h"

#include "EngineStyles.h"
#include "Game.h"
#include "PoweredByJadeEngineScene.h"

using namespace JadeEngine;
using namespace HelloJadeEngine;

int32_t main(int32_t argc, char* argv[])
{
  auto& game = GGame;
  if (game.Initialize({ std::ref(kEngineStyle),  std::ref(kGameStyle) }))
  {
    game.AddScene(kGameScene, std::make_shared<GameScene>());
    game.AddScene(kPoweredByJadeEngineScene, std::make_shared<PoweredByJadeEngineScene>());
    game.PlayScene(kPoweredByJadeEngineScene);
    game.Start();
  }
  game.CleanUp();

  return 0;
}