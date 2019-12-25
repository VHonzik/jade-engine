#include "ITextObject.h"

namespace JadeEngine
{
  ITextObject::ITextObject()
    : _preloaded(false)
    , _shown(true)
    , _z(0)
  {
  }

  void ITextObject::Preload(SDL_Renderer* renderer)
  {
    _preloaded = DoPreload(renderer);
  }

  void ITextObject::Render(SDL_Renderer* renderer)
  {
    if (_shown)
    {
      DoRender(renderer);
    }
  }
}