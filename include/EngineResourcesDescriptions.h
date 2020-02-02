#pragma once

#include "EngineDataTypes.h"

#include <cstdint>
#include <SDL_mouse.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

namespace JadeEngine::detail
{
  struct FontDescription
  {
    std::string name;
    TTF_Font* ttfFont;
    uint32_t size;
  };

  struct CursorDescription
  {
    SDL_Cursor* cursor;
    SDL_Surface* surface;
  };

  struct SpriteSheetEntryDescription
  {
    Rectangle rect;
  };

  struct SpriteSheetDescription
  {
    std::string textureName;
    std::unordered_map<std::string, SpriteSheetEntryDescription> sprites;
  };

  struct KeyBindingDescription
  {
    std::string uiDescription;
    int32_t     key;
  };
}

