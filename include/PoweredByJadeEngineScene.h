#pragma once

#include "IScene.h"

namespace JadeEngine
{
  class Text;
  class TextSprite;

  class PoweredByJadeEngineScene : public IScene
  {
  public:
    PoweredByJadeEngineScene();
    void Start() override;
    void Update() override;
  private:
    Text* _text;
    TextSprite* _logo;
  };
}
