#pragma once

namespace JadeEngine
{
  /**
  Specifies how is a texture sampled when rendering it in a different scale then 100%.
  */
  enum TextureSampling
  {
    /**
    Nearest pixel sampling. Corresponds to SDL2 "nearest" SDL_HINT_RENDER_SCALE_QUALITY.
    */
    kTextureSampling_neareast,
    /**
    Linear filtering. Corresponds to SDL2 "linear" SDL_HINT_RENDER_SCALE_QUALITY.
    */
    kTextureSampling_linear,
    /**
    Anisotropic filtering. Corresponds to SDL2 "best" SDL_HINT_RENDER_SCALE_QUALITY.
    */
    kTextureSampling_anisotropic
  };
}