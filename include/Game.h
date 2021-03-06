#pragma once

#include "DisplayModeInfo.h"
#include "EngineDataTypes.h"
#include "EngineResourcesDescriptions.h"
#include "IGameObject.h"
#include "Sprite.h"
#include "Texture.h"

#include <array>
#include <filesystem>
#include <memory>
#include <random>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace JadeEngine::detail;

namespace JadeEngine
{
  class   FTC;
  struct  GameInitParams;
  class   IScene;

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

    Among other things, the current scene receives Update() callbacks and all %game objects created during this scene are rendered.

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

    It is advised to also update Setting::FullScreen settings value if this change is meant to persist across sessions. The actual change happens as the very first thing next game loop tick.

    @param fullscreen Whether the game should be in fullscreen mode or not. Nothing happens if it's currently already in the desired mode.
    @code
      // Assuming we have created a Checkbox _fullScreenCheckbox
      if (_fullScreenCheckbox->Changed())
      {
        GGame.SetFullscreen(_fullScreenCheckbox->Checked());
        GPersistence.SetSettingTyped<Setting::FullScreen>(_fullScreenCheckbox->Checked());
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

    int32_t GetWidth() const { return _renderResolutionWidth; }
    int32_t GetHeight() const { return _renderResolutionHeight; }

    int32_t GetHalfWidth() const { return GetWidth() / 2; }
    int32_t GetHalfHeight() const { return GetHeight() / 2; }

    Vector2D_i32 GetHalfSize() const { return { GetHalfWidth(), GetHalfHeight() }; };
    Vector2D_i32 GetMiddlePoint() const { return GetHalfSize(); };

    const Sprite* GetHoveredSprite() const { return _hoveredSprite; }
    SDL_Renderer* GetRenderer() { return _renderer; }
    bool IsFullscreen() const { return _fullscreen; }

    void SetDisplayMode(const int32_t index);
    const int32_t GetCurrentDisplayMode() const { return _currentMode; }
    const std::vector<DisplayModeInfo>& GetDisplayModes() const { return _displayModes; }


    /**
    Create a new %game object.

    The object must inherit from IGameObject interface.

    The object will belong to the current scene unless kObjectLayer_Persistent_UI is specified in which case it will belong to special persistent scene.

    When creating large amount of objects consider doing so between GGame.StartBatchCreate() and GGame.EndBatchCreate() block.

    @param params Creation structure. For the actual type and its description see the class's constructor or header.
    @returns Pointer to the newly created %game object. Game instance owns the %game object but it might not possible to look it up later. Storing the pointer is advised.
    @see IGameObject, Text, TextParams, FTC, FTCParams, Button, ButtonParams, Checkbox, CheckboxParams, Dropdown, DropdownParams, Slider, SliderParams, Sprite, SpriteParams, BoxSprite, BoxSpriteParams
    @code
    // Assuming we have filled `TextParams textParams` variable
    Text* text = GGame.Create<Text>(textParams);
    text->SetPosition(50, 50);
    @endcode
    */
    template<typename Class, typename CreationStruct>
    std::add_pointer_t<Class> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_Persistent_UI ? _persistentScene : _currentScene;
      auto result = _gameObjects[scene].emplace_back(std::move(std::make_unique<Class>(params))).get();

      // Some objects benefit from being loaded immediately in order to be positioned correctly in the same frame they were created
      if (result->GetLoadState() == kLoadState_Wanted)
      {
        result->SetLoadState(result->Load(_renderer));
      }

      if constexpr (std::is_base_of_v<Sprite, Class>)
      {
        _sprites.insert(result);
      }

      if (!_batchCreate) SortGameObjectsRendering(scene);
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
    @warning Only useful when creating a complex custom text-based %game objects and should be seldom used. For usage see existing text objects such as Text.
    */
    TTF_Font* FindFont(const std::string& fontName, const uint32_t size) const;

    /**
    Find Texture instance given a texture name.
    @param textureName The texture identification string as defined in GameInitParamsTextureEntry when initializing the game.
    @returns Found Texture instance or nullptr if not found.
    @pre GGame was Initialize() with the wanted texture.
    @see GameInitParamsFontEntry, GameInitParams, Sprite
    @warning Only useful when creating a complex custom texture-based %game objects and should be seldom used. For usage see Sprite.
    */
    std::shared_ptr<Texture> FindTexture(const std::string& textureName) const;

    /**
    Create a new deep copy of Texture. Note that it is not possible to look-up this new texture later hence the return value should be captured.

    Used for tinting and setting transparency of sprites as an unique instance is necessary otherwise such operations would change all sprites using this texture.

    @param textureDesc Existing texture that can be obtained with Game::FindTexture.
    @param sampling Wanted texture sampling for the new texture.
    @returns A new texture, deep copy of the passed texture.
    @see FindTexture, TextureSampling
    @warning Potentially expensive operation including render targets.
    @warning Instead of working with textures it is recommend to use existing Sprite-based classes. Should be seldom used.
    */
    std::shared_ptr<Texture> CopyTexture(const std::shared_ptr<Texture>& textureDesc, const TextureSampling sampling);

    void DestroyCopyTexture(SDL_Texture* texture);

  private:
    std::string AssetPathToAbsolute(const char* assetName);
    void CollectDisplayModes();
    bool CreateSolidColorTexture(const std::string& name, const int32_t width, const int32_t height, const SDL_Color& color);
    void DestroyGameObjects();
    void GetBoundingBoxAndHitArray(SDL_Surface* surface, Rectangle& boundingBox, std::vector<bool>& hitArray, bool hitsRequired);
    uint32_t GetPixel(SDL_Surface* surface, int32_t x, int32_t y);
    std::string HashSolidColorTexture(const uint32_t width, const uint32_t height, const SDL_Color& color);
    void RegisterKeybindings(const GameInitParams& initParams);
    Sprite* GameObjectToSprite(IGameObject* gameObject);
    bool LoadAssets(const GameInitParams& initParams);
    bool LoadCursor(const char* assetName, const char* textureFile, int32_t centerX, int32_t centerY);
    bool LoadFont(const std::vector<uint32_t>& sizes, const char* assetName, const char* fontFile);
    bool LoadSpritesheet(const char* assetName, const char* textureFile, const char* sheetFile, const TextureSampling sampling);
    bool LoadTexture(const char* assetName, const char* textureFile, const bool hitsRequired, const TextureSampling sampling);
    void PlayScene(std::shared_ptr<IScene>& scene);
    void RenderGameObjects(std::shared_ptr<IScene>& scene);
    void SetHoveredSprite(Sprite* sprite);
    void SortGameObjectsRendering(const std::shared_ptr<IScene>& scene);
    void Update();
    void LoadGameObjects(std::shared_ptr<IScene>& scene);
    void HoverSprites(std::shared_ptr<IScene>& scene);
    void UpdateGameObjects(std::shared_ptr<IScene>& scene);
    void UpdateGameObjectsTransforms(std::shared_ptr<IScene>& scene);
    void UpdateKeybindings();
    void DestroyGameObject(IGameObject* gameObject);

    SDL_Window* _window;
    SDL_Renderer* _renderer;
    uint32_t _nativeTextureFormats;
    SDL_Texture* _nativeRenderBuffer;

    std::shared_ptr<IScene> _currentScene;
    std::shared_ptr<IScene> _persistentScene;
    std::unordered_map<int32_t, std::shared_ptr<IScene>> _scenes;

    std::unordered_map<std::shared_ptr<IScene>, std::vector<std::unique_ptr<IGameObject>>> _gameObjects;
    std::unordered_set<IGameObject*> _sprites;

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

    Rectangle _scaledBufferRect;
    Rectangle _windowBufferRect;

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
