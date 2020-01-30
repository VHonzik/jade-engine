#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "SampleConstants.h"
#include "Vector.h"

using namespace JadeEngine;

namespace JadeEngine
{
  class Sprite;
}

namespace MatchThree
{
  struct PieceParams
  {
    ObjectLayer layer;
    PieceType   type;
    int32_t     z;
    int32_t     width;
    int32_t     height;
  };

  class Piece : public IGameObject
  {
  public:
    Piece(const PieceParams& params);
    void Update() override;
    void Show(const bool shown) override;
    void Clean() override;
    void Deselect();
    void EnableInput(const bool enabled);

    bool IsSelected() const { return _selected; }
    bool IsHovered() const { return _hovered; }
    bool IsMoving() const { return _moving; }
    bool IsFlashing() const { return _flashingError || _flashingMatch; }
    bool IsFlashingMatchDone() const { return _flashingMatchDone; }

    void MoveCenterTo(const Vector& pos, const float speed);
    void TeleportCenterTo(const Vector& pos);

    void SetZ(const int32_t z) { _z = z; }

    void FlashError();
    void FlashMatch();
    PieceType GetType() const { return _type; }

  private:
    void UpdatePosition();

    PieceType _type;

    Sprite* _symbol;
    Sprite* _background;
    Sprite* _backgroundHovered;
    Sprite* _backgroundSelected;
    Sprite* _backgroundSelectedHovered;
    Sprite* _backgroundError;
    Sprite* _backgroundMatched;

    bool _hovered;
    bool _selected;
    bool _inputEnabled;

    Vector _wantedCenter;

    float _movingCenterX;
    float _movingCenterY;
    bool _moving;
    float _movingSpeed;

    bool _flashingError;
    float _errorTimer;

    bool _flashingMatch;
    float _symbolScalingTimer;
    bool _flashingMatchDone;
  };
}