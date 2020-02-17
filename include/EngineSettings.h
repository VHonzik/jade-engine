#pragma once

#include <vector>
#include "Settings.h"
#include "SDL_keycode.h"

namespace JadeEngine
{
  enum SettingsIDs
  {
    kSettingsIDs_MusicVolume,
    kSettingsIDs_SoundVolume,

    kSettingsIDs_FullScreen,
    kSettingsIDs_ResolutionWidth,
    kSettingsIDs_ResolutionHeight,
    kSettingsIDS_EngineEnd
  };

  namespace detail
  {
    const std::vector<SettingsEntry> kEngineSettings =
    {
      SettingsEntry::Create(kSettingsIDs_MusicVolume, 0.5f,
      "Music volume. Goes from 0.0 to 1.0, 0.0 being completely silent."),
      SettingsEntry::Create(kSettingsIDs_SoundVolume, 0.5f,
      "Sounds volume. Goes from 0.0 to 1.0, 0.0 being completely silent."),
      SettingsEntry::Create(kSettingsIDs_FullScreen, false,
      "Fullscreen mode."),
      SettingsEntry::Create(kSettingsIDs_ResolutionWidth, 1280,
      "Wanted resolution width. Minimum width is 800."),
      SettingsEntry::Create(kSettingsIDs_ResolutionHeight, 720,
      "Wanted resolution height. Minimum height is 600."),
    };
  }
}
