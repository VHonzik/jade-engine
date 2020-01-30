#pragma once

#include <cstdint>
#include <memory>

struct SDL_Renderer;

namespace JadeEngine
{
  class Transform;

  /**
  Enumeration for state of loading a %game object is currently in.
  */
  enum LoadState
  {
    /**
    The %game object owns resources that require loading.

    It's Load function will be called once every frame until the load state changes. The Update function is suspended.

    @see IGameObject::Load
    */
    kLoadState_wanted,
    /**
    The %game object's resources were successfully loaded or there were no resources to load.

    The object is operating normally, its Update function is called every frame.
    */
    kLoadState_done,

    /**
    There was an error in loading the %game object's resources and the load process was abandoned.

    This usually implies a non-recoverable state and %game object should be cleaned up and destroyed. The Update function is suspended.
    */
    kLoadState_abandoned,
  };

  /**
  Interface for %game objects.

  The most basic building block of Jade Engine, %game objects are objects that live inside a scene and can be updated and rendered every frame.

  Additionally %game objects can be used to compose simple %game objects into more advanced ones. For example a Button %game object will have collection of Sprite %game objects for various pressed states and a Text %game objects.

  When creating new %game objects based on IGameObject it is necessary to create a constructor that accepts only one argument: constant reference to creation structure which will contain at the very least `ObjectLayer layer;` member. See following example:
  @code
  #include "IGameObject.h"
  #include "ObjectLayer.h"

  using namespace JadeEngine;

  namespace JadeEngine
  {
    class Sprite;
  }

  struct SomeObjectParams
  {
    ObjectLayer layer; // Required
    // Other parameters necessary to initialize the _childSprite and other child objects
  };

  class SomeObject : public IGameObject
  {
  public:
    SomeObject(const SomeObjectParams& cardParams); // Create _childSprite and other child objects in here
    void Update() override; // Perform frame logic here
  private:
    Sprite* _childSprite;
  };
  @endcode

  @see IScene, Game::Create
  */
  class IGameObject
  {
  public:
    /**
    Triggered every frame while the scene that owns this %game object is active and the %game object was successfully loaded.

    The trigger order is the following: IScene::PreUpdate -> IGameObject::Load -> IGameObject::Update -> IScene::Update -> IGameObject::Load -> IGameObject::Render.

    To obtain delta time since last frame use GTime.deltaTime.
    @see Time, LoadState, IGameObject::Render, IScene::Update, IScene::PreUpdate
    */
    virtual void Update() {};

    /**
    Triggered when the %game object is about to be destroyed or reset.

    All resources belonging solely to the %game object, such as textures, should be released at this point. The %game object instance itself may or may not be destroyed after this callback.
    */
    virtual void Clean() {};

    /**
    Triggered every frame when GetLoadState function returns `kLoadState_wanted` to allow resources, such as textures, to be loaded.

    The trigger order is the following: IScene::PreUpdate -> IGameObject::Load -> IGameObject::Update -> IScene::Update -> IGameObject::Load -> IGameObject::Render.

    @param renderer The engine's SDL2 renderer that can be used to create resources.
    @returns The function must return the load state that reflects the result of the load operation. It will be used in SetLoadState function by the Game class.

    @see GetLoadState
    */
    virtual LoadState Load(SDL_Renderer* renderer) { return GetLoadState(); }

    /**
    Triggered every frame while the scene that owns this %game object is active, the %game object was successfully loaded and the %game object is shown.

    The trigger order is the following: IScene::PreUpdate -> IGameObject::Load -> IGameObject::Update -> IScene::Update -> IGameObject::Load -> IGameObject::Render.

    @param renderer The engine's SDL2 renderer that should be used to render the %game object.

    @see IGameObject::Update, LoadState, IGameObject::IsShown
    */
    virtual void Render(SDL_Renderer* renderer) {};

    /**
    Return the current load state of the %game object.
    @see LoadState
    */
    LoadState GetLoadState() const { return _loadState; }

    /**
    Set the load state to a new value.
    @see LoadState
    */
    void SetLoadState(const LoadState newState) { _loadState = newState; }

    /**
    Returns whether %game object is currently shown.

    Only shown objects are rendered and have their Render function triggered.

    @see IGameObject::Show, IGameObject::Render
    @returns true if the object is shown, false otherwise.
    */
    bool IsShown() const { return _shown; }

    /**
    Show or hide %game object.

    @warning This does not affect any owned child %game objects as this interface has no knowledge of them. It only handles tracking the visibility state. It is recommended to override this function for %game object that own other %game objects. Inside the overridden function one should call this - base - function and handle hiding/showing of children %game objects there.
    @code
    void SomeGameObject::Show(const bool shown) //override
    {
      IGameObject::Show(shown);
      // Show or hide any child objects or perform any other necessary operations
      _childObject->Show(IsShown());
    }
    @endcode

    @see IGameObject::IsShown
    */
    virtual void Show(const bool show) { _shown = show; }

    /**
    Returns Z coordinate. The %game objects with higher Z coordinate will be drawn over the ones with lower one.
    */
    int32_t GetZ() const { return _z; }

    /**
    Mark the object to be destroyed.

    At the begging of the next frame its Clean function will be called and the %game object will removed from list of game objects.
    */
    void Destroy() { _destructionWanted = true; }

    /**
    Returns whether the object is marked for destruction.
    */
    bool DestructionWanted() const { return _destructionWanted; }

    /**
    Transform of the %game object.
    */
    const std::shared_ptr<Transform> transform = std::make_shared<Transform>();

  protected:
    LoadState _loadState = kLoadState_done;
    bool _shown = true;
    int32_t _z = 0;
  private:
    bool _destructionWanted = false;
  };
}