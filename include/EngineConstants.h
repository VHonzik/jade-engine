#pragma once

#include <inttypes.h>
#include <SDL_pixels.h>

namespace JadeEngine
{
  const char kBuildSeperator = '.';

  const char kDefaultTextureName[] = "default";

  enum Scene
  {
    kScene_PoweredByJadeEngine,
    kScene_MainMenu,
    kScene_OptionsMenu,
    kScene_JadeEngineScenesEnd
  };

  const SDL_Color k50GreyColor = { 153, 153, 153, 255 };
  const SDL_Color kAzureColor = { 135, 206, 250, 255 };
  const SDL_Color kBlackColor = { 0, 0, 0, 255 };
  const SDL_Color kLighterDarkGreyColor = { 50, 50, 50, 255 };
  const SDL_Color kDarkGreyColor = { 30, 30, 30, 255 };
  const SDL_Color kGoldColor = { 248, 237, 49, 255 };
  const SDL_Color kGreenColor = { 0, 147, 68, 255 };
  const SDL_Color kJadeGreenColor = { 66, 142, 59, 255 };
  const SDL_Color kLightAzureColor = { 222, 246, 255, 255 };
  const SDL_Color kLightBlueColor = { 86, 156, 205, 255 };
  const SDL_Color kLightGreenColor = { 78, 201, 176, 255 };
  const SDL_Color kLightGreyColor = { 200, 200, 200, 255 };
  const SDL_Color kLightRedColor = { 140, 47, 47, 255 };
  const SDL_Color kOrangeColor = { 214, 157, 133, 255 };
  const SDL_Color kPurpleColor = { 189, 99, 197, 255 };
  const SDL_Color kRedColor = { 255, 0, 0, 255 };
  const SDL_Color kWhiteColor = { 255, 255, 255, 255 };
  const SDL_Color kYellowColor = { 233, 213, 133, 255 };

  const char kKennyFontSquare[] = "kennyfsquare";
  const char kVeraFont[] = "vera";
  const char kVeraFontBold[] = "verabold";

  const char kJadeEngineLogoTexture[] = "jadeenginelogo";
  const char kJadeEngineUISpritesheet[] = "engineUI";

  const int32_t kDefaultMaxResolutionFraction = 8;
}