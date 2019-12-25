#pragma once

namespace JadeEngine
{
  class Game;
  class Sprite;

  class IScene
  {
  public:
    virtual void Update() {};
    virtual void SpriteHovered(Sprite* oldSprite, Sprite* newSprite) {};
    virtual void Start() {};

    bool GetInitialized() const { return _initialized; }
    void SetInitialized(bool initialized) { _initialized = initialized; }
  private:
    bool _initialized = false;
  };
}