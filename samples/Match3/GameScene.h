#pragma once

#include "IScene.h"

#include <vector>

using namespace JadeEngine;

namespace MatchThree
{
  class Piece;

  class GameScene : public IScene
  {
  public:
    void Start() override;
    void Update() override;
    void EnableInput(const bool enable);
  private:
    std::vector<Piece*> _pieces;
    bool _inputEnabled;
  };
}