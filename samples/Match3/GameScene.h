#pragma once

#include "IScene.h"

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

  class GameScene : public IScene
  {
  public:
    void Start() override;
    void Update() override;
  private:
    PiecesGrid* _grid;
    ScoreMeter* _scoreMeter;
    int32_t _score;
  };
}