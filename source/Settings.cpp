#include "Settings.h"

#include "EngineConstants.h"
#include "Game.h"
#include "SettingsFile.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <sstream>
#include <string>

namespace JadeEngine
{
  Settings GSettings;

  SettingsEntry::SettingsEntry()
    : _id(SettingsEntry::InvalidID)
    , _type(kSettingsType_Int)
    , _description("")
    , _value(0)
    , _defaultValue(0)
  {
  }

  SettingsEntry::SettingsEntry(const SettingsEntryID& id, const int32_t& defaultValue,
    const std::string& description)
    : SettingsEntry()
  {
    _id = id;
    _type = kSettingsType_Int;
    _value = defaultValue;
    _defaultValue = defaultValue;
    _description = description;
  }

  SettingsEntry::SettingsEntry(const SettingsEntryID& id, const float& defaultValue,
    const std::string& description)
    : SettingsEntry()
  {
    _id = id;
    _type = kSettingsType_Float;
    _value = defaultValue;
    _defaultValue = defaultValue;
    _description = description;
  }

  SettingsEntry::SettingsEntry(const SettingsEntryID& id, const bool& defaultValue,
    const std::string& description)
    : SettingsEntry()
  {
    _id = id;
    _type = kSettingsType_Bool;
    _value = defaultValue;
    _defaultValue = defaultValue;
    _description = description;
  }

  template<>
  int32_t SettingsEntry::GetValue<int32_t>() const
  {
    assert(_type == kSettingsType_Int);
    return std::get<int32_t>(_value);
  }

  template<>
  float SettingsEntry::GetValue<float>() const
  {
    assert(_type == kSettingsType_Float);
    return std::get<float>(_value);
  }

  template<>
  bool SettingsEntry::GetValue<bool>() const
  {
    assert(_type == kSettingsType_Bool);
    return std::get<bool>(_value);
  }

  template<>
  void SettingsEntry::SetValue<int32_t>(const int32_t& value)
  {
    assert(_type == kSettingsType_Int);
    _value = value;
  }

  template<>
  void SettingsEntry::SetValue<float>(const float& value)
  {
    assert(_type == kSettingsType_Float);
    _value = value;
  }

  template<>
  void SettingsEntry::SetValue<bool>(const bool& value)
  {
    assert(_type == kSettingsType_Bool);
    _value = value;
  }

  void Settings::ReadSingleEntry(const std::string& line)
  {
    std::istringstream dataStream(line);

    SettingsEntryID id;
    SettingsType type;
    auto separator = decltype(SettingsSeparator){};
    dataStream >> id >> separator >> type >> separator;

    auto found = _entries.find(id);
    assert(found != std::end(_entries));

    if (found != std::end(_entries))
    {
      assert(found->second.GetType() == type);
      switch (type)
      {
      case kSettingsType_Int:
      {
        int32_t value;
        dataStream >> value;
        found->second.SetValue<int>(value);
      }
      break;
      case kSettingsType_Float:
      {
        float value;
        dataStream >> value;
        found->second.SetValue<float>(value);
      }
      break;
      case kSettingsType_Bool:
      {
        bool value;
        dataStream >> value;
        found->second.SetValue<bool>(value);
      }
      break;
      default:
        assert(false); // Unknown type
        break;
      }
    }
  }

  bool Settings::Init(const std::string& appName)
  {
    _appName = appName;

    const auto settingsFile = SettingsFile::Get(_appName);
    if (!settingsFile.IsValid())
    {
      return false;
    }
    _settingsFile = settingsFile.FullPath();

    // If no settings file is found, try to fill the default ones
    if (!ReadSettings())
    {
      return WriteSettings();
    }

    return true;
  }

  bool Settings::ReadSettings()
  {
    std::fstream fs(_settingsFile, std::fstream::in);

    if (!fs.is_open())
    {
      return false;
    }

    std::string line;

    // First entry apart from headers is always build number
    bool buildNumberFound = false;
    while (!buildNumberFound && std::getline(fs, line))
    {
      if (line.empty() || !line.compare(0, SettingsComment.length(), SettingsComment))
      {
        continue;
      }
      buildNumberFound = true;
    }

    int32_t buildMajor;
    int32_t buildMinor;
    std::string hash;
    auto separator = decltype(kBuildSeperator){};
    std::istringstream dataStream(line);
    dataStream >> buildMajor >> separator >> buildMinor >> separator >> hash;

    // If different version of settings don't try to read it
    if (buildMajor != GGame.GetMajorVersion() || buildMinor != GGame.GetMinorVersion())
    {
      return false;
    }

    while (std::getline(fs, line))
    {
      if (line.empty() || !line.compare(0, SettingsComment.length(), SettingsComment))
      {
        continue;
      }
      ReadSingleEntry(line);
    }

    return true;
  }

  bool Settings::WriteSettings()
  {
    std::fstream fs(_settingsFile, std::fstream::binary | std::fstream::out);
    if (!fs.is_open())
    {
      return false;
    }

    fs << SettingsComment << " " << _appName << " settings file. Modify at your own risk." <<
      " Deleting this file will reset settings and new file will be created next run." << std::endl;

    fs << std::endl;
    fs << SettingsComment << " Build number" << std::endl;
    fs << GGame.GetMajorVersion() << kBuildSeperator <<
      GGame.GetMinorVersion() << kBuildSeperator << GGame.GetHashVersion() << std::endl;

    fs << std::endl;
    fs << SettingsComment << " Setting entries have following format:" << std::endl;
    fs << SettingsComment << " " << SettingsComment << " Setting description" << std::endl;
    fs << SettingsComment << " id" << SettingsSeparator << "type" << SettingsSeparator <<
      "value" << std::endl;
    fs << SettingsComment << " Where 'id' is unique identifier of the settings. 'type'" <<
      " is either "<< kSettingsType_Int << " (int), "
      << kSettingsType_Float << " (float) or "
      << kSettingsType_Bool <<" (bool). 'value' is the value of setting." << std::endl;

    for (const auto& entry : _entries)
    {
      fs << entry.second;
    }

    return true;
  }

  std::ostream& operator<<(std::ostream& stream, const SettingsType& type)
  {
    stream << static_cast<int32_t>(type);
    return stream;
  }

  std::istream& operator>>(std::istream& stream, SettingsType& type)
  {
    int32_t typeInt;
    stream >> typeInt;
    type = static_cast<SettingsType>(typeInt);
    return stream;
  }

  std::ostream& operator<<(std::ostream& stream, const SettingsEntry& entry)
  {
    stream << std::endl;

    stream << "//" << entry.GetDescription() << std::endl;
    stream << entry.GetID() << SettingsSeparator << entry.GetType() << SettingsSeparator;

    switch (entry.GetType())
    {
    case kSettingsType_Int:
      stream << entry.GetValue<int32_t>();
      break;
    case kSettingsType_Float:
      stream << entry.GetValue<float>();
      break;
    case kSettingsType_Bool:
      stream << entry.GetValue<bool>();
      break;
    default:
      assert(false); // Unknown type
      break;
    }

    stream << std::endl;

    return stream;
  }
}