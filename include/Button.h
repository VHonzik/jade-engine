#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"

#include <SDL.h>
#include <string>

namespace JadeEngine
{
  class BoxSprite;
  class Text;

  /**
  Parameters required to create a Button %game object.

  @see Game::Create

  Template to copy:
  @code
  ButtonParams buttonParams;
  buttonParams.layer = kObjectLayer_UI;
  buttonParams.textureNormal = "blue_button09.png";
  buttonParams.texturePressed = "blue_button10.png";
  buttonParams.textureDisabled = "grey_button11.png";
  buttonParams.textureHovered = "blue_button14.png";
  buttonParams.spriteSheet = true;
  buttonParams.spriteSheetName = "engineUI";
  buttonParams.width = 250;
  buttonParams.height = 50;
  buttonParams.z = 0;
  buttonParams.cornerSize = 9;
  buttonParams.pressedDepth = 5;
  buttonParams.text = "Button";
  buttonParams.fontName = "vera";
  buttonParams.fontSize = 32;
  buttonParams.textColor = { 255, 255, 255, 255 };
  buttonParams.disabledTextColor = { 153, 153, 153, 255 };
  buttonParams.clickSound = "uiClick";
  @endcode
  */
  struct ButtonParams
  {
    /**
    Layer that will the Sprite belong to.

    @see ObjectLayer
    */
    ObjectLayer layer;

    /**
    Texture to use representing the button when it is in the normal state.

    If the `spriteSheet` is true this refers to a texture defined in a sprite-sheet. See SpriteParams::textureName for more details.

    @see SpriteParams::textureName
    */
    std::string textureNormal;

    /**
    Texture to use representing the button when it is in the pressed state.

    If the `spriteSheet` is true this refers to a texture defined in a sprite-sheet. See SpriteParams::textureName for more details.

    @see SpriteParams::textureName
    */
    std::string texturePressed;

    /**
    Texture to use representing the button when it is disabled.

    If the `spriteSheet` is true this refers to a texture defined in a sprite-sheet. See SpriteParams::textureName for more details.

    @see SpriteParams::textureName, Button::Disable
    */
    std::string textureDisabled;

    /**
    Texture to use on top of `textureNormal` when the button is hovered by mouse.

    If the `spriteSheet` is true this refers to a texture defined in a sprite-sheet. See SpriteParams::textureName for more details.

    @see SpriteParams::textureName, Button::Disable
    */
    std::string textureHovered;

    /**
    Whether the `textureNormal`, `texturePressed`, `textureDisabled`, `textureHovered` refer to sprite-sheet contained textures. Same meaning as SpriteParams::spriteSheet.

    @see SpriteParams::spriteSheet, ButtonParams::spriteSheetName
    */
    bool          spriteSheet;

    /**
    If `spriteSheet` is true defines the texture to use as sprite-sheet source. Same meaning as SpriteParams::spriteSheetName.

    @see SpriteParams::spriteSheetName, ButtonParams::spriteSheet
    */
    std::string   spriteSheetName;
    int32_t       width;
    int32_t       height;
    int32_t       z;
    int32_t       cornerSize;
    int32_t       pressedDepth;
    std::string   text;
    std::string   fontName;
    uint32_t      fontSize;
    SDL_Color     textColor;
    SDL_Color     disabledTextColor;
    std::string   clickSound;
  };

  class Button : public IGameObject
  {
  public:
    Button(const ButtonParams& params);

    void Disable(bool disabled);
    bool Disabled() const { return _disabled; };

    void Show(const bool shown) override;

    void Update() override;

    void SetPosition(uint32_t x, uint32_t y);
    void SetCenterPosition(uint32_t x, uint32_t y);

    int32_t GetWidth() const;
    int32_t GetHeight() const;

    int32_t GetX() const;
    int32_t GetY() const;

    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    void SetText(const std::string& text);

    bool Pressed() { return _pressed; }
    bool Down() { return _down; }
    bool Released() { return _released; }

  private:
    void AdjustTextPosition();

    Text* _text;
    BoxSprite* _normalSprite;
    BoxSprite* _pressedSprite;
    BoxSprite* _disabledSprite;
    BoxSprite* _hoveredSprite;

    int32_t _pressedOffset;
    int32_t _disabledOffset;

    SDL_Color _normalTextColor;
    SDL_Color _disabledTextColor;

    std::string _clickSound;

    bool _disabled;

    bool _hovered;
    bool _pressed;
    bool _released;
    bool _down;
  };
}