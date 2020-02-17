#pragma once

#include "EngineConstants.h"
#include "GameInitParams.h"

using namespace JadeEngine;

namespace MatchThree
{
  const GameInitParams kSampleInitParams = {
    // uint32_t renderingResolutionWidth;
    1280,
    //uint32_t renderingResolutionHeight;
    720,
    //std::string windowName;
    "Match3",
    //std::string appName;
    "Match3",
    //std::vector<uint32_t> fontSizes;
    { 64 },
    //SDL_Color backgroundColor;
    kDarkGreyColor,
    //std::vector<GameInitParamsTextureEntry> textures;
    {
    },
    //std::vector<GameInitParamsFontEntry> fonts;
    {
    },
    //std::vector<GameInitParamsSoundEntry> sounds;
    {
    },
    //std::vector<GameInitParamsCursorEntry> cursors;
    {
    },
    //std::vector<GameInitParamsSpriteSheetEntry> spritesheets;
    {
      { kMatchThreeSpritesheet, "assets/matchThree.png", "assets/matchThree.json", kTextureSampling_Anisotropic},
    },
    //std::vector<GameInitParamsKeyBindingEntry> keybindings;
    {
    },
    //std::string author;
    "Vaclav Honzik",
    //int32_t copyrightYear;
    2020,
  };
}
