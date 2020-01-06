#pragma once

#include "DisplayModeInfo.h"
#include "EngineDataTypes.h"
#include "ICompositeObject.h"
#include "ITextObject.h"
#include "LineStrip.h"
#include "Renderable.h"
#include "Sprite.h"
#include "Texture.h"

#include <array>
#include <filesystem>
#include <memory>
#include <random>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <vector>

namespace JadeEngine
{
  class   Button;
  class   IScene;
  struct  GameInitParams;
  class   FTC;
  class   Text;

  class Game
  {
  public:
    /**
    Default ctor for Game class.
    @warning GGame global static variable was already constructed for you, hence you shouldn't construct new instances of this class.
    */
    Game();

    /**
    Initialize a new game.

    @param initParams Parameters required to initialize the game.
    @param argv Argument vector as passed to the main function.
    @warning Should be a first operation on GGame in the program life cycle.
    @return Whether the initialization was successful. Reasons for failing are generally non-recoverable internal SDL2 libraries failing. Debugging recommended.
    @see GameInitParams

    @code
    #include "Game.h"
    #include "GameInitParams.h"

    using namespace JadeEngine;

    int32_t main(int32_t argc, char* argv[])
    {
      // Assuming we have filled GameInitParams variable kGameInitParams, see GameInitParams for a template
      auto& game = GGame;
      if (game.Initialize(kGameInitParams, argv))
      {
        // Add scenes, play one, Start(), etc.
      }
      game.CleanUp();

      return 0;
    }

    @endcode
    */
    bool Initialize(const GameInitParams& initParams, char* argv[]);

    /**
    Destroy all loaded assets, close any outstanding handles, "quit" internal SDL2 systems.

    Should be last operation on GGame before exiting the program.

    @code
    #include "Game.h"
    #include "GameInitParams.h"

    int32_t main(int32_t argc, char* argv[])
    {
      auto& game = GGame;

      // Initialize the game and start it, see Game::Initialize

      game.CleanUp();
      return 0;
    }
    @endcode
    */
    void CleanUp();

    /**
      Enter a blocking game-loop and start the game.

      In order to exit the loop call Game::End.

      It will also start GTime ticking.

      @see Game::End
      @pre Game::Initialize was called and was successful.

      @code
      #include "Game.h"
      #include "GameInitParams.h"

      // Assuming we have filled GameInitParams variable kGameInitParams, see GameInitParams for a template
      auto& game = GGame;
      if (game.Initialize(kGameInitParams, argv))
      {
        // Add scenes, play one
        game.Start();
      }
      game.CleanUp();
      @endcode

    */
    void Start();

    /**
      Copying not-allowed, use GGame global variable as the single Game instance.
    */
    Game(const Game&) = delete;
    /**
      Copying not-allowed, use GGame global variable as the single Game instance.
    */
    Game& operator=(const Game&) = delete;

    /**
    Add a scene instance to the Game map of scenes.

    Adding scene should generally be done after successful Initialize() and before Start() call.

    @param id Identification of the scene that will serve as a key in the map of scenes. For built-in scene use Scene enum in EngineConstants.h. For new scenes it is recommended to create new enum starting with kScene_JadeEngineScenesEnd to guarantee uniqueness.
    @param scene Scene instance. Game class will make a copy of this shared_ptr so it's not necessary to hold onto it and can be constructed in place.
    @see Game::PlayScene, IScene

    @code
      #include "EngineConstants.h"
      #include "Game.h"
      #include "GameInitParams.h"
      #include "PoweredByJadeEngineScene.h"

      // Assuming we have filled GameInitParams variable kGameInitParams, see GameInitParams for a template
      auto& game = GGame;
      if (game.Initialize(kGameInitParams, argv))
      {
        game.AddScene(kScene_PoweredByJadeEngine, std::make_shared<PoweredByJadeEngineScene>());
        game.PlayScene(kScene_PoweredByJadeEngine);
        game.Start();
      }
    @endcode
    */
    void AddScene(const int32_t id, const std::shared_ptr<IScene>& scene);

    /**
    Play a previously added scene, making it the current scene.

    Among other things, the current scene receives Update() callbacks and all game objects created during this scene are rendered.

    If the scene is being played for the first time its Start() callback is triggered.

    Nothing happens if a scene that was not previously added is played.

    @param name Scene identification number as specified in Game::AddScene.
    @see IScene, Game::AddScene
    @pre The scene was added using Game::AddScene.
    @code
      #include "EngineConstants.h"
      #include "Game.h"
      #include "GameInitParams.h"
      #include "PoweredByJadeEngineScene.h"

      // Assuming we have filled GameInitParams variable kGameInitParams, see GameInitParams for a template
      auto& game = GGame;
      if (game.Initialize(kGameInitParams, argv))
      {
        game.AddScene(kScene_PoweredByJadeEngine, std::make_shared<PoweredByJadeEngineScene>());
        game.PlayScene(kScene_PoweredByJadeEngine);
        game.Start();
      }
    @endcode
    */
    void PlayScene(const int32_t id);

    /**
    Request the game loop to finish and stop updating. Typically used when exiting the game.

    The game loop will exit at the beginning of the next tick. All updates and rendering will stop. However nothing gets cleaned up for that use Game::CleanUp.

    @see Game::Start, Game::CleanUp
    @code
      // Assuming we have created a Button _quitButton
      if (_quitButton->Released())
      {
        GGame.End();
      }
    @endcode
    */
    void End() { _quit = true; }

    /**
    Signal the game to switch to fullscreen or windowed mode.

    It is advised to also update kSettingsIDs_fullScreen settings value if this change is meant to persist across sessions. The actually change happens as the very first thing next game loop tick.

    @param fullscreen Whether the game should be in fullscreen mode or not. Nothing happens if it's currently already in the desired mode.
    @code
      // Assuming we have created a Checkbox _fullScreenCheckbox
      if (_fullScreenCheckbox->Changed())
      {
        GGame.SetFullscreen(_fullScreenCheckbox->Checked());
        GSettings.Set(kSettingsIDs_fullScreen, _fullScreenCheckbox->Checked());
      }
    @endcode
    */
    void SetFullscreen(const bool fullscreen);

    /**
    Return a random boolean value.

    Internally uses std::mt19937 and std::uniform_int_distribution.
    */
    bool RandomBool();

    /**
    Returns a random integer value.

    Internally uses std::mt19937 and std::uniform_int_distribution.

    @param min Minimum value of random number. The potential random value includes this minimum.
    @param max Maximum value of random number. The potential random value includes this maximum.
    */
    int32_t RandomNumber(int32_t min, int32_t max);

    /**
    Returns a random floating value.

    Internally uses std::mt19937 and std::uniform_real_distribution.

    @param min Minimum value of random number. The potential random value includes this minimum.
    @param max Maximum value of random number. The potential random value includes this maximum.
    */
    float RandomNumber(float min, float max);

    /**
    Returns a random floating value between 0.0f and 1.0f (including both values).

    Internally uses std::mt19937 and std::uniform_real_distribution.
    */
    float RandomNumber01();

    /**
    Set a cursor as an active one.

    @pre The cursor has been added in Game::Initialize.
    @see Game::Initialize, GameInitParams::cursors
    @param name Cursor name as specified in GameInitParamsCursorEntry::assetName
    */
    void SetCursor(const std::string& name);

    Sprite* CreateSolidColorSprite(const uint32_t width, const uint32_t height, const SDL_Color& color, int32_t z, ObjectLayer layer);
    void DestroySprite(Sprite* sprite);

    int32_t GetWidth() const { return _renderResolutionWidth; }
    int32_t GetHeight() const { return _renderResolutionHeight; }

    int32_t GetHalfWidth() const { return GetWidth() / 2; }
    int32_t GetHalfHeight() const { return GetHeight() / 2; }

    const Sprite* GetHoveredSprite() const { return _hoveredSprite; }
    SDL_Renderer* GetRenderer() { return _renderer; }
    bool IsFullscreen() const { return _fullscreen; }

    void SetDisplayMode(const int32_t index);
    const int32_t GetCurrentDisplayMode() const { return _currentMode; }
    const std::vector<DisplayModeInfo>& GetDisplayModes() const { return _displayModes; }

    template <typename  Class>
    using EnableOnlyForITextObjectAndDerivedClasses = std::enable_if_t<std::is_base_of_v<ITextObject, Class>, std::add_pointer_t<Class>>;
    template <typename  Class>
    using EnableOnlyForICompositeObjectAndDerivedClasses = std::enable_if_t<std::is_base_of_v<ICompositeObject, Class>, std::add_pointer_t<Class>>;
    template <typename  Class>
    using EnableOnlyForSpriteAndDerivedClasses = std::enable_if_t<std::is_base_of_v<Sprite, Class>, std::add_pointer_t<Class>>;
    template <typename  Class>
    using EnableOnlyForLineStripAndDerivedClasses = std::enable_if_t<std::is_base_of_v<LineStrip, Class>, std::add_pointer_t<Class>>;

    /**
    Create a text based game object.

    The object must inherit from ITextObject interface. The object will belong to the current scene unless kObjectLayer_persitant_ui is specified.

    When creating large amount of objects consider doing so between GGame.StartBatchCreate() and GGame.EndBatchCreate() block.

    @param params Creation structure. For the actual type and its description see the class's constructor or header.
    @returns Pointer to the newly created object. Game instance owns the object but it might not possible to look it up later. Storing the pointer is advised.
    @see ITextObject, Text, TextParams, FTC, FTCParams
    @code
    // Assuming we have filled `TextParams textParams` variable
    Text* text = GGame.Create<Text>(textParams);
    text->SetPosition(50, 50);
    @endcode
    */
    template<typename Class, typename CreationStruct>
    EnableOnlyForITextObjectAndDerivedClasses<Class> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto result = _textObjects[scene].emplace_back(std::make_unique<Class>(params)).get();
      result->Preload(_renderer);
      _renderables[scene].push_back(make_renderable<ITextObject>(result));
      if (!_batchCreate) SortRenderables(scene);
      return static_cast<std::add_pointer_t<Class>>(result);
    }

    /**
    Create a composite game object.

    The object must inherit from ICompositeObject interface. The object will belong to the current scene unless kObjectLayer_persitant_ui is specified.

    When creating large amount of objects consider doing so between GGame.StartBatchCreate() and GGame.EndBatchCreate() block.

    @param params Creation structure. For the actual type and its description see the class's constructor or header.
    @returns Pointer to the newly created object. Game instance owns the object but it might not possible to look it up later. Storing the pointer is advised.
    @see ICompositeObject, Button, ButtonParams, Checkbox, CheckboxParams, Dropdown, DropdownParams, Slider, SliderParams
    @code
    // Assuming we have filled `ButtonParams buttonParams` variable
    Button* button = GGame.Create<Button>(buttonParams);
    button->SetPosition(50, 50);
    @endcode
    */
    template<typename Class, typename CreationStruct>
    EnableOnlyForICompositeObjectAndDerivedClasses<Class> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto& result = _compositeObjects[scene].emplace_back(std::make_unique<Class>(params));
      return static_cast<std::add_pointer_t<Class>>(result.get());
    }

    /**
    Create a sprite based game object.

    The object must inherit from Sprite class. The object will belong to the current scene unless kObjectLayer_persitant_ui is specified.

    When creating large amount of objects consider doing so between GGame.StartBatchCreate() and GGame.EndBatchCreate() block.

    @param params Creation structure. For the actual type and its description see the class's constructor or header.
    @returns Pointer to the newly created object. Game instance owns the object but it might not possible to look it up later. Storing the pointer is advised.
    @see Sprite, SpriteParams, BoxSprite, BoxSpriteParams
    @code
    // Assuming we have filled `SpriteParams spriteParams` variable
    Sprite* sprite = GGame.Create<Sprite>(spriteParams);
    sprite->SetPosition(50, 50);
    @endcode
    */
    template<typename Class, typename CreationStruct>
    EnableOnlyForSpriteAndDerivedClasses<Class> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto result = _sprites[scene].emplace_back(std::make_unique<Class>(params)).get();
      _renderables[scene].push_back(make_renderable<Sprite>(result));
      if (!_batchCreate) SortRenderables(scene);
      return static_cast<std::add_pointer_t<Class>>(result);
    }

    template<typename Class, typename CreationStruct>
    EnableOnlyForLineStripAndDerivedClasses<Class> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto result = _lineStrips[scene].emplace_back(std::make_unique<Class>(params)).get();
      _renderables[scene].push_back(make_renderable<LineStrip>(result));
      if (!_batchCreate) SortRenderables(scene);
      return static_cast<std::add_pointer_t<Class>>(result);
    }

    void StartBatchCreate() { _batchCreate = true; }
    void EndBatchCreate();

    const SpriteSheetDescription* GetSpriteSheetDescription(const std::string& name)
    {
      const auto result = _spriteSheets.find(name);
      return result != std::end(_spriteSheets) ? &result->second : nullptr;
    }

    void SetKeybinding(const int32_t settingsId, const int32_t newValue);

    const std::unordered_map<int32_t, KeyBindingDescription>& GetKeyBindings() const { return _keybindings; }
    int32_t GeyKeyFromKeybind(const int32_t keybindSettingId);

    int32_t            GetMajorVersion()  const { return _majorVersion; }
    int32_t            GetMinorVersion()  const { return _minorVersion; }
    const std::string& GetHashVersion()   const { return _hashVersion; }
    int32_t            GetCopyrightYear() const { return _copyrightYear; }
    const std::string& GetAuthor()        const { return _author; }

    void SetVersion(const int32_t major, const int32_t minor, const std::string& hash);
    void SetCopyrightYear(const int32_t year);
    void SetAuthor(const std::string& author);

    uint32_t GetNativeTextureFormats() const { return _nativeTextureFormats; };

    /**
    Find underlying SDL2 TTF font instance given a font name and a size.
    @param fontName The font identification string as defined in GameInitParamsFontEntry when initializing the game. For fonts included in Jade Engine see EngineDefaultInitParams.h : kDefaultFonts.
    @param size The font size as listed in GameInitParams::fontSizes when initializing the game. For font sizes for fonts included in Jade Engine see EngineDefaultInitParams.h : kDefaultFontSizes.
    @returns Found underlying SDL2 TTF_Font opaque pointer or nullptr if not found.
    @pre GGame was Initialize() with the wanted font or in is part of default assets.
    @see GameInitParamsFontEntry, GameInitParams, Text
    @warning Only useful when creating a complex custom text-based game objects and should be seldom used. For usage see existing text objects such as Text.
    */
    TTF_Font* FindFont(const std::string& fontName, const uint32_t size) const;

    /**
    Find Texture instance given a texture name.
    @param textureName The texture identification string as defined in GameInitParamsTextureEntry when initializing the game.
    @returns Found Texture instance or nullptr if not found.
    @pre GGame was Initialize() with the wanted texture.
    @see GameInitParamsFontEntry, GameInitParams, Sprite
    @warning Only useful when creating a complex custom texture-based game objects and should be seldom used. For usage see Sprite.
    */
    std::shared_ptr<Texture> FindTexture(const std::string& textureName) const;

    /**
    Create a new deep copy of Texture. Note that it is not possible to look-up this new texture later hence the return value should be captured.

    Used for tinting and setting transparency of sprites as an unique instance is necessary otherwise such operations would change all sprites using this texture.

    @param textureDesc Existing texture that can be obtained with Game::FindTexture.
    @returns A new texture, deep copy of the passed texture.
    @see FindTexture
    @warning Potentially expensive operation including render targets.
    @warning Instead of working with textures it is recommend to use existing Sprite-based classes. Should be seldom used.
    */
    std::shared_ptr<Texture> CopyTexture(const std::shared_ptr<Texture>& textureDesc);

  private:
    void CollectDisplayModes();
    bool CreateSolidColorTexture(const std::string& name, const int32_t width, const int32_t height, const SDL_Color& color);
    void GetBoundingBoxAndHitArray(SDL_Surface* surface, SDL_Rect& boundingBox, std::vector<bool>& hitArray, bool hitsRequired);
    uint32_t GetPixel(SDL_Surface* surface, int32_t x, int32_t y);
    std::string HashSolidColorTexture(const uint32_t width, const uint32_t height, const SDL_Color& color);
    void InitializeSettings(const GameInitParams& initParams);
    bool LoadAssets(const GameInitParams& initParams);
    bool LoadCursor(const char* assetName, const char* textureFile, int32_t centerX, int32_t centerY);
    bool LoadFont(const std::vector<uint32_t>& sizes, const char* assetName, const char* fontFile);
    bool LoadSpritesheet(const char* assetName, const char* textureFile, const char* sheetFile);
    bool LoadTexture(const char* assetName, const char* textureFile, bool hitsRequired);
    void PlayScene(std::shared_ptr<IScene>& scene);
    void SetHoveredSprite(Sprite* sprite);
    void SortRenderables(const std::shared_ptr<IScene>& scene);
    void Update();
    void UpdateKeybindings();
    void UpdateTextObject(std::unique_ptr<ITextObject> & textObject);
    std::string AssetPathToAbsolute(const char* assetName);

    SDL_Window* _window;
    SDL_Renderer* _renderer;
    uint32_t _nativeTextureFormats;
    SDL_Texture* _nativeRenderBuffer;

    std::shared_ptr<IScene> _currentScene;
    std::shared_ptr<IScene> _persistentScene;
    std::unordered_map<int32_t, std::shared_ptr<IScene>> _scenes;

    std::unordered_map<std::shared_ptr<IScene>, std::vector<std::unique_ptr<ITextObject>>> _textObjects;
    std::unordered_map<std::shared_ptr<IScene>, std::vector<std::unique_ptr<Sprite>>> _sprites;
    std::unordered_map<std::shared_ptr<IScene>, std::vector<std::unique_ptr<ICompositeObject>>> _compositeObjects;
    std::unordered_map<std::shared_ptr<IScene>, std::vector<std::unique_ptr<LineStrip>>> _lineStrips;

    std::unordered_map<std::shared_ptr<IScene>, std::vector<Renderable>> _renderables;

    std::unordered_map<std::string, FontDescription> _fonts;
    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
    std::vector<std::shared_ptr<Texture>> _textureCopies;
    std::unordered_map<std::string, CursorDescription> _cursors;
    std::unordered_map<std::string, SpriteSheetDescription> _spriteSheets;

    std::vector<Sprite*> _possibleSprites;
    Sprite* _hoveredSprite;

    SDL_Color _clearColor;

    FTC* _fpsText;
    float _fpsTimer;
    int _fpsCount;

    std::mt19937 _re;

    bool _quit;

    int32_t _renderResolutionWidth;
    int32_t _renderResolutionHeight;

    SDL_Rect _scaledBufferRect;
    SDL_Rect _windowBufferRect;

    bool _fullscreen;
    bool _fullscreenChangedWanted;

    int32_t _currentMode;
    std::vector<DisplayModeInfo> _displayModes;

    std::unordered_map<int32_t, KeyBindingDescription> _keybindings;

    bool _batchCreate;

    int32_t     _majorVersion;
    int32_t     _minorVersion;
    std::string _hashVersion;

    std::string _author;
    int32_t     _copyrightYear;

    std::filesystem::path _executablePath;
  };

  extern Game GGame;
}
