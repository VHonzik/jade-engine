#include "Persistence.h"

#include "EngineTime.h"

#include <cassert>
#include <fstream>
#include <Shlobj.h>

namespace JadeEngine
{
  const char kPersistenceFilesExtension[] = ".json";
  const char kSettingFileName[] = "Settings";
  const char kGameStateFileName[] = "GameSave";

  SettingEntry::SettingEntry(const SettingID& id, const json& defaultValue, const std::string& entryName, const std::string& entryObjectName)
    : _id(id)
    , _value(defaultValue)
    , _name(entryName)
    , _objectName(entryObjectName)
  {
  }

  void SettingEntry::Write(json& settingsRoot) const
  {
    if (HasObject())
    {
      settingsRoot[_objectName][_name] = _value;
    }
    else
    {
      settingsRoot[_name] = _value;
    }
  }

  Persistence::Persistence()
    : _gameSaveState(kGameSaveState_NotFound)
    , _autoSaveFrequency(-1.0f)
    , _autoSaveTimer(0.0f)
    , _settingsPersist(false)
  {
    char buffer[MAX_PATH];
    const auto success = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer);
    assert(SUCCEEDED(success));

    _appDataPath = std::filesystem::weakly_canonical(std::filesystem::path(buffer));

    _settings.emplace(static_cast<SettingID>(Setting::MusicVolume),       SettingEntry::CreateTyped<Setting::MusicVolume>(0.5f, "musicVolume", "jadeEngine"));
    _settings.emplace(static_cast<SettingID>(Setting::SoundVolume),       SettingEntry::CreateTyped<Setting::SoundVolume>(0.5f, "soundVolume", "jadeEngine"));
    _settings.emplace(static_cast<SettingID>(Setting::FullScreen),        SettingEntry::CreateTyped<Setting::FullScreen>(false, "fullScreen", "jadeEngine"));
    _settings.emplace(static_cast<SettingID>(Setting::ResolutionWidth),   SettingEntry::CreateTyped<Setting::ResolutionWidth>(1280, "resolutionWidth", "jadeEngine"));
    _settings.emplace(static_cast<SettingID>(Setting::ResolutionHeight),  SettingEntry::CreateTyped<Setting::ResolutionHeight>(720, "resolutionHeight", "jadeEngine"));

    for (SettingID id = 0; id < static_cast<SettingID>(Setting::EngineEnd); id++)
    {
      const auto settingEntryFound = _settings.find(id);
      assert(settingEntryFound != std::cend(_settings));

      settingEntryFound->second.Write(_settingsJson);
    }
  }

  bool Persistence::Initialize(const std::string& appName, const bool persistSettings)
  {
    _storageFolderPath = _appDataPath / appName;
    if (!std::filesystem::exists(_storageFolderPath))
    {
      const auto createdFolder = std::filesystem::create_directory(_storageFolderPath);
      assert(createdFolder);
      if (!createdFolder)
      {
        return false;
      }
    }

    _settingsFilePath = _storageFolderPath / (std::string(kSettingFileName) + kPersistenceFilesExtension);
    if (_settingsPersist)
    {
      if (!std::filesystem::exists(_settingsFilePath))
      {
        WriteSettings();
      }
      else
      {
        ReadSettings();
      }
    }

    _gameStateFilePath = _storageFolderPath / (std::string(kGameStateFileName) + kPersistenceFilesExtension);
    if (std::filesystem::exists(_gameStateFilePath))
    {
      SetGameSaveState(kGameSaveState_Found);
    }

    return true;
  }

  void Persistence::Update()
  {
    if (_autoSaveFrequency > 0.0f)
    {
      _autoSaveTimer += GTime.deltaTime;
      if (_autoSaveTimer >= _autoSaveFrequency)
      {
        _autoSaveTimer -= _autoSaveFrequency;

        std::vector<GameSaveAutoSaveRequestReply> replies(_gameSaveListeners.size());
        for (auto listener : _gameSaveListeners)
        {
          replies.push_back(listener->GameSaveAutoSaveRequest());
        }

        // No-one block the game save request
        if (std::find(std::cbegin(replies), std::cend(replies), kGameSaveAutoSaveRequestReply_Block) == std::cend(replies))
        {
          // At least one state has changed
          if (std::find(std::cbegin(replies), std::cend(replies), kGameSaveAutoSaveRequestReply_Changed) != std::cend(replies))
          {
            WriteGameSave();
          }
        }
      }
    }
  }

  void Persistence::WriteSettings()
  {
    for (const auto& settingPair : _settings)
    {
      settingPair.second.Write(_settingsJson);
    }

    if (!_settingsPersist) return;

    std::ofstream ofs(_settingsFilePath);
    assert(ofs.is_open());
    if (ofs.is_open())
    {
      ofs << std::setw(2) << _settingsJson << std::endl;
    }
  }

  void Persistence::ReadSettings()
  {
    if (!_settingsPersist) return;

    std::ifstream ifs(_settingsFilePath);
    assert(ifs.is_open());

    if (ifs.is_open())
    {
      json settingsJson;
      ifs >> settingsJson;

      for (auto& settingEntryPair : _settings)
      {
        auto& settingEntry = settingEntryPair.second;
        if (settingEntry.HasObject())
        {
          const auto objectFound = settingsJson.find(settingEntry.GetObjectName());
          assert(objectFound != settingsJson.end());
          if (objectFound != settingsJson.end())
          {
            const auto entryFound = objectFound->find(settingEntry.GetName());
            assert(entryFound != objectFound->end());
            settingEntry.SetValue(*entryFound);
          }
        }
        else
        {
          const auto entryFound = settingsJson.find(settingEntry.GetName());
          if (entryFound != settingsJson.end())
          {
            settingEntry.SetValue(*entryFound);
          }
        }
      }

      WriteSettings();
    }
  }

  void Persistence::SetGameSaveState(const GameSaveState newState)
  {
    const auto previousState = GetGameSaveState();
    if (previousState != newState)
    {
      _gameSaveState = newState;
      for (auto listener : _gameSaveListeners)
      {
        assert(listener);
        listener->GameSaveStateChange(previousState, newState);
      }
    }
  }

  void Persistence::RegisterGameSaveListener(IGameSaveListener* listener)
  {
    assert(listener != nullptr);
    assert(std::find(std::cbegin(_gameSaveListeners), std::cend(_gameSaveListeners), listener) == std::cend(_gameSaveListeners));
    _gameSaveListeners.push_back(listener);

    const auto currentStatus = GetGameSaveState();
    for (auto listener : _gameSaveListeners)
    {
      assert(listener);
      listener->GameSaveStateChange(currentStatus, currentStatus);
    }

    if (currentStatus == kGameSaveStatus_Loaded)
    {
      for (auto listener : _gameSaveListeners)
      {
        assert(listener);
        listener->GameSaveLoaded(_gameSaveJson);
      }
    }
  }

  void Persistence::WriteGameSave()
  {
    _gameSaveJson.clear();

    for (auto listener : _gameSaveListeners)
    {
      assert(listener);
      listener->GameSaveWriteRequested(_gameSaveJson);
    }

    std::ofstream ofs(_gameStateFilePath);
    assert(ofs.is_open());
    if (ofs.is_open())
    {
      ofs << std::setw(2) << _gameSaveJson << std::endl;
      SetGameSaveState(kGameSaveStatus_Saved);
    }
  }

  void Persistence::LoadGameSave()
  {
    std::ifstream ifs(_gameStateFilePath);
    assert(ifs.is_open());

    if (ifs.is_open())
    {
      ifs >> _gameSaveJson;
      SetGameSaveState(kGameSaveStatus_Loaded);

      for (auto listener : _gameSaveListeners)
      {
        assert(listener);
        listener->GameSaveLoaded(_gameSaveJson);
      }
    }
  }

  void Persistence::StartAutoSaveRequest(const float frequency)
  {
    _autoSaveTimer = 0.0f;
    _autoSaveFrequency = frequency;
  }

  Persistence GPersistence;
}