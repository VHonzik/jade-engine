#include "Audio.h"

#include "Utils.h"
#include "Settings.h"
#include "EngineSettings.h"

#include <SDL_mixer.h>

namespace JadeEngine
{
  Audio GAudio;

  bool Audio::Init()
  {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
      return false;
    }

    Mix_ReserveChannels(2);

    _musicChannelActive = 0;
    _musicChannelOld = 1;

    SetMusicVolume(GSettings.Get<float>(kSettingsIDs_MusicVolume));
    SetSoundVolume(GSettings.Get<float>(kSettingsIDs_SoundVolume));

    return true;
  }

  bool Audio::LoadSound(const std::string& soundName, const std::string& soundFile)
  {
    auto soundWav = Mix_LoadWAV(soundFile.c_str());

    if (soundWav == nullptr)
    {
      return false;
    }

    _sounds[soundName] = soundWav;

    return true;
  }

  void Audio::SwitchMusic(const char* soundName, bool loop)
  {
    auto sound = _sounds.find(soundName);
    if (sound != _sounds.end())
    {
      std::swap(_musicChannelOld, _musicChannelActive);
      Mix_PlayChannel(_musicChannelActive, sound->second, loop ? -1 : 0);
    }
  }

  void Audio::PlaySound(const char* soundName)
  {
    auto sound = _sounds.find(soundName);
    if (sound != _sounds.end())
    {
      Mix_PlayChannel(-1, sound->second, 0);
    }
  }

  void Audio::CleanUp()
  {
    for (auto& sound : _sounds)
    {
      Mix_FreeChunk(sound.second);
    }

    Mix_CloseAudio();
  }

  void Audio::SetMusicVolume(const float volume)
  {
    _musicVolume = Clamp01(volume);
    auto mixerVolume = Clamp(static_cast<int32_t>(_musicVolume * MIX_MAX_VOLUME),
      0, MIX_MAX_VOLUME);
    Mix_Volume(0, mixerVolume);
    Mix_Volume(1, mixerVolume);
  }

  void Audio::SetSoundVolume(const float volume)
  {
    _soundVolume = Clamp01(volume);
    auto mixerVolume = Clamp(static_cast<int32_t>(Clamp01(volume) * MIX_MAX_VOLUME),
      0, MIX_MAX_VOLUME);
    auto channelCount = Mix_GroupCount(-1);
    for (int32_t i = 2; i < channelCount; i++)
    {
      Mix_Volume(i, mixerVolume);
    }
  }
}
