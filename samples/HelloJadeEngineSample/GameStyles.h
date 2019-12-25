#pragma once

#include <EngineStyles.h>

using namespace JadeEngine;

namespace HelloJadeEngine
{
  const char kGameScene[] = "GameScene";
  const char kAppName[] = "HelloJadeEngineSample";
  const char kAuthor[] = "Vaclav Honzik";

  const Style kGameStyle = {
    //uint32_t nativeResolutionWidth;
    1280,
    //uint32_t nativeResolutionHeight;
    720,
    //int32_t maxResolutionFraction;
    kEngineStyle.maxResolutionFraction,
    //std::string windowName;
    kAppName,
    //std::string appName;
    kAppName,
    //FTCParams fpsStyle;
    kEngineStyle.fpsStyle,

    //const std::vector<uint32_t> fontSizes;
    { 64 },

    //SDL_Color backgroundColor;
    kDarkGreyColor,

    //std::vector<TextureStyleEntry> textures;
    {
    },
    //std::vector<FontStyleEntry> fonts;
    {
      {kVeraFont,         "assets/Vera.ttf" },
    },
    //std::vector<SoundStyleEntry> sounds;
    {
    },
    //std::vector<CursorStyleEntry> cursors;
    {
    },
    //std::vector<SpriteSheetStyleEntry> spritesheets;
    {
    },
    //  std::vector<KeyBindingStyle> keybindings;
    {
    },
    //int32_t     majorVersion;
    1,
    //int32_t     minorVersion;
    0,
    //std::string hashVersion;
    kEngineStyle.hashVersion,
    //std::string author;
    "Vaclav Honzik",
    //int32_t     copyrightYear;
    2019
  };
}
