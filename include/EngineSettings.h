#pragma once

#include <vector>
#include "Settings.h"
#include "SDL_keycode.h"

namespace JadeEngine
{
  enum SettingsIDs
  {
    kSettingsIDs_musicVolume,
    kSettingsIDs_soundVolume,

    kSettingsIDs_fullScreen,
    kSettingsIDs_resolutionWidth,
    kSettingsIDs_resolutionHeight,
    kSettingsIDS_engineEnd
  };

  const std::vector<SettingsEntry> kEngineSettings =
  {
    SettingsEntry::Create(kSettingsIDs_musicVolume, 0.5f,
    "Music volume. Goes from 0.0 to 1.0, 0.0 being completely silent."),
    SettingsEntry::Create(kSettingsIDs_soundVolume, 0.5f,
    "Sounds volume. Goes from 0.0 to 1.0, 0.0 being completely silent."),
    SettingsEntry::Create(kSettingsIDs_fullScreen, false,
    "Fullscreen mode."),
    SettingsEntry::Create(kSettingsIDs_resolutionWidth, 1280,
    "Wanted resolution width. Minimum width is 800."),
    SettingsEntry::Create(kSettingsIDs_resolutionHeight, 720,
    "Wanted resolution height. Minimum height is 600."),
  };
}
