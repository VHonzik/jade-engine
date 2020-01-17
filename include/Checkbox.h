#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"

#include <cstdint>
#include <string>

namespace JadeEngine
{
  class Sprite;

  struct CheckboxParams
  {
    ObjectLayer   layer;
    std::string   checkedTexture;
    std::string   emptyTexture;
    bool          spriteSheet;
    std::string   spriteSheetName;
    int32_t       z;
    bool          checked;
  };

  class Checkbox : public IGameObject
  {
  public:
    Checkbox(const CheckboxParams& params);

    void Update() override;

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    void Show(const bool shown) override;

    int32_t GetX() const;
    int32_t GetY() const;

    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    int32_t GetWidth() const;
    int32_t GetHeight() const;

    bool Changed() const { return _checkedChanged; }
    bool Checked() const { return _checked; }

  private:
    bool _checked;
    bool _checkedChanged;

    Sprite* _checkedSprite;
    Sprite* _emptySprite;
  };
}
