#pragma once

#include "IScene.h"

#include <cstdint>

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
    void SetNextScene(const int32_t nextSceneId);
  private:
    Text* _text;
    TextSprite* _logo;
    float _nextSceneTimer;
    int32_t _nextSceneId;
  };
}
