#pragma once

#include "IScene.h"
#include "Persistence.h"

#include <vector>
#include <cstdint>

using namespace JadeEngine;

namespace JadeEngine
{
  class FTC;
}

namespace MatchThree
{
  class PiecesGrid;
  class ScoreMeter;

  class GameScene : public IScene, public IGameSaveListener
  {
  public:
    void Start() override;
    void Update() override;

    void GameSaveStateChange(const GameSaveState previousState, const GameSaveState newState) override;
    GameSaveAutoSaveRequestReply GameSaveAutoSaveRequest() override;

  private:
    PiecesGrid* _grid;
    ScoreMeter* _scoreMeter;
    int32_t _score;
  };
}