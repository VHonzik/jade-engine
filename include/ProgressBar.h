#pragma once

#include "ICompositeObject.h"
#include "ObjectLayer.h"

#include <string>
#include <vector>

namespace JadeEngine
{
  struct ProgressBarColor
  {
    std::string textureLeft;
    std::string textureMiddle;
    std::string textureRight;

    float       threshold;
  };

  struct ProgressBarParams
  {
    ObjectLayer                   layer;

    std::vector<ProgressBarColor> colors;

    std::string                   backgroundTextureLeft;
    std::string                   backgroundTextureMiddle;
    std::string                   backgroundTextureRight;

    bool                          spriteSheet;
    std::string                   spriteSheetName;

    int32_t                       z;
    int32_t                       width;

    float                         fullBarUpdateDuration;
  };

  class Sprite;

  struct Foreground
  {
    Sprite* left;
    Sprite* middle;
    Sprite* right;
    float threshold;
  };

  class ProgressBar : public ICompositeObject
  {
  public:
    ProgressBar(const ProgressBarParams& params);

    void Update() override;
    void Show(const bool shown) override;

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    int32_t GetWidth() const;
    int32_t GetHeight() const;

    float GetT() const { return _t; }

    void InitValue(float maxValue, float initValue);
    void ChangeValue(float amount);

  private:
    void UpdateForegroundsVisibility();

    void SetT(const float value);

    std::vector<Foreground> _foregrounds;

    Sprite* _backgroundLeft;
    Sprite* _backgroundMiddle;
    Sprite* _backgroundRight;

    int32_t _backgroundWidth;
    int32_t _foregroundWidth;

    float _t;

    float _maxValue;
    float _currentValue;
    float _displayedValue;

    float _valueUpdateSpeed;
    float _valueUpdateDuration;
  };
}
