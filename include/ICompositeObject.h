#pragma once

namespace JadeEngine
{
  /**
  Interface for a composite objects.

  The interface doesn't place any constraints on the implementing object. It only provides Update() callback and basis for showing/hiding the child game objects

  Composite objects are a collection of "primitive" game objects that act as one game object. For example a Button will have collection of Sprites for various pressed states and a Text objects.

  When creating new game objects based on ICompositeObjects it is necessary to create a constructor that accepts only one argument: constant reference to creation structure which will contain at the very least `ObjectLayer layer;` member. See following example:
  @code
  #include "ICompositeObject.h"
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

  class SomeObject : public ICompositeObject
  {
  public:
    SomeObject(const SomeObjectParams& cardParams); // Create _childSprite and other child objects in here
  private:
    Sprite* _childSprite;
  };
  @endcode
  */
  class ICompositeObject
  {
  public:
    /**
    Constructor for the ICompositeObject.
    */
    ICompositeObject();

    /**
    Triggered every frame while the owning scene is active.

    To obtain delta time since last frame use GTime.deltaTime.
    */
    virtual void Update() {};

    /**
    Show or hide the game object.

    @warn This does not shows/hides the owned game objects as this interface has no knowledge of them. It only handles tracking the visibility state. Instead it is recommended to override this function, call this - base - function in the overridden one and handle hiding/showing there.
    @code
    void SomeGameObject::Show(const bool shown) //override
    {
      ICompositeObject::Show(shown);
      _childObject->Show(IsShown());
    }
    @endcode
    */
    virtual void Show(const bool shown) { _shown = shown; }

    /**
    Returns whether the game object is currently shown or hidden.

    @see Show
    @returns true if shown, false otherwise.
    */
    bool IsShown() const { return _shown; }
  protected:
    bool _shown;
  };
}
