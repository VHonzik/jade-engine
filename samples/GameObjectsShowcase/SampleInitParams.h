#pragma once

#include "EngineConstants.h"
#include "GameInitParams.h"

using namespace JadeEngine;

const GameInitParams kSampleInitParams = {
  // uint32_t renderingResolutionWidth;
  1280,
  //uint32_t renderingResolutionHeight;
  720,
  //std::string windowName;
  "GameObjectsShowcase",
  //std::string appName;
  "GameObjectsShowcase",
  //std::vector<uint32_t> fontSizes;
  {},
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
  },
  //std::vector<GameInitParamsKeyBindingEntry> keybindings;
  {
  },
  //std::string author;
  "Vaclav Honzik",
  //int32_t copyrightYear;
  2020,
  //int32_t majorVersion;
  0,
  //int32_t minorVersion;
  1,
  //std::string hashVersion;
  "4b825dc6"
};

