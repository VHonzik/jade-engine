#pragma once

namespace JadeEngine
{
  /**
  Enumeration for layers that %game objects must belong to.
  */
  enum ObjectLayer
  {
    /**
    Game objects that are created in UI layer belong to the currently active scene and are rendered in the screen coordinates.

    The scene that is currently active when the %game object is created will be the owner and therefore the %game object will be only updated and rendered while its active.

    To explain screen coordinates, imagine %game object with X,Y coordinates of 10,10. In UI layer its top-left corner will be 10,10 pixels from top-left corner of the game window.
    */
    kObjectLayer_UI,

    /**
    Game objects that are created in World layer belong to the currently active scene and are rendered in world coordinates.

    The scene that is currently active when the %game object is created will be the owner and therefore the %game object will be only updated and rendered while its active.

    Position of %game objects, placed in world coordinates, on the screen depends on WorldCamera position.

    @see WorldCamera
    */
    kObjectLayer_World,

    /**
    Similarly to `kObjectLayer_UI` %game objects created in `kObjectLayer_Persistent_UI` are rendered in screen coordinates but they belong to singleton persistent scene.

    Game objects in persistent scene are always rendered and updated.
    */
    kObjectLayer_Persistent_UI,
  };
}
