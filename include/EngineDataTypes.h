#pragma once

#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

namespace JadeEngine
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
    SDL_Rect rect;
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
