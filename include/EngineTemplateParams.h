#pragma once

#include "BoxSprite.h"
#include "Button.h"
#include "Checkbox.h"
#include "Dropdown.h"
#include "EngineConstants.h"
#include "Slider.h"
#include "Text.h"

namespace JadeEngine
{
  const TextParams kVera16LightGrey =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string fontName;
    kVeraFont,
    //uint32_t fontSize;
    16,
    //std::string text;
    "",
    //SDL_Color color;
    kLightGreyColor,
    //int32_t z;
    1,
  };

  const TextParams kVera1650Grey =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string fontName;
    kVeraFont,
    //uint32_t fontSize;
    16,
    //std::string text;
    "",
    //SDL_Color color;
    k50GreyColor,
    //int32_t z;
    1,
  };

  const TextParams kVeraBold1650Grey =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string fontName;
    kVeraFontBold,
    //uint32_t fontSize;
    16,
    //std::string text;
    "",
    //SDL_Color color;
    k50GreyColor,
    //int32_t z;
    1,
  };

  const TextParams kVeraBold1250Grey =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string fontName;
    kVeraFontBold,
    //uint32_t fontSize;
    12,
    //std::string text;
    "",
    //SDL_Color color;
    k50GreyColor,
    //int32_t z;
    1,
  };

  const ButtonParams kMainMenuBlueButton =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string textureNormal;
    "blue_button09.png",
    //std::string texturePressed;
    "blue_button10.png",
    //std::string textureDisabled;
    "grey_button11.png",
    //std::string textureHovered;
    "blue_button14.png",
    //bool spriteSheet;
    true,
    //std::string spriteSheetName;
    kJadeEngineUISpritesheet,
    //int32_t width;
    250,
    //int32_t height;
    50,
    //int32_t z;
    1,
    //int32_t cornerSize;
    9,
    //int32_t pressedDepth;
    5,

    //std::string text;
    "",
    //std::string fontName;
    kVeraFontBold,
    //uint32_t fontSize;
    32,
    //SDL_Color textColor;
    kLightAzureColor,
    //SDL_Color disabledTextColor;
    kLightGreyColor,

    //std::string clickSound;
    "uiClick",
  };

  const BoxSpriteParams kOptionsGreyPanel =
  {
    {
      //ObjectLayer layer;
      kObjectLayer_UI,
      //std::string textureName;
      "grey_panel.png",
      //int32_t z;
      0,
      //bool spriteSheet;
      true,
      //std::string spriteSheetName;
      kJadeEngineUISpritesheet,
    },
    //int32_t cornerSize;
    7,
    // Vector2D_i32 size;
    {510, 400}
  };

  const ButtonParams kOptionsSectionTitleButton =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string textureNormal;
    "grey_panel.png",
    //std::string texturePressed;
    "grey_panel.png",
    //std::string textureDisabled;
    "blue_panel.png",
    //std::string textureHovered;
    "",
    //bool spriteSheet;
    true,
    //std::string spriteSheetName;
    kJadeEngineUISpritesheet,

    //int32_t width;
    170,
    //int32_t height;
    50,
    //int32_t z;
    2,
    //int32_t cornerSize;
    7,
    //int32_t pressedDepth;
    0,

    //std::string text;
    "",
    //std::string fontName;
    kVeraFontBold,
    //uint32_t fontSize;
    24,
    //SDL_Color textColor;
    k50GreyColor,
    //SDL_Color disabledTextColor;
    kLightAzureColor,

    //std::string clickSound;
    "uiClick",
  };

  const SliderParams kOptionsSlider =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string font;
    kVeraFontBold,
    //uint32_t fontSize;
    16,
    //SDL_Color fontColor;
    k50GreyColor,
    //std::string minTitle;
    "0",
    //std::string maxTitle;
    "100",
    //int32_t width;
    300,
    //int32_t minMaxYMargin;
    20,
    //std::string axisTexture;
    "grey_sliderHorizontal.png",
    //std::string axisEndTexture;
    "grey_sliderEnd.png",
    //std::string pointerTexture;
    "grey_sliderDown.png",
    //bool spriteSheet;
    true,
    //std::string spriteSheetName;
    kJadeEngineUISpritesheet,
    //int32_t z;
    10,
    //float initialValue;
    0,
  };

  const ButtonParams kOptionsKeybindButton =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string textureNormal;
    "grey_button10.png",
    //std::string texturePressed;
    "grey_button11.png",
    //std::string textureDisabled;
    "blue_button06.png",
    //std::string textureHovered;
    "",
    //bool spriteSheet;
    true,
    //std::string spriteSheetName;
    kJadeEngineUISpritesheet,

    //int32_t width;
    120,
    //int32_t height;
    50,
    //int32_t z;
    1,
    //int32_t cornerSize;
    9,
    //int32_t pressedDepth;
    5,

    //std::string text;
    "",
    //std::string fontName;
    kVeraFontBold,
    //uint32_t fontSize;
    12,
    //SDL_Color textColor;
    k50GreyColor,
    //SDL_Color disabledTextColor;
    k50GreyColor,

    //std::string clickSound;
    "uiClick",
  };

  const CheckboxParams kBlueCheckbox =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string checkedTexture;
    "blue_boxCheckmark.png",
    //std::string emptyTexture;
    "grey_box.png",
    //bool spriteSheet;
    true,
    //std::string spriteSheetName;
    kJadeEngineUISpritesheet,
    //int32_t z;
    2,
    //bool checked;
    false
  };

  const DropdownParams kOptionsDropdown =
  {
    //ObjectLayer layer;
    kObjectLayer_UI,
    //std::string boxTexture;
    "dropdown_button.png",
    //int32_t cornerSize;
    9,
    //std::string expandArrowTexture;
    "grey_arrowDownGrey.png",
    //std::string contractArrowTexture;
    "grey_arrowUpGrey.png",
    //std::string scrollBarTexture;
    "grey_sliderVertical.png",
    //std::string scrollBarPointTexture;
    "grey_sliderEnd.png",
    //bool spriteSheet;
    true,
    //std::string spriteSheetName;
    kJadeEngineUISpritesheet,
    //int32_t width;
    400,
    //int32_t maxEntries;
    5,
    //int32_t entryHeight;
    50,
    //int32_t arrowsMargin;
    20,
    //int32_t scrollBarMargin;
    5,
    //int32_t z;
    5,
    //std::string textFont;
    kVeraFont,
    //uint32_t textSize;
    16,
    //SDL_Color textColor;
    k50GreyColor
  };
}
