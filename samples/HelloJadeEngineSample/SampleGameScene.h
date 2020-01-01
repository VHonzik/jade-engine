#pragma once

#include "IScene.h"

using namespace JadeEngine;

namespace JadeEngine
{
  class Text;
}

namespace HelloJadeEngine
{
  class GameScene : public IScene
  {
  public:
    void Start() override;
    void Update() override {}
  private:
    Text* _helloJadeEngineText;
  };
}
