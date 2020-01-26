#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"

using namespace JadeEngine;

namespace JadeEngine
{
  class Sprite;
}

namespace MatchThree
{
  enum PieceType
  {
    kPieceType_BlueOctagon,
    kPieceType_GreenCircle,
    kPieceType_OrangeTriangle,
    kPieceType_PinkHeart,
    kPieceType_RedDiamond,
    kPieceType_WhitePentagon,
    kPieceType_YellowStar,
    kPieceType_Count,
  };

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

    void SetCenterPosition(const int32_t x, const int32_t y);
    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    bool IsSelected() const { return _selected; }
    bool IsHovered() const { return _hovered; }
    bool IsMoving() const { return _moving; }
    bool IsFlashing() const { return _flashingError || _flashingMatch; }
    bool IsFlashingMatchDone() const { return _flashingMatchDone; }

    void MoveCenterTo(const int32_t x, const int32_t y, const float speed);

    void SetZ(const int32_t z) { _z = z; }

    void FlashError();
    void FlashMatch();
    PieceType GetType() const { return _type; }

  private:
    void SetCenterPosition(const int32_t x, const int32_t y, const bool hard);

    PieceType _type;

    int32_t _width;
    int32_t _height;

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

    int32_t _wantedCenterX;
    int32_t _wantedCenterY;

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