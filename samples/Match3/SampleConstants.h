#pragma once

#include "EngineConstants.h"
#include "TypedSetting.h"
#include "Vector2D.h"

#include <array>
#include <string>

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
    Vector2D_i32 position;
    bool initiatedByPlayerSwap;
  };

  const char kMatchThreeSpritesheet[] = "matchThree";

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

  const std::array<SDL_Color, kPieceType_Count> kPiecesColor =
  {
    kPieceColor_BlueOctagon,    //BlueOctagon
    kPieceColor_GreenCircle,    //GreenCircle
    kPieceColor_OrangeTriangle, //OrangeTriangle
    kPieceColor_PinkHeart,      //PinkHeart
    kPieceColor_RedDiamond,     //RedDiamond
    kPieceColor_WhitePentagon,  //WhitePentagon
    kPieceColor_YellowStar,     //YellowStar
  };

  enum class SampleKeybinding : int
  {
    ExitToMenu = static_cast<int>(Setting::EngineEnd)
  };

  template<> struct TypedSetting<SampleKeybinding, SampleKeybinding::ExitToMenu> { using type = int32_t; };
}