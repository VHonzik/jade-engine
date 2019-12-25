#pragma once

#include "EngineConstants.h"
#include "EngineTemplateParams.h"
#include "ObjectLayer.h"
#include "Style.h"

namespace JadeEngine
{
  const char kAppName[] = "JadeEngine";

  const Style kEngineStyle =
  {
    //uint32_t nativeResolutionWidth;
    1280,
    //uint32_t nativeResolutionHeight;
    720,
    //int32_t maxResolutionFraction;
    8,
    //std::string windowName;
    kAppName,
    //std::string appName;
    kAppName,
    //FTCParams fpsStyle;
    {
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
    },

    //const std::vector<uint32_t> fontSizes;
    { 12, 16, 24, 32 },

    //SDL_Color backgroundColor;
    kDarkGreyColor,

    //std::vector<TextureStyleEntry> textures;
    {
      { kJadeEngineLogoTexture, "assets/jadeEngineLogo.png", false},
    },
    //std::vector<FontStyleEntry> fonts;
    {
      {kKennyFontSquare,  "assets/KenneyPixelSquare.ttf"},
      {kVeraFontBold,     "assets/VeraBd.ttf"},
      {kVeraFont,         "assets/Vera.ttf" },
    },
    //std::vector<SoundStyleEntry> sounds;
    {
      { "uiBeep",         "assets/UIBeepDoubleQuickDeepMuffledstereo.wav"},
      { "uiClick",        "assets/UIClickDistinctShortmono.wav"},
    },
    //std::vector<CursorStyleEntry> cursors;
    {
    },
    //std::vector<SpriteSheetStyleEntry> spritesheets;
    {
      { kJadeEngineUISpritesheet, "assets/engineUI.png", "assets/engineUI.json" },
    },
    //  std::vector<KeyBindingStyle> keybindings;
    {
    },
    //int32_t     majorVersion;
    1,
    //int32_t     minorVersion;
    0,
    //std::string hashVersion;
    "4b825dc6", // Not important but git empty tree hash - first 8 characters
    //std::string author;
    "Vaclav Honzik",
    //int32_t     copyrightYear;
    2019
  };
}
