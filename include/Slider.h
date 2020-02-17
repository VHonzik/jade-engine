#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"

#include <SDL.h>
#include <string>

namespace JadeEngine
{
  class Sprite;
  class Text;

  struct SliderParams
  {
    ObjectLayer   layer;
    std::string   font;
    uint32_t      fontSize;
    SDL_Color     fontColor;
    std::string   minTitle;
    std::string   maxTitle;
    int32_t       width;
    int32_t       minMaxYMargin;
    std::string   axisTexture;
    std::string   axisEndTexture;
    std::string   pointerTexture;
    bool          spriteSheet;
    std::string   spriteSheetName;
    int32_t       z;
    float         initialValue;
  };

  class Slider : public IGameObject
  {
  public:
    Slider(const SliderParams& params);

    void Update() override;

    void Show(const bool shown) override;

    float GetValue() const { return _value; }
    bool Changed() const { return _valueChanged; }
    bool Released() const { return _slidingReleased;  }

  private:
    Sprite* _pointer;
    Sprite* _axis;
    Sprite* _axisLEnd;
    Sprite* _axisREnd;

    Text* _minTitle;
    Text* _maxTitle;

    int32_t _width;
    int32_t _minMaxYMargin;

    bool _valueChanged;
    float _value;

    bool _sliding;
    bool _slidingReleased;
    int32_t _slidingOffset;
  };
}
