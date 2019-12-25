#pragma once

#include "FTC.h"

#include <SDL_pixels.h>
#include <string>

namespace JadeEngine
{
  struct TextureStyleEntry
  {
    std::string assetName;
    std::string fileLocation;
    bool        generateHitMap;
  };

  struct FontStyleEntry
  {
    std::string assetName;
    std::string fileLocation;
  };

  struct SoundStyleEntry
  {
    std::string assetName;
    std::string fileLocation;
  };

  struct CursorStyleEntry
  {
    std::string assetName;
    std::string fileLocation;
    int32_t     centerX;
    int32_t     centerY;
  };

  struct SpriteSheetStyleEntry
  {
    std::string assetName;
    std::string textureFileLocation;
    std::string sheetJSONFileLocation;
  };

  struct KeyBindingStyleEntry
  {
    std::string uiDescription;
    int32_t     settingsId;
    std::string settingsDescription;
    int32_t     defaultKey;
  };

  struct Style
  {
    uint32_t nativeResolutionWidth;
    uint32_t nativeResolutionHeight;

    int32_t maxResolutionFraction;

    std::string windowName;
    std::string appName;

    FTCParams fpsStyle;

    std::vector<uint32_t> fontSizes;

    SDL_Color backgroundColor;

    std::vector<TextureStyleEntry> textures;
    std::vector<FontStyleEntry> fonts;
    std::vector<SoundStyleEntry> sounds;
    std::vector<CursorStyleEntry> cursors;
    std::vector<SpriteSheetStyleEntry> spritesheets;
    std::vector<KeyBindingStyleEntry> keybindings;

    int32_t     majorVersion;
    int32_t     minorVersion;
    std::string hashVersion;

    std::string author;
    int32_t     copyrightYear;
  };
}
