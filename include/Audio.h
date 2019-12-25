#pragma once

#include <unordered_map>

struct Mix_Chunk;

namespace JadeEngine
{
  class Audio
  {
  public:
    bool Init();
    bool LoadSound(const std::string& soundName, const std::string& soundFile);

    void SwitchMusic(const char* soundName, bool loop);

    void PlaySound(const char* soundName);
    void CleanUp();

    void SetMusicVolume(const float volume);
    void SetSoundVolume(const float volume);

    float GetMusicVolume() const { return _musicVolume; };
    float GetSoundVolume() const { return _soundVolume; };

  private:
    std::unordered_map<std::string, Mix_Chunk*> _sounds;
    int _musicChannelActive;
    int _musicChannelOld;

    float _musicVolume;
    float _soundVolume;
  };

  extern Audio GAudio;
}
