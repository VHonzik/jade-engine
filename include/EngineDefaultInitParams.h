#pragma once

#include "EngineConstants.h"
#include "EngineTemplateParams.h"
#include "GameInitParams.h"
#include "ObjectLayer.h"
#include "Style.h"

namespace JadeEngine
{
  const FTCParams kDefaultFPSFTCParams = {
    //ObjectLayer layer;
    kObjectLayer_persitant_ui,
    //TTF_Font* font;
    nullptr,
    //std::string fontName;
    kKennyFontSquare,
    //uint32_t fontSize;
    12,
    //std::string format;
    "FPS: #",
    //SDL_Color defaultColor;
    kLightGreyColor,
  };

  const auto kDefaultFontSizes = decltype(GameInitParams::fontSizes){ 12, 16, 24, 32 };

  const auto kDefaultTextures = decltype(GameInitParams::textures){
    { kJadeEngineLogoTexture, "assets/jadeEngineLogo.png", false},
  };

  const auto kDefaultFonts = decltype(GameInitParams::fonts){
    { kKennyFontSquare,  "assets/KenneyPixelSquare.ttf"},
    { kVeraFontBold,     "assets/VeraBd.ttf" },
    { kVeraFont,         "assets/Vera.ttf" },
  };

  const auto kDefaultSounds = decltype(GameInitParams::sounds){
      { "uiBeep",         "assets/UIBeepDoubleQuickDeepMuffledstereo.wav"},
      { "uiClick",        "assets/UIClickDistinctShortmono.wav"},
  };

  const auto kDefaultCursors = decltype(GameInitParams::cursors){
  };

  const auto kDefaultSpritesheets = decltype(GameInitParams::spritesheets){
    { kJadeEngineUISpritesheet, "assets/engineUI.png", "assets/engineUI.json" },
  };

  const auto kDefaultKeybindings = decltype(GameInitParams::keybindings){
  };
}
