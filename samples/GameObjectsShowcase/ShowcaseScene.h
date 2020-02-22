#pragma once

#include "IScene.h"

using namespace JadeEngine;

namespace JadeEngine
{
  class BoxSprite;
  class Button;
  class LineBox;
  class Text;
  class TransformGroup;
  class Checkbox;
}

namespace GameObjectsShowcase
{
  class ShowcaseScene : public IScene
  {
    void Start() override;
    void Update() override;
  private:
    Button* _button;
    Text* _text;
    TransformGroup* _leftColumnGroup;
    LineBox* _leftColumnBox;
    Text* _leftColumnTitle;
    BoxSprite* _boxSprite;
    Text* _boxSpriteTitle;
    Checkbox* _checkbox;
    Text* _checkboxTitle;
  };
}