#pragma once

#include "IScene.h"

using namespace JadeEngine;

namespace JadeEngine
{
  class Button;
}

namespace GameObjectsShowcase
{
  class ShowcaseScene : public IScene
  {
    void Start() override;
    void Update() override;
  private:
    Button* _button;
  };
}