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

    void Show(const bool shown) override;

    bool Changed() const { return _checkedChanged; }
    bool Checked() const { return _checked; }

  private:
    bool _checked;
    bool _checkedChanged;

    Sprite* _checkedSprite;
    Sprite* _emptySprite;
  };
}
