#pragma once

#include "IScene.h"

#include <vector>

using namespace JadeEngine;

namespace MatchThree
{
  class PiecesGrid;

  class GameScene : public IScene
  {
  public:
    void Start() override;
    void Update() override;
  private:
    PiecesGrid* _grid;
  };
}