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

    @warning Should be a first operation on GGame in the program life cycle.
    @return Whether the initialization was successful. Reasons for failing are generally non-recoverable internal SDL2 libraries failing. Debugging recommended.
    @see GameInitParams
    @mainpage

    @code
    #include "Game.h"
    #include "GameInitParams.h"

    // Assuming we have filled GameInitParams variable kGameInitParams, see GameInitParams for a template
    auto& game = GGame;
    if (game.Initialize(kGameInitParams))
    {
      // Add scenes, play one, Start(), etc.
    }
    game.CleanUp();
    @endcode
    */
    bool Initialize(const GameInitParams& initParams);

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

    /*
      Enter a blocking game-loop and start the game.

      In order to exit the loop call Game::End.
      Will also start GTime ticking.

      @see Game::End
      @pre Game::Initialize was called and was successful.
      @mainpage

      @code
      #include "Game.h"
      #include "GameInitParams.h"

      // Assuming we have filled GameInitParams variable kGameInitParams, see GameInitParams for a template
      auto& game = GGame;
      if (game.Initialize(kGameInitParams))
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

    void AddScene(const std::string& name, const std::shared_ptr<IScene>& scene);
    void PlayScene(std::shared_ptr<IScene>& scene);
    void PlayScene(const std::string& name);
    void End() { _quit = true; }

    void SetFullscreen(const bool fullscreen);
    void SetDisplayMode(const int32_t index);

    bool CreateSolidColorTexture(const std::string& name, const int32_t width,
      const int32_t height, const SDL_Color& color);
    Sprite* CreateSolidColorSprite(const uint32_t width, const uint32_t height,
      const SDL_Color& color, int32_t z, ObjectLayer layer);
    void DestroySprite(Sprite* sprite);

    void SetCursor(const std::string& name);

    bool RandomBool();
    int32_t RandomNumber(int32_t min, int32_t max);
    float RandomNumber(float min, float max);
    float RandomNumber();

    int32_t GetWidth() const { return _renderResolutionWidth; }
    int32_t GetHeight() const { return _renderResolutionHeight; }

    int32_t GetHalfWidth() const { return GetWidth() / 2; }
    int32_t GetHalfHeight() const { return GetHeight() / 2; }

    const Sprite* GetHoveredSprite() const { return _hoveredSprite; }
    SDL_Renderer* GetRenderer() { return _renderer; }
    bool IsFullscreen() const { return _fullscreen; }

    const int32_t GetCurrentDisplayMode() const { return _currentMode; }
    const std::vector<DisplayModeInfo>& GetDisplayModes() const { return _displayModes; }

    template<typename Class, typename CreationStruct>
    std::enable_if_t<std::is_base_of_v<ITextObject, Class>, std::add_pointer_t<Class>> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto result = _textObjects[scene].emplace_back(std::make_unique<Class>(params)).get();
      result->Preload(_renderer);
      _renderables[scene].push_back(make_renderable<ITextObject>(result));
      if (!_batchCreate) SortRenderables(scene);
      return static_cast<std::add_pointer_t<Class>>(result);
    }

    template<typename Class, typename CreationStruct>
    std::enable_if_t<std::is_base_of_v<ICompositeObject, Class>, std::add_pointer_t<Class>> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto& result = _compositeObjects[scene].emplace_back(std::make_unique<Class>(params));
      return static_cast<std::add_pointer_t<Class>>(result.get());
    }

    template<typename Class, typename CreationStruct>
    std::enable_if_t<std::is_base_of_v<Sprite, Class>, std::add_pointer_t<Class>> Create(const CreationStruct& params)
    {
      const auto& scene = params.layer == kObjectLayer_persitant_ui ? _persistentScene : _currentScene;
      auto result = _sprites[scene].emplace_back(std::make_unique<Class>(params)).get();
      _renderables[scene].push_back(make_renderable<Sprite>(result));
      if (!_batchCreate) SortRenderables(scene);
      return static_cast<std::add_pointer_t<Class>>(result);
    }

    template<typename Class, typename CreationStruct>
    std::enable_if_t<std::is_base_of_v<LineStrip, Class>, std::add_pointer_t<Class>> Create(const CreationStruct& params)
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
    @param fontName The font identification string as defined in GameInitParamsFontEntry when initializing the game.
      For fonts included in Jade Engine see EngineDefaultInitParams.h : kDefaultFonts.
    @param size The font size as listed in GameInitParams::fontSizes when initializing the game.
      For font sizes for fonts included in Jade Engine see EngineDefaultInitParams.h : kDefaultFontSizes.
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
    bool LoadAssets(const GameInitParams& initParams);
    bool LoadCursor(const char* assetName, const char* textureFile, int32_t centerX, int32_t centerY);
    bool LoadFont(const Style& style, const char* assetName, const char* fontFile);
    bool LoadSpritesheet(const char* assetName, const char* textureFile, const char* sheetFile);
    bool LoadTexture(const char* assetName, const char* textureFile, bool hitsRequired);

    void InitializeSettings(const std::vector<std::reference_wrapper<const Style>>& styles);

    void UpdateKeybindings();

    uint32_t GetPixel(SDL_Surface* surface, int32_t x, int32_t y);
    void GetBoundingBoxAndHitArray(SDL_Surface* surface,
      SDL_Rect& boundingBox, std::vector<bool>& hitArray, bool hitsRequired);

    void CollectDisplayModes(const Style& style);
    void Update();
    void UpdateTextObject(std::unique_ptr<ITextObject> & textObject);
    void SetHoveredSprite(Sprite* sprite);
    std::string HashSolidColorTexture(const uint32_t width, const uint32_t height,
      const SDL_Color& color);

    void SortRenderables(const std::shared_ptr<IScene>& scene);

    SDL_Window* _window;
    SDL_Renderer* _renderer;
    uint32_t _nativeTextureFormats;
    SDL_Texture* _nativeRenderBuffer;

    std::shared_ptr<IScene> _currentScene;
    std::shared_ptr<IScene> _persistentScene;
    std::unordered_map<std::string, std::shared_ptr<IScene>> _scenes;

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
  };

  extern Game GGame;
}
