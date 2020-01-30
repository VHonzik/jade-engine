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
  ScoreMeter::ScoreMeter(const ScoreMeterParams& params)
    : _wantedScore(0)
    , _currentScore(0.0f)
    , _displayedScore(0)
  {
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

  void ScoreMeter::CreateRegularPolygonVertices(std::vector<Vector>& vertices, const int32_t sides, const int32_t radius)
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
    for (const auto& match : matches)
    {
      _wantedScore += 1;
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
  }

}