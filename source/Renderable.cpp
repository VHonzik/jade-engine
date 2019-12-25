#include "Renderable.h"

#include "ITextObject.h"
#include "LineStrip.h"
#include "Sprite.h"

namespace JadeEngine
{

  template<>
  Renderable make_renderable<Sprite>(Sprite* object)
  {
    return
    {
      static_cast<void*>(object),
      object->GetZ(),
      [](void* sprite, SDL_Renderer* renderer) { static_cast<Sprite*>(sprite)->Render(renderer); }
    };
  }

  template<>
  Renderable make_renderable<ITextObject>(ITextObject* object)
  {
    return
    {
      static_cast<void*>(object),
      object->GetZ(),
      [](void* textObject, SDL_Renderer* renderer) { static_cast<ITextObject*>(textObject)->Render(renderer); }
    };
  }

  template<>
  Renderable make_renderable<LineStrip>(LineStrip* object)
  {
    return
    {
      static_cast<void*>(object),
      object->GetZ(),
      [](void* lineObject, SDL_Renderer* renderer) { static_cast<LineStrip*>(lineObject)->Render(renderer); }
    };
  }
}