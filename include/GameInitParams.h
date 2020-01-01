/** @file GameInitParams.h
 * Structures for initialization of a new game instance.
 *
 * @author Vaclav Honzik <honzik.vaclav@gmail.com>
 * @date 2019
 */

#pragma once

#include <cstdint>
#include <SDL_pixels.h>
#include <string>
#include <vector>

namespace JadeEngine
{
  /**
  Parameters for loading a texture.
  */
  struct GameInitParamsTextureEntry
  {
    /**
    String to identify the texture that will be used to reference it when creating game objects with textures.

    Must be unique across all loaded textures.
    */
    std::string assetName;

    /**
    String with full path to the texture file relative to the executable.

    For example "assets/engineUI.png".
    */
    std::string fileLocation;

    /**
    Whether to run additional processing on the texture to create a hit map.

    Hit map simply remembers for each pixel of the texture whether it is opaque (alpha > 0).
    This is then be used in Sprite::HitTest method, for example when more precise action of clicking on an object is necessary.

    Note that this processing can be expensive and has significant memory footprint per texture.

    @see Sprite::HitTest
    */
    bool generateHitMap;
  };

  /**
  Parameters for loading a font.
  */
  struct GameInitParamsFontEntry
  {
    /**
    String to identify the font that will be used to reference it when creating game objects with fonts.

    Must be unique across all loaded fonts.
    */
    std::string assetName;

    /**
    String with full path to the font file relative to the executable.

    For example "assets/Vera.ttf".
    */
    std::string fileLocation;
  };

  /**
  Parameters for loading a sound.
  */
  struct GameInitParamsSoundEntry
  {
    /**
    String to identify the sound that will be used to reference it when playing sounds.

    Must be unique across all loaded sounds.
    */
    std::string assetName;

    /**
    String with full path to the sound file relative to the executable.

    For example "assets/UIClickDistinctShortmono.wav".
    */
    std::string fileLocation;
  };

  /**
  Parameters for loading a cursor.
  */
  struct GameInitParamsCursorEntry
  {
    /**
    String to identify the cursor that will be used to reference it when setting cursor.

    Must be unique across all loaded cursors.
    */
    std::string assetName;

    /**
    String with full path to the cursor texture file relative to the executable.

    For example "assets/cursorGauntlet.png".
    */
    std::string fileLocation;

    /*
    X of the center of cursor, that is, a pixel that "actually does the clicking."

    Also called hot-spot.
    This number is passed to SDL_CreateColorCursor() and is relative to top-left of the texture.
    */
    int32_t     centerX;

    /*
    Y of the center of cursor, that is, a pixel that "actually does the clicking."

    Also called hot-spot.
    This number is passed to SDL_CreateColorCursor() and is relative to top-left of the texture.
    */
    int32_t     centerY;
  };

  /**
  Parameters for loading a sprite-sheet.
  */
  struct GameInitParamsSpriteSheetEntry
  {
    /**
    String to identify the sprite-sheet that will be used to reference it when creating game objects with textures that support sprite-sheets.

    Must be unique across all loaded sprite-sheets.
    */
    std::string assetName;

    /**
    String with full path to the sprite-sheet texture file relative to the executable.

    For example "assets/engineUI.png".
    */
    std::string textureFileLocation;

    /**
    String with full path to the sprite-sheet configuration file relative to the executable.

    For example "assets/engineUI.json".
    */
    std::string sheetJSONFileLocation;
  };

  /**
  Parameters for defining a key-binding.
  */
  struct GameInitParamsKeyBindingEntry
  {
    /**
    String displayed in OptionsMenuScene to represent the key-binding.
    */
    std::string uiDescription;

    /*
    Unique identification for key-binding in the settings file.

    It is recommended to create an enum with settings IDs starting at kSettingsIDS_engineEnd + 0.

    @see SettingsIDs
    */
    int32_t     settingsId;

    /**
    String stored in settings file for this key-binding.

    It is recommended for this to be a brief description of what action the key-binding represent along with following text:
    "Uses internal engine key identifier, see https://wiki.libsdl.org/SDL_Keycode."
    The goal is that the settings file is a human-friendly place should the need ever arise.
    */
    std::string settingsDescription;

    /*
    Default key code for the key-binding.

    Uses SDL2 key codes enumeration of type SDL_Keycode (typedef of int32_t), see SDL_keycode.h.
    */
    int32_t     defaultKey;
  };

  /*
  Parameters for initialization of a new game instance.

  Usage:
  @code
  #include "Game.h"
  #include "GameInitParams.h"

  // Assuming we have filled GameInitParams variable kGameInitParams, see bellow for template
  auto& game = GGame;
  if (game.Initialize(kGameInitParams))
  {
    // Add scenes, play one, start, etc.
  }
  @endcode


  Template:
  @code
  const GameInitParams kGameInitParams = {
    // uint32_t renderingResolutionWidth;
    1280,
    //uint32_t renderingResolutionHeight;
    720,
    //std::string windowName;
    "JadeEngineGame v0.1",
    //std::string appName;
    "JadeEngineGame",
    //std::vector<uint32_t> fontSizes;
    {24, 32},
    //SDL_Color backgroundColor;
    { 12, 16, 24, 32 },
    //std::vector<GameInitParamsTextureEntry> textures;
    {
      { kSomeTextureID,     "assets/someTexture.png", false},
    },
    //std::vector<GameInitParamsFontEntry> fonts;
    {
      { kSomeFontID,        "assets/someFont.ttf" },
    },
    //std::vector<GameInitParamsSoundEntry> sounds;
    {
      { kSomeSoundID,       "assets/someSound.wav"},
    },
    //std::vector<GameInitParamsCursorEntry> cursors;
    {
      { kSomeCursorID,      "assets/someCursor.png", 9, 9 },
    },
    //std::vector<GameInitParamsSpriteSheetEntry> spritesheets;
    {
      { kSomeSpritesheetID, "assets/someSpriteSheet.png", "assets/someSpriteSheet.json" },
    },
    //std::vector<GameInitParamsKeyBindingEntry> keybindings;
    {
      { "Keybinding 1", kCustomSettingsEnum_Keybinding1, "Keybinding for X. Uses internal engine key identifier, see https://wiki.libsdl.org/SDL_Keycode.", SDLK_v},
    },
    //std::string author;
    "Vaclav Honzik",
    //int32_t copyrightYear;
    2019,
  };
  @endcode
  */
  struct GameInitParams
  {
    /**
    Width in pixels of "rendering" resolution that will be used to draw all game objects.

    What is presented on the screen is potentially scaled version of that depending on the user chosen "display" resolution.
    All assets should be targeting this rendering resolution.
    */
    uint32_t renderingResolutionWidth;

    /**
    Height in pixels of "rendering" resolution that will be used to draw all game objects.

    @see GameInitParams::renderingResolutionWidth
    */
    uint32_t renderingResolutionHeight;

    /**
    String displayed in the title of window.

    @see SDL_CreateWindow::title
    */
    std::string windowName;

    /**
    String representing the game name used for the settings file name.

    Will be used in file operations and should be a valid file name for the targeted system.
    Note the settings file location is subject to the jade-engine and extension will appended to it.
    */
    std::string appName;

    /**
    List of font sizes that will be generated for each font in fonts member.

    It is not currently possible to add font sizes during runtime.

    @see GameInitParams::fonts
    */
    std::vector<uint32_t> fontSizes;

    /**
    Background color of the game screen, alpha component is ignored.

    Each rendering frame following operation SDL_SetRenderDrawColor(backgroundColor) -> SDL_RenderClear happen as the very first thing.
    Note that this is RGBA as uint8_t, i.e. 0-255.
    */
    SDL_Color backgroundColor;

    /**
    List of textures to load during game initialization.

    Textures are loaded using SDL2 IMG library, namely IMG_Load() function.

    @see GameInitParamsTextureEntry
    */
    std::vector<GameInitParamsTextureEntry> textures;

    /**
    List of font files to load during game initialization.

    Fonts are generally referenced using the std::string originating in FontStyleEntry::assetName
    and uint32_t size that has been listed in GameInitParams::fontSizes.

    Fonts are loaded using SDL2 TTF library, namely TTF_OpenFont() function.

    @see GameInitParamsFontEntry, GameInitParams::fontSizes
    */
    std::vector<GameInitParamsFontEntry> fonts;

    /**
    List of sound files to load during game initialization.

    Sounds are loaded using SDL2 Mixer library, namely Mix_LoadWAV() function.

    @see GameInitParamsSoundEntry
    */
    std::vector<GameInitParamsSoundEntry> sounds;

    /**
    List of cursor texture files to load during game initialization and create cursors out of them.

    Cursors are loaded using SDL2 IMG library, namely IMG_Load() function and then passed to SDL2 SDL_CreateColorCursor().

    @see GameInitParamsCursorEntry
    */
    std::vector<GameInitParamsCursorEntry> cursors;

    /**
    List of sprite-sheets to load during game initialization.

    Sprite-sheets are collection of textures packed into one texture with corresponding configuration file specifying the layout.
    jade-engine can read configuration JSON files produced by free version <a href="https://www.codeandweb.com/texturepacker">TexturePacker</a>.

    The texture parts are loaded using SDL2 IMG library, namely IMG_Load() function.

    @see GameInitParamsSpriteSheetEntry
    */
    std::vector<GameInitParamsSpriteSheetEntry> spritesheets;

    /**
    List of key-bindings that can be used with Input class.

    Key-bindings are stored in settings files to persist across sessions and are displayed and configurable in OptionsMenuScene.

    @see GameInitParamsKeyBindingEntry
    */
    std::vector<GameInitParamsKeyBindingEntry> keybindings;

    /**
    Name of person or organization behind the game.

    Displayed in MainMenuScene inside copyright text.
    */
    std::string author;

    /**
    Year for copyright text displayed in MainMenuScene.
    */
    int32_t copyrightYear;

    /**
    Major version of the game.

    Used together with minor version in settings file to determine whether the settings is compatible.
    Both majorVersion and minorVersion stored in the settings file have to be equal to those passed in GameInitParams otherwise the settings file we be recreated.
    Displayed in the in MainMenuScene.

    @see minorVersion
    */
    int32_t majorVersion;

    /**
    Minor version of the game.

    Used together with major version in settings file to determine whether the settings is compatible.
    Both majorVersion and minorVersion stored in the settings file have to be equal to those passed in GameInitParams otherwise the settings file we be recreated.
    Displayed in the in MainMenuScene.

    @see majorVersion
    */
    int32_t minorVersion;

    /**
    Hash version of the game.

    Displayed in the in MainMenuScene.

    @see majorVersion, minorVersion
    */
    std::string hashVersion;
  };
}
