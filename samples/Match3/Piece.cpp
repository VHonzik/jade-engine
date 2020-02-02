#include "Piece.h"

#include "EngineTime.h"
#include "Game.h"
#include "Input.h"
#include "SampleConstants.h"
#include "Sprite.h"
#include "Transform.h"
#include "Utils.h"

#include <array>

namespace
{
  const char kBackgroundTileNormal[] = "BackTile_17.png";
  const char kBackgroundTileHover[] = "BackTile_18.png";
  const char kBackgroundTileSelected[] = "BackTile_09.png";
  const char kBackgroundTileSelectedHover[] = "BackTile_10.png";
  const char kBackgroundTileSelectedMatched[] = "BackTile_13.png";
  const char kBackgroundTileError[] = "BackTile_19.png";

  const auto kBackgroundEmptySpacePercentage = 0.50f;
  const auto kFlashErrorDuration = 0.5f;
  const auto kFlashMatchSymbolScaleDuration = 0.5f;
  const auto kFlashMatchSymbolScale = 1.5f;
}

namespace MatchThree
{

  Piece::Piece(const PieceParams& params)
    : _hovered(false)
    , _inputEnabled(true)
    , _moving(false)
    , _flashingError(false)
    , _flashingMatch(false)
    , _flashingMatchDone(false)
    , _type(params.type)
    , _movingSpeed(100.0f)
  {
    transform->Initialize(0, 0, params.width, params.height);

    SpriteParams spriteParams;
    spriteParams.layer = kObjectLayer_ui;
    spriteParams.textureName = kPiecesTextures[params.type];
    spriteParams.z = params.z;
    spriteParams.spriteSheet = true;
    spriteParams.spriteSheetName = kMatchThreeSpritesheet;

    _symbol = GGame.Create<Sprite>(spriteParams);
    _symbol->transform->SetSize(static_cast<int32_t>(transform->GetWidth() * kBackgroundEmptySpacePercentage), static_cast<int32_t>(transform->GetHeight() * kBackgroundEmptySpacePercentage));
    transform->Attach(_symbol->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    spriteParams.z = params.z-1;
    spriteParams.textureName = kBackgroundTileNormal;
    _background = GGame.Create<Sprite>(spriteParams);
    _background->transform->SetSize(transform->GetSize());
    transform->Attach(_background->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    spriteParams.textureName = kBackgroundTileHover;
    _backgroundHovered = GGame.Create<Sprite>(spriteParams);
    _backgroundHovered->transform->SetSize(transform->GetSize());
    transform->Attach(_backgroundHovered->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    spriteParams.textureName = kBackgroundTileSelected;
    _backgroundSelected = GGame.Create<Sprite>(spriteParams);
    _backgroundSelected->transform->SetSize(transform->GetSize());
    transform->Attach(_backgroundSelected->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    spriteParams.textureName = kBackgroundTileSelectedHover;
    _backgroundSelectedHovered = GGame.Create<Sprite>(spriteParams);
    _backgroundSelectedHovered->transform->SetSize(transform->GetSize());
    transform->Attach(_backgroundSelectedHovered->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    spriteParams.textureName = kBackgroundTileError;
    _backgroundError = GGame.Create<Sprite>(spriteParams);
    _backgroundError->transform->SetSize(transform->GetSize());
    transform->Attach(_backgroundError->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    spriteParams.textureName = kBackgroundTileSelectedMatched;
    _backgroundMatched = GGame.Create<Sprite>(spriteParams);
    _backgroundMatched->transform->SetSize(transform->GetSize());
    transform->Attach(_backgroundMatched->transform, kZeroVector2D_i32, kAttachmentPoint_Center);

    Show(true);
  }


  void Piece::Show(const bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _symbol->Show(isShown);
    _background->Show(                isShown && !_hovered  && !_selected && !_flashingError && !_flashingMatch);
    _backgroundHovered->Show(         isShown && _hovered   && !_selected && !_flashingError && !_flashingMatch);
    _backgroundSelected->Show(        isShown && !_hovered  && _selected  && !_flashingError && !_flashingMatch);
    _backgroundSelectedHovered->Show( isShown && _hovered   && _selected  && !_flashingError && !_flashingMatch);
    _backgroundError->Show(           isShown && !_flashingMatch && _flashingError);
    _backgroundMatched->Show(         isShown && _flashingMatch);
  }

  void Piece::Update()
  {
    if (_moving)
    {
      if (_wantedCenter != transform->GetCenterPosition())
      {
        _movingCenterX = MoveTowards(_movingCenterX, static_cast<float>(_wantedCenter.x), _movingSpeed * GTime.deltaTime);
        _movingCenterY = MoveTowards(_movingCenterY, static_cast<float>(_wantedCenter.y), _movingSpeed * GTime.deltaTime);
      }

      transform->SetCenterPosition(static_cast<int32_t>(_movingCenterX), static_cast<int32_t>(_movingCenterY));

      if (_wantedCenter == transform->GetCenterPosition())
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

    if (_flashingMatch && _symbolScalingTimer > 0.0f)
    {
      _symbolScalingTimer = Clamp(_symbolScalingTimer - GTime.deltaTime, 0.0f, kFlashMatchSymbolScaleDuration);
      const auto t = (1.0f - _symbolScalingTimer / kFlashMatchSymbolScaleDuration);

      const auto scale = Interpolate(1.0f, kFlashMatchSymbolScale, t);
      const auto wantedWidth = static_cast<int32_t>(transform->GetWidth() * kBackgroundEmptySpacePercentage * scale);
      const auto wantedHeight = static_cast<int32_t>(transform->GetHeight() * kBackgroundEmptySpacePercentage * scale);
      _symbol->transform->SetSize(wantedWidth, wantedHeight);

      if (_symbolScalingTimer <= 0.0f)
      {
        _flashingMatchDone = true;
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

  void Piece::MoveCenterTo(const Vector2D_i32& pos, const float speed)
  {
    _wantedCenter = pos;

    _movingCenterX = static_cast<float>(transform->GetCenterX());
    _movingCenterY = static_cast<float>(transform->GetCenterY());

    _movingSpeed = speed;

    _moving = true;
  }

  void Piece::TeleportCenterTo(const Vector2D_i32& pos)
  {
    _wantedCenter = transform->GetCenterPosition();
    _moving = false;
    transform->SetCenterPosition(pos);
  }

  void Piece::FlashError()
  {
    _flashingError = true;
    _errorTimer = kFlashErrorDuration;
    EnableInput(false);
    Show(IsShown());
  }

  void Piece::FlashMatch()
  {
    _flashingMatch = true;
    _symbolScalingTimer = kFlashMatchSymbolScaleDuration;
    EnableInput(false);
    Show(IsShown());
  }

  void Piece::Clean()
  {
    _symbol->Destroy();
    _background->Destroy();
    _backgroundHovered->Destroy();
    _backgroundSelected->Destroy();
    _backgroundSelectedHovered->Destroy();
    _backgroundError->Destroy();
    _backgroundMatched->Destroy();
  }
}