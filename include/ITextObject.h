#pragma once

#include "Aligment.h"

#include <functional>

struct SDL_Renderer;

namespace JadeEngine
{
  class ITextObject
  {
  public:
    ITextObject();
    virtual void DoRender(SDL_Renderer* renderer) {};
    virtual bool DoPreload(SDL_Renderer* renderer) { return true; };
    virtual void Update() {};
    virtual bool RequiresPreload() const { return !_preloaded; };
    virtual void Clean() {};

    void Preload(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer);

    virtual void Show(const bool shown) { _shown = shown; }
    bool IsShown() const { return _shown; }

    virtual void SetHorizontalAlign(const HorizontalAlignment align) { _textHorizontalAlign = align; }
    HorizontalAlignment GetHorizontalAlign() const { return _textHorizontalAlign; }

    virtual void SetVerticalAlign(const VerticalAlignment align) { _textVerticalAlign = align; }
    VerticalAlignment GetVerticalAlign() const { return _textVerticalAlign; }

    int32_t GetZ() const { return _z; }

  protected:
    bool _preloaded;
    bool _shown;
    HorizontalAlignment _textHorizontalAlign;
    VerticalAlignment _textVerticalAlign;
    int32_t _z;
  };
}