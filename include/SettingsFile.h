/**
* "Stolen" and modified from https://github.com/Malvineous/cfgpath. Retrieved 11/18/2018.
* Relevant part of copyright from original file:
*
* -------------------------------------------------------------------
* Copyright (C) 2013 Adam Nielsen <malvineous@shikadi.net>
*
* This code is placed in the public domain.  You are free to use it for any
* purpose.  If you add new platform support, please contribute a patch!
* -------------------------------------------------------------------
*
*/

#pragma once

#include <string>
#include <sstream>

#ifdef _WIN32
#include <Shlobj.h>
#elif defined __linux__
#include <stdlib.h>
#define MAX_PATH 512  /* arbitrary value */
#endif

namespace JadeEngine
{
  class SettingsFile
  {
  public:
    static SettingsFile& Get(const std::string& appName)
    {
      static SettingsFile oneAndOnly(appName);
      return oneAndOnly;
    }

    bool IsValid() const { return _valid; };
    const std::string& FullPath() const { return _fullPath; };


  private:
    SettingsFile(const std::string& appName)
    {
      _valid = false;
#if defined(_WIN32)
      char buffer[MAX_PATH];

      if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer)))
      {
        return;
      }

      /* Don't try to create the AppData folder as it always exists already */
      std::stringstream stream;
      stream << std::string(buffer) << "\\" << appName << ".ini";

      _valid = true;
      _fullPath = stream.str();

#elif defined __linux__
      std::string home = getenv("XDG_CONFIG_HOME");
      bool configNeeded = false;
      if (home.length() == 0)
      {
        home = getenv("HOME");
        if (home.length() == 0)
        {
          return;
        }
        configNeeded = true;
      }

      std::stringstream stream;
      stream << home << '/';

      if (configNeeded)
      {
        stream << ".config/";
        mkdir(stream.str(), 0755);
      }

      stream << appName << ".conf";

      _valid = true;
      _fullPath = stream.str();
#endif
    }

    bool _valid;
    std::string _fullPath;
  };
}
