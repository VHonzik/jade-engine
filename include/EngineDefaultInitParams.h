#pragma once

#include "EngineConstants.h"
#include "EngineTemplateParams.h"
#include "FTC.h"
#include "GameInitParams.h"
#include "ObjectLayer.h"
#include "TextureSampling.h"

namespace JadeEngine
{
  const FTCParams kDefaultFPSFTCParams = {
    //ObjectLayer layer;
    kObjectLayer_persistent_ui,
    //std::string fontName;
    kKennyFontSquare,
    //uint32_t fontSize;
    12,
    //std::string format;
    "FPS: #",
    //SDL_Color defaultColor;
    kLightGreyColor,
  };

  const auto kDefaultFontSizes = decltype(GameInitParams::fontSizes){ 12, 16, 24, 32, 64 };

  const auto kDefaultTextures = decltype(GameInitParams::textures){
    { kJadeEngineLogoTexture, "assets/jadeEngineLogo.png", false, kTextureSampling_anisotropic},
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
    { kCursorPointer, "assets/cursor_pointer3D.png", 3, 3},
  };

  const auto kDefaultSpritesheets = decltype(GameInitParams::spritesheets){
    { kJadeEngineUISpritesheet, "assets/engineUI.png", "assets/engineUI.json", kTextureSampling_anisotropic },
  };

  const auto kDefaultKeybindings = decltype(GameInitParams::keybindings){
  };
}
