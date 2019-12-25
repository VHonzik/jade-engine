#include "Input.h"

#include "Camera.h"
#include "Game.h"
#include "Utils.h"

#include <unordered_map>
#include <cctype>

namespace
{
  const std::unordered_map<SDL_Keycode, std::string> kModifierKeyNames =
  {
    { SDLK_LCTRL, "LControl"},
    { SDLK_LSHIFT, "LShift"},
    { SDLK_LALT, "LAlt"},
    { SDLK_LGUI, "LGui"},
    { SDLK_RCTRL, "RControl"},
    { SDLK_RSHIFT, "RShift"},
    { SDLK_RALT, "RAlt"},
    { SDLK_RGUI, "RGui"},
    { SDLK_CAPSLOCK, "CapsLock"},
    { SDLK_NUMLOCKCLEAR, "NumLock"},
    { SDLK_MODE, "Mode"},
    { SDLK_SPACE, "Space" },
    { SDLK_F1, "F1" },
    { SDLK_F2, "F2" },
    { SDLK_F3, "F3" },
    { SDLK_F4, "F4" },
    { SDLK_F5, "F5" },
    { SDLK_F6, "F6" },
    { SDLK_F7, "F7" },
    { SDLK_F8, "F8" },
    { SDLK_F9, "F9" },
    { SDLK_F10, "F10" },
    { SDLK_F11, "F11" },
    { SDLK_F12, "F12" },

    { SDLK_RETURN, "Enter" },
    { SDLK_BACKSPACE, "Backspace" },
    { SDLK_APPLICATION, "App" },
    { SDLK_INSERT, "Insert" },
    { SDLK_HOME, "Home" },
    { SDLK_DELETE, "Delete" },
    { SDLK_END, "End" },
    { SDLK_PAGEUP, "PageUp" },
    { SDLK_PAGEDOWN, "PageDown" },

    { SDLK_RIGHT, "Right" },
    { SDLK_LEFT, "Left" },
    { SDLK_DOWN, "Down" },
    { SDLK_UP, "Up" },

    { SDLK_KP_ENTER, "KEnter" },
    { SDLK_KP_DIVIDE, "KDivide" },
    { SDLK_KP_MULTIPLY, "KMultiply" },
    { SDLK_KP_MINUS, "KMinus" },
    { SDLK_KP_PLUS, "KPlus" },
    { SDLK_KP_1, "K1" },
    { SDLK_KP_2, "K2" },
    { SDLK_KP_3, "K3" },
    { SDLK_KP_4, "K4" },
    { SDLK_KP_5, "K5" },
    { SDLK_CLEAR, "K5" }, // Not sure if SDL bug
    { SDLK_KP_6, "K6" },
    { SDLK_KP_7, "K7" },
    { SDLK_KP_8, "K8" },
    { SDLK_KP_9, "K9" },
    { SDLK_KP_0, "K0" },
    { SDLK_KP_PERIOD, "KPeriod" },

    { SDLK_PRINTSCREEN, "PrintScreen" },
    { SDLK_SCROLLLOCK, "ScrollLock" },
    { SDLK_PAUSE, "Pause" },
  };
}

namespace JadeEngine
{
  Input GInput;

  Input::Input()
    : _firstKeyPressed(SDLK_UNKNOWN)
    , _mouseWheelY(0)
  {
  }


  int32_t Input::GetMouseX() const
  {
    return _mouseX;
  }

  int32_t Input::GetMouseY() const
  {
    return _mouseY;
  }

  int32_t Input::GetLastMouseX() const
  {
    return _lastMouseX;
  }

  int32_t Input::GetLastMouseY() const
  {
    return _lastMouseY;
  }

  void Input::ProcessMessage(const SDL_Event& event)
  {
    if (event.type == SDL_KEYDOWN)
    {
      const auto key = event.key.keysym.sym;
      auto found = _keyStates.find(key);
      if (found == _keyStates.end())
      {
        _keyStates[key] = kKeyState_Pressed;
        if (_firstKeyPressed == SDLK_UNKNOWN)
        {
          _firstKeyPressed = key;
        }
      }
      else
      {
        if (found->second == kKeyState_Pressed)
        {
          found->second = kKeyState_Down;
        }
        else if (found->second == kKeyState_Up)
        {
          _keyStates[key] = kKeyState_Pressed;
          if (_firstKeyPressed == SDLK_UNKNOWN)
          {
            _firstKeyPressed = key;
          }
        }
      }
    }
    else if (event.type == SDL_KEYUP)
    {
      const auto key = event.key.keysym.sym;
      _keyStates[key] = kKeyState_Up;
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
      SDL_GetMouseState(&_mouseX, &_mouseY);
      _mouseX = GUICamera.WindowToRenderX(_mouseX);
      _mouseY = GUICamera.WindowToRenderY(_mouseY);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      SDL_GetMouseState(&_mouseX, &_mouseY);

      _mouseX = GUICamera.WindowToRenderX(_mouseX);
      _mouseY = GUICamera.WindowToRenderY(_mouseY);

      const auto button = event.button.button;
      auto found = _mouseButtonStates.find(button);
      if (found == _mouseButtonStates.end())
      {
        _mouseButtonStates[button] = kKeyState_Pressed;
      }
      else
      {
        if (found->second == kKeyState_Pressed)
        {
          found->second = kKeyState_Down;
        }
        else if (found->second == kKeyState_Up)
        {
          _mouseButtonStates[button] = kKeyState_Pressed;
        }
      }
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
      SDL_GetMouseState(&_mouseX, &_mouseY);

      _mouseX = GUICamera.WindowToRenderX(_mouseX);
      _mouseY = GUICamera.WindowToRenderY(_mouseY);

      const auto button = event.button.button;
      _mouseButtonStates[button] = kKeyState_Up;
    }
    else if (event.type == SDL_MOUSEWHEEL)
    {
      SDL_GetMouseState(&_mouseX, &_mouseY);

      _mouseX = GUICamera.WindowToRenderX(_mouseX);
      _mouseY = GUICamera.WindowToRenderY(_mouseY);

      _mouseWheelY = (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? event.wheel.y : -event.wheel.y;
    }
  }

  bool Input::KeyDown(SDL_Keycode key)
  {
    auto found = _keyStates.find(key);
    if (found == _keyStates.end())
    {
      _keyStates[key] = kKeyState_Up;
      return false;
    }
    else
    {
      return found->second > kKeyState_Up;
    }
  }

  bool Input::KeyPressed(SDL_Keycode key)
  {
    auto found = _keyStates.find(key);
    if (found == _keyStates.end())
    {
      _keyStates[key] = kKeyState_Up;
      return false;
    }
    else
    {
      return found->second == kKeyState_Pressed;
    }
  }

  SDL_Keycode Input::FirstKeyPressed() const
  {
    return _firstKeyPressed;
  }

  bool Input::MouseButtonDown(int32_t key)
  {
    auto found = _mouseButtonStates.find(key);
    if (found == _mouseButtonStates.end())
    {
      _mouseButtonStates[key] = kKeyState_Up;
      return false;
    }
    else
    {
      return found->second > kKeyState_Up;
    }
  }

  bool Input::MouseButtonPressed(int32_t key)
  {
    auto found = _mouseButtonStates.find(key);
    if (found == _mouseButtonStates.end())
    {
      _mouseButtonStates[key] = kKeyState_Up;
      return false;
    }
    else
    {
      return found->second == kKeyState_Pressed;
    }
  }

  void Input::Update()
  {
    _firstKeyPressed = SDLK_UNKNOWN;
    _lastMouseX = _mouseX;
    _lastMouseY = _mouseY;
  }

  void Input::AfterUpdate()
  {
    for (auto& keyState : _keyStates)
    {
      if (keyState.second == kKeyState_Pressed)
      {
        keyState.second = kKeyState_Down;
      }
    }

    for (auto& buttonState : _mouseButtonStates)
    {
      if (buttonState.second == kKeyState_Pressed)
      {
        buttonState.second = kKeyState_Down;
      }
    }

    _mouseWheelY = 0;
  }

  std::string Input::GetKeyName(SDL_Keycode key) const
  {
    auto found = kModifierKeyNames.find(key);
    if (found != std::cend(kModifierKeyNames))
    {
      return found->second;
    }

    return std::string(1, std::toupper(key));
  }

  bool Input::KeybindDown(const int32_t keybind)
  {
    const auto key = GGame.GeyKeyFromKeybind(keybind);
    return key != SDLK_UNKNOWN ? KeyDown(key) : false;
  }

  bool Input::KeybindPressed(const int32_t keybind)
  {
    const auto key = GGame.GeyKeyFromKeybind(keybind);
    return key != SDLK_UNKNOWN ? KeyPressed(key) : false;
  }
}