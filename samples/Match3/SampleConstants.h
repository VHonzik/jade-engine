#pragma once

#include "EngineConstants.h"
#include "Vector2D.h"

using namespace JadeEngine;

namespace MatchThree
{
  enum SampleScene
  {
    kSampleScene_GameScene = kScene_JadeEngineScenesEnd,
  };

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

  const SDL_Color kPieceColor_BlueOctagon     = { 57, 177, 228, 255 };
  const SDL_Color kPieceColor_GreenCircle     = { 128, 190, 31, 255 };
  const SDL_Color kPieceColor_OrangeTriangle  = { 232, 106, 23, 255 };
  const SDL_Color kPieceColor_PinkHeart       = { 255, 165, 210, 255 };
  const SDL_Color kPieceColor_RedDiamond      = { 200, 62, 62, 255 };
  const SDL_Color kPieceColor_WhitePentagon   = { 229, 229, 229, 255 };
  const SDL_Color kPieceColor_YellowStar      = { 255, 204, 0, 255 };

  struct MatchInfo
  {
    PieceType type;
    Vector2D_i32    position;
  };

  const char kMatchThreeSpritesheet[] = "matchThree";
}