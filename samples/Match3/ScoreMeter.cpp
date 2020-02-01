#include "ScoreMeter.h"

#include "EngineConstants.h"
#include "EngineTime.h"
#include "Game.h"
#include "LineStrip.h"
#include "Text.h"
#include "Transform.h"
#include "Utils.h"

#include <cmath>

namespace MatchThree
{
  const std::array<SDL_Color, kPieceType_Count> kPiecesColor = {
    kPieceColor_BlueOctagon   ,
    kPieceColor_GreenCircle   ,
    kPieceColor_OrangeTriangle,
    kPieceColor_PinkHeart     ,
    kPieceColor_RedDiamond    ,
    kPieceColor_WhitePentagon ,
    kPieceColor_YellowStar    ,
  };

  ScoreMeter::ScoreMeter(const ScoreMeterParams& params)
    : _wantedScore(0)
    , _currentScore(0.0f)
    , _displayedScore(0)
    , _layer(params.layer)
  {
    _z = params.z;
    transform->Initialize(0, 0, params.width, params.height);

    LineStripParams lineStripParams;
    lineStripParams.layer = params.layer;
    lineStripParams.color = kLighterDarkGreyColor;
    lineStripParams.z = params.z;

    auto radius = std::min(transform->GetWidth(), transform->GetHeight()) / 2;
    CreateRegularPolygonVertices(lineStripParams.initialPoints, params.sides, radius);

    _fullEnergyBorder = GGame.Create<LineStrip>(lineStripParams);

    radius = radius * 6 / 4;
    CreateRegularPolygonVertices(lineStripParams.initialPoints, params.sides, radius);
    _thresholdEnergyBorder = GGame.Create<LineStrip>(lineStripParams);
    transform->Attach(_fullEnergyBorder->transform);
    transform->Attach(_thresholdEnergyBorder->transform);

    TextParams textParams;
    textParams.layer = params.layer;
    textParams.fontName = kVeraFontBold;
    textParams.fontSize = 32;
    textParams.text = std::to_string(_displayedScore);
    textParams.color = kLightGreyColor;
    textParams.z = params.z;

    _text = GGame.Create<Text>(textParams);
    _text->SetVerticalAlign(kVerticalAlignment_Center);
    _text->SetHorizontalAlign(kHorizontalAlignment_Center);
    _text->SetPosition(transform->GetCenterX(), transform->GetCenterY());
  }

  void ScoreMeter::CreateRegularPolygonVertices(std::vector<Vector2D_i32>& vertices, const int32_t sides, const int32_t radius)
  {
    const auto offset = -0.75f * 2 * kFPI / sides;
    vertices.clear();
    // To close the loop we need the starting point twice
    vertices.resize(sides + 1);
    for (size_t i = 0; i < vertices.size(); i++) {
      // Repeat starting point as the last element
      const auto polygonVertexIndex = i < vertices.size() - 1 ? i : 0;
      const auto sideAngle = (2 * kFPI * polygonVertexIndex / sides);
      const auto x = transform->GetCenterX() + radius * std::cos(offset + sideAngle);
      const auto y = transform->GetCenterY() + radius * std::sin(offset + sideAngle);
      vertices[i] = { static_cast<int32_t>(x), static_cast<int32_t>(y) };
    }
  }

  void ScoreMeter::Score(const std::vector<MatchInfo>& matches)
  {
    TextParams textParams;
    textParams.layer = _layer;
    textParams.fontName = kVeraFont;
    textParams.fontSize = 12;
    textParams.text = "+1";
    textParams.z = _z + 100;

    for (const auto& match : matches)
    {
      textParams.color = kPiecesColor[match.type];

      FloatingPoint point;
      point.text = GGame.Create<Text>(textParams);
      point.text->SetHorizontalAlign(kHorizontalAlignment_Center);
      point.text->SetVerticalAlign(kVerticalAlignment_Center);
      point.text->SetPosition(match.position.x, match.position.y);
      point.currentPos = match.position;
      point.wantedPos = transform->GetCenterPosition();
      point.speed = 10.0f;
      point.acceleration = 200.0f;
      point.maxSpeed = 1000.0f;

      _floatingPoints.push_back(point);
    }
  }

  void ScoreMeter::UpdateFloatingPoint(FloatingPoint& point)
  {
    point.speed = std::min(point.maxSpeed, point.speed + GTime.deltaTime * point.acceleration);
    const auto moveTowardsResult = MoveTowardsDone(point.currentPos, point.wantedPos, point.speed  *GTime.deltaTime);
    point.currentPos = moveTowardsResult.first;

    Vector2D_i32 currentPos = {};
    currentPos = point.currentPos;

    point.text->SetPosition(currentPos.x, currentPos.y);

    if (moveTowardsResult.second)
    {
      _wantedScore += 1;
      point.text->Destroy();
    }
  }

  void ScoreMeter::Update()
  {
    if (transform->IsDirty(kDirtyFlag_centerPosition))
    {
      _text->SetPosition(transform->GetCenterX(), transform->GetCenterY());
    }

    if (_wantedScore != _displayedScore)
    {
      const auto diff = std::abs(_wantedScore - _currentScore);

      // Fitted to increase the speed linearly with distance
      const auto speed = 1.0f / (0.01817625f * diff + 0.09100367f);
      const auto maxChange = speed * GTime.deltaTime;
      _currentScore = MoveTowards(_currentScore, static_cast<float>(_wantedScore), maxChange);
      const auto displayedScore = static_cast<int32_t>(std::floor(_currentScore));
      if (displayedScore != _displayedScore)
      {
        _displayedScore = displayedScore;
        _text->SetText(std::to_string(static_cast<int32_t>(std::floor(_displayedScore))));
      }
    }

    for (auto& point : _floatingPoints)
    {
      UpdateFloatingPoint(point);
    }

    _floatingPoints.erase(std::remove_if(std::begin(_floatingPoints), std::end(_floatingPoints), [](const FloatingPoint& point) { return point.text->DestructionWanted(); }), std::end(_floatingPoints));
  }

}