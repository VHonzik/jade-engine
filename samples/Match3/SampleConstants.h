#pragma once

#include "EngineConstants.h"
#include "Vector.h"

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

  struct MatchInfo
  {
    PieceType type;
    Vector    position;
  };

  const char kMatchThreeSpritesheet[] = "matchThree";
}