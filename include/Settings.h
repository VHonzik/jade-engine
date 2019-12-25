#pragma once

#include <unordered_map>
#include <variant>
#include <ostream>
#include <cassert>


namespace JadeEngine
{
  enum SettingsType
  {
    kSettingsType_Int,
    kSettingsType_Float,
    kSettingsType_Bool
  };

  using SettingsEntryID = int32_t;

  static const char SettingsSeparator = ';';
  static const std::string SettingsComment = "//";

  class SettingsEntry
  {
  public:
    static const SettingsEntryID InvalidID = -1;

    SettingsEntry();

    template<typename T>
    static SettingsEntry Create(const SettingsEntryID& id, const T& defaultValue,
      const std::string& description)
    {
      return SettingsEntry(id, defaultValue, description);
    }

    template<typename ValueType, typename KeyType>
    static SettingsEntry Create(const KeyType& id, const ValueType& defaultValue,
      const std::string& description)
    {
      return Create<ValueType>(static_cast<SettingsEntryID>(id), defaultValue, description);
    }

    template<typename T>
    void SetValue(const T& value);

    template<typename ReturnType>
    ReturnType GetValue() const;

    SettingsEntryID GetID() const { return _id; }
    SettingsType GetType() const { return _type; }
    const std::string& GetDescription() const { return _description; }

  private:
    SettingsEntry(const SettingsEntryID& id, const int32_t& defaultValue,
      const std::string& description);
    SettingsEntry(const SettingsEntryID& id, const float& defaultValue,
      const std::string& description);
    SettingsEntry(const SettingsEntryID& id, const bool& defaultValue,
      const std::string& description);

    SettingsEntryID _id;
    SettingsType _type;
    std::string _description;

    std::variant<int32_t, float, bool> _value;
    std::variant<int32_t, float, bool> _defaultValue;
  };

  class Settings
  {
  public:

    bool Init(const std::string& appName);

    template<typename ReturnType>
    ReturnType Get(const SettingsEntryID& id) const
    {
      assert(_entries.count(id) != 0);
      return _entries.find(id)->second.GetValue<ReturnType>();
    }

    template<typename ReturnType, typename IDType>
    ReturnType Get(const IDType& id) const
    {
      return Get<ReturnType>(static_cast<SettingsEntryID>(id));
    }

    template<typename ValueType>
    void Set(const SettingsEntryID& id, const ValueType& value)
    {
      assert(_entries.count(id) != 0);
      _entries.find(id)->second.SetValue<ValueType>(value);
    }

    template<typename ValueType, typename IDType>
    void Set(const IDType& id, const ValueType& value)
    {
      return Set<ValueType>(static_cast<SettingsEntryID>(id), value);
    }

    void Register(const SettingsEntry& entry)
    {
      _entries[entry.GetID()] = entry;
    }

    template<typename InputIt>
    void RegisterAll(InputIt first, InputIt last)
    {
      for (auto it = first; it != last; ++it)
      {
        Register(*it);
      }
    }

    bool ReadSettings();
    bool WriteSettings();

  private:
    void ReadSingleEntry(const std::string& line);

    std::unordered_map<SettingsEntryID, SettingsEntry> _entries;
    std::string _settingsFile;
    std::string _appName;
  };

  std::ostream& operator<<(std::ostream& stream, const SettingsType& type);
  std::istream& operator>>(std::istream& stream, SettingsType& type);
  std::ostream& operator<<(std::ostream& stream, const SettingsEntry& entry);

  extern Settings GSettings;
}

