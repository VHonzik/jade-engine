#include "Piece.h"

#include "EngineTime.h"
#include "Game.h"
#include "Input.h"
#include "SampleConstants.h"
#include "Sprite.h"
#include "Utils.h"

#include <array>

namespace
{
  const std::array<std::string, MatchThree::kPieceType_Count> kPiecesTextures =
  {
    "tileBlue_42.png",    //BlueOctagon
    "tileGreen_35.png",   //GreenCircle
    "tileOrange_30.png",  //OrangeTriangle
    "tilePink_48.png",    //PinkHeart
    "tileRed_34.png",     //RedDiamond
    "tileGrey_29.png",    //WhitePentagon
    "tileYellow_33.png",  //YellowStar
  };

  const char kBackgroundTileNormal[] = "BackTile_17.png";
  const char kBackgroundTileHover[] = "BackTile_18.png";
  const char kBackgroundTileSelected[] = "BackTile_09.png";
  const char kBackgroundTileSelectedHover[] = "BackTile_10.png";
  const char kBackgroundTileSelectedMatched[] = "BackTile_13.png";
  const char kBackgroundTileError[] = "BackTile_19.png";


  const auto kBackgroundEmptySpacePercentage = 0.50f;
  const auto kMovementSpeed = 200.0f;
  const auto kFlashErrorDuration = 0.5f;
}

namespace MatchThree
{

  Piece::Piece(const PieceParams& params)
    : _hovered(false)
    , _inputEnabled(true)
    , _moving(false)
    , _flashingError(false)
    , _type(params.type)
  {
    SpriteParams spriteParams;
    spriteParams.layer = kObjectLayer_ui;
    spriteParams.textureName = kPiecesTextures[params.type];
    spriteParams.z = params.z;
    spriteParams.spriteSheet = true;
    spriteParams.spriteSheetName = kMatchThreeSpritesheet;

    _symbol = GGame.Create<Sprite>(spriteParams);
    _symbol->SetWidthHeight(static_cast<int32_t>(params.width * kBackgroundEmptySpacePercentage), static_cast<int32_t>(params.height * kBackgroundEmptySpacePercentage));

    spriteParams.z = params.z-1;
    spriteParams.textureName = kBackgroundTileNormal;
    _background = GGame.Create<Sprite>(spriteParams);
    _background->SetWidthHeight(params.width, params.height);

    spriteParams.textureName = kBackgroundTileHover;
    _backgroundHovered = GGame.Create<Sprite>(spriteParams);
    _backgroundHovered->SetWidthHeight(params.width, params.height);

    spriteParams.textureName = kBackgroundTileSelected;
    _backgroundSelected = GGame.Create<Sprite>(spriteParams);
    _backgroundSelected->SetWidthHeight(params.width, params.height);

    spriteParams.textureName = kBackgroundTileSelectedHover;
    _backgroundSelectedHovered = GGame.Create<Sprite>(spriteParams);
    _backgroundSelectedHovered->SetWidthHeight(params.width, params.height);

    spriteParams.textureName = kBackgroundTileError;
    _backgroundError = GGame.Create<Sprite>(spriteParams);
    _backgroundError->SetWidthHeight(params.width, params.height);

    SetCenterPosition(0, 0);
    Show(true);
  }

  void Piece::SetCenterPosition(const int32_t x, const int32_t y, const bool hard)
  {
    if (hard)
    {
      _wantedCenterX = x;
      _wantedCenterY = y;
      _moving = false;
    }

    _symbol->SetCenterPosition(x, y);
    _background->SetCenterPosition(x, y);
    _backgroundHovered->SetCenterPosition(x, y);
    _backgroundSelected->SetCenterPosition(x, y);
    _backgroundSelectedHovered->SetCenterPosition(x, y);
    _backgroundError->SetCenterPosition(x, y);
  }

  void Piece::SetCenterPosition(const int32_t x, const int32_t y)
  {
    SetCenterPosition(x, y, true);
  }

  void Piece::Show(const bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _symbol->Show(isShown);
    _background->Show(                isShown && !_hovered  && !_selected && !_flashingError);
    _backgroundHovered->Show(         isShown && _hovered   && !_selected && !_flashingError);
    _backgroundSelected->Show(        isShown && !_hovered  && _selected  && !_flashingError);
    _backgroundSelectedHovered->Show( isShown && _hovered   && _selected  && !_flashingError);
    _backgroundError->Show(           isShown && _flashingError);
  }

  void Piece::Update()
  {
    if (_moving)
    {
      if (_wantedCenterX != GetCenterX())
      {
        _movingCenterX = MoveTowards(_movingCenterX, static_cast<float>(_wantedCenterX), kMovementSpeed * GTime.deltaTime);
      }

      if (_wantedCenterY != GetCenterY())
      {
        _movingCenterY = MoveTowards(_movingCenterY, static_cast<float>(_wantedCenterY), kMovementSpeed * GTime.deltaTime);
      }

      SetCenterPosition(static_cast<int32_t>(_movingCenterX), static_cast<int32_t>(_movingCenterY), false);

      if (_wantedCenterX == GetCenterX() && _wantedCenterY == GetCenterY())
      {
        _moving = false;
      }
    }

    if (_flashingError)
    {
      _errorTimer -= GTime.deltaTime;
      if (_errorTimer < 0.0f)
      {
        _flashingError = false;
        Show(IsShown());
      }
    }


    if (!_inputEnabled) return;

    const auto hoveredSprite = GGame.GetHoveredSprite();
    const auto hovered = hoveredSprite == _symbol || hoveredSprite == _background || hoveredSprite == _backgroundHovered || hoveredSprite == _backgroundSelected || hoveredSprite == _backgroundSelectedHovered;
    if (hovered != _hovered)
    {
      _hovered = hovered;
      Show(IsShown());
    }

    if (_hovered && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
    {
      _selected = !_selected;
      Show(IsShown());
    }
  }

  void Piece::Deselect()
  {
    _selected = false;
    Show(IsShown());
  }

  void Piece::EnableInput(const bool enabled)
  {
    _inputEnabled = enabled;
  }

  void Piece::MoveCenterTo(const int32_t x, const int32_t y)
  {
    _wantedCenterX = x;
    _wantedCenterY = y;

    _movingCenterX = static_cast<float>(GetCenterX());
    _movingCenterY = static_cast<float>(GetCenterY());

    _moving = true;
  }

  int32_t Piece::GetCenterX() const
  {
    return _symbol->GetCenterX();
  }

  int32_t Piece::GetCenterY() const
  {
    return _symbol->GetCenterY();
  }

  void Piece::FlashError()
  {
    _flashingError = true;
    _errorTimer = kFlashErrorDuration;
    Show(IsShown());
  }
}