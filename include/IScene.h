#pragma once

namespace JadeEngine
{
  class Sprite;

  /**
  Interface for Scene classes.

  Scene is a high-level building block of a game in Jade engine. It is a collection of %game objects that are updated and rendered at the same time, and usually contains logic that operates on those %game objects.

  All %game objects created while a scene is active, such as during it's Start() function will be considered part of that scene and will stop being updated and rendered when a new scene is played. Exception to that are objects created with kObjectLayer_persistent_ui ObjectLayer which are always rendered and updated.

  All scenes must inherit from IScene and can override the public virtual functions in order receive callbacks from the Jade engine.

  @see Game::AddScene, Game::PlayScene

  Template for new scene:
  @code
  #include "IScene.h"

  using namespace JadeEngine;

  class GameScene : public IScene
  {
  public:
    void Start() override;
    void Update() override;
  };
  @endcode

  */
  class IScene
  {
  public:
    /**
    Triggered when a new Sprite was hovered over by mouse while this scene is active.

    @param oldSprite Previously hovered Sprite or nullptr if no Sprite was previously hovered.
    @param newSprite Newly hovered Sprite.
    @see Game::GetHoveredSprite
    */
    virtual void SpriteHovered(Sprite* oldSprite, Sprite* newSprite) {};

    /**
    Triggered the first time this scene is played.

    An ideal callback to initialize the scene's %game objects. Only triggered once, i.e. second call to GGame.PlayScene with this scene will not trigger Start(). Start() will be triggered immediately when GGame.PlayScene is called while the first Update() will happen next frame.

    @see Game::PlayScene
    @code
    void GameScene::Start() //override
    {
      // Assuming GameScene has `Button* _button` and `float _timer` members and we have filled `ButtonParams buttonParams` variable.
      _button = GGame.Create<Button>(buttonParams);
      _timer = 5.0f;
    }
    @endcode
    */
    virtual void Start() {};

    /**
    Triggered ever frame by the Jade Engine while this scene is active.

    To obtain delta time since last frame use GTime.deltaTime.

    The trigger order is the following: IScene::PreUpdate -> IGameObject::Load -> IGameObject::Update -> IScene::Update -> IGameObject::Load -> IGameObject::Render.

    @see Game::PlayScene, Time, IGameObject::Update
    */
    virtual void PreUpdate() {};

    /**
    Triggered every frame by the Jade Engine while this scene is active.

    To obtain delta time since last frame use GTime.deltaTime.

    The trigger order is the following: IScene::PreUpdate -> IGameObject::Load -> IGameObject::Update -> IScene::Update -> IGameObject::Load -> IGameObject::Render.

    @see Game::PlayScene, Time, IGameObject::Update
    @code
    #include "Button.h"
    #include "EngineTime.h"

    void GameScene::Update() //override
    {
      // Assuming we have created a Button in the scene's Start() callback
      if (_button->Released())
      {
        // Button was pressed, do something
      }

      // Assuming the scene contains `float _timer` member variable initialized to certain amount of time in seconds
      _timer -= GTime.deltaTime;
      if (_timer < 0.0f)
      {
        // Timer expired, do something
      }
    }
    @endcode

    */
    virtual void Update() {};

    /**
    Returns whether this scene was already initialized, i.e. its Start() was already called.
    */
    bool GetInitialized() const { return _initialized; }

    /**
    Set this scene as initialized, i.e. next GGame.PlayScene will now longer trigger Start().
    */
    void SetInitialized(bool initialized) { _initialized = initialized; }
  private:
    bool _initialized = false;
  };
}