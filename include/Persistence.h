#pragma once

#include "EngineConstants.h"
#include "TypedSetting.h"

#include <cstdint>
#include <json.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>

using nlohmann::json;

namespace JadeEngine
{
  enum GameSaveState
  {
    kGameSaveState_NotFound,
    kGameSaveState_Found,
    kGameSaveStatus_Loaded,
    kGameSaveStatus_Saved
  };

  enum GameSaveAutoSaveRequestReply
  {
    kGameSaveAutoSaveRequestReply_NotChanged,
    kGameSaveAutoSaveRequestReply_Changed,
    kGameSaveAutoSaveRequestReply_Block
  };

  class IGameSaveListener
  {
  public:
    virtual void GameSaveLoaded(const json& save) {};
    virtual void GameSaveStateChange(const GameSaveState previousState, const GameSaveState newState) {};
    virtual void GameSaveWriteRequested(json& save) {};
    virtual GameSaveAutoSaveRequestReply GameSaveAutoSaveRequest() { return kGameSaveAutoSaveRequestReply_NotChanged; };
  };

  class SettingEntry
  {
  public:
    SettingID GetID() const { return _id; }

    template<auto ScopedEnumValue>
    static SettingEntry CreateTyped(const TypedSetting_t<ScopedEnumValue>& defaultValue, const std::string& entryName, const std::string& entryObjectName = "")
    {
      return Create<TypedSetting_t<ScopedEnumValue>>(static_cast<SettingID>(ScopedEnumValue), defaultValue, entryName, entryObjectName);
    }

    template<typename T>
    static SettingEntry Create(const SettingID& id, const T& defaultValue, const std::string& entryName, const std::string& entryObjectName = "")
    {
      return SettingEntry(id, defaultValue, entryName, entryObjectName);
    }

    template<auto ScopedEnumValue>
    void SetValueTyped(const TypedSetting_t<ScopedEnumValue>& value)
    {
      SetValue<TypedSetting_t<ScopedEnumValue>>(value);
    }

    template<auto ScopedEnumValue>
    TypedSetting_t<ScopedEnumValue> GetValueTyped() const
    {
      return _value.get<TypedSetting_t<ScopedEnumValue>>();
    }

    template<typename T>
    T GetValue() const
    {
      return _value.get<T>();
    }

    template<typename T>
    void SetValue(const T& value)
    {
      _value = value;
    }

    void SetValue(const json& value)
    {
      _value = value;
    }

    const std::string& GetName() const { return _name; }
    const std::string& GetObjectName() const { return _objectName; }
    bool HasObject() const { return !_objectName.empty(); }
    void Write(json& settingsRoot) const;

  private:
    SettingEntry(const SettingID& id, const json& defaultValue, const std::string& entryName, const std::string& entryObjectName);

    SettingID _id;
    std::string _name;
    std::string _objectName;
    json _value;
  };


  class Persistence
  {
  public:
    Persistence();
    bool Initialize(const std::string& appName, const bool persistSettings);
    void Update();

    template<auto ScopedEnumValue>
    TypedSetting_t<ScopedEnumValue> GetSettingTyped() const
    {
      const auto settingId = static_cast<SettingID>(ScopedEnumValue);
      const auto settingEntryFound = _settings.find(settingId);
      assert(settingEntryFound != std::cend(_settings));
      return settingEntryFound->second.GetValueTyped<ScopedEnumValue>();
    }

    template<typename T>
    T GetSetting(const SettingID& id) const
    {
      const auto settingEntryFound = _settings.find(id);
      assert(settingEntryFound != std::cend(_settings));
      return settingEntryFound->second.GetValue<T>();
    }

    template<auto ScopedEnumValue>
    void SetSettingTyped(const TypedSetting_t<ScopedEnumValue>& value)
    {
      const auto settingId = static_cast<SettingID>(ScopedEnumValue);
      const auto settingEntryFound = _settings.find(settingId);
      assert(settingEntryFound != std::cend(_settings));
      settingEntryFound->second.SetValueTyped<ScopedEnumValue>(value);
    }

    template<typename T>
    void SetSetting(const SettingID& id, const T& value)
    {
      const auto settingEntryFound = _settings.find(id);
      assert(settingEntryFound != std::cend(_settings));
      return settingEntryFound->second.SetValue<T>(value);
    }

    void WriteSettings();
    void ReadSettings();

    void RegisterSetting(const SettingEntry& entry)
    {
      _settings.emplace(entry.GetID(), entry);
    }

    template<typename InputIt>
    void RegisterSettings(InputIt first, InputIt last)
    {
      for (auto it = first; it != last; ++it)
      {
        RegisterSetting(*it);
      }
    }

    void RegisterGameSaveListener(IGameSaveListener* listener);
    void WriteGameSave();
    GameSaveState GetGameSaveState() const { return _gameSaveState; }
    void LoadGameSave();
    void StartAutoSaveRequest(const float frequency);

  private:
    void SetGameSaveState(const GameSaveState newState);

    std::filesystem::path _appDataPath;
    std::filesystem::path _storageFolderPath;
    std::filesystem::path _settingsFilePath;
    std::filesystem::path _gameStateFilePath;

    bool _settingsPersist;
    std::unordered_map<SettingID, SettingEntry> _settings;
    json _settingsJson;

    GameSaveState _gameSaveState;
    json _gameSaveJson;
    float _autoSaveFrequency;
    float _autoSaveTimer;
    std::vector<IGameSaveListener*> _gameSaveListeners;
  };

  extern Persistence GPersistence;
}
