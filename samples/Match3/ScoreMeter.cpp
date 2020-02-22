#include "ScoreMeter.h"

#include "EngineConstants.h"
#include "EngineTime.h"
#include "Game.h"
#include "LineStrip.h"
#include "SampleConstants.h"
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"
#include "Utils.h"

#include <cmath>

namespace MatchThree
{
  const float kMinEnergy = 0.0f;
  const float kThresholdEnergy = 60.0f;
  const float kMaxEnergy = 100.f;
  const float kMatchEnergyContribution = 1.0f;

  ScoreMeter::ScoreMeter(const ScoreMeterParams& params)
    : _wantedScore(0)
    , _currentScore(0.0f)
    , _displayedScore(0)
    , _layer(params.layer)
    , _scoreChanged(false)
    , _gameSaveDirty(false)
  {
    _z = params.z;
    transform->Initialize(0, 0, params.width, params.height);

    LineStripParams lineStripParams;
    lineStripParams.layer = _layer;
    lineStripParams.color = kLighterDarkGreyColor;
    lineStripParams.z = _z;

    _maxEnergyDisplayedRadius = static_cast<float>(std::min(transform->GetWidth(), transform->GetHeight()) / 2);
    _thresholdEnergyDisplayedRadius = _maxEnergyDisplayedRadius * 2.0f / 3.0f;
    _minEnergyDisplayedRadius = 30.0f;

    CreateRegularPolygonVertices(lineStripParams.initialPoints, kPieceType_Count, _maxEnergyDisplayedRadius);
    _fullEnergyBorder = GGame.Create<LineStrip>(lineStripParams);

    CreateRegularPolygonVertices(lineStripParams.initialPoints, kPieceType_Count, _thresholdEnergyDisplayedRadius);
    _thresholdEnergyBorder = GGame.Create<LineStrip>(lineStripParams);

    for (size_t i = 0; i < kPieceType_Count; i++)
    {
      lineStripParams.initialPoints = { kZeroVector2D_i32, CreateRegularPolygonVertex(kPieceType_Count, _maxEnergyDisplayedRadius, i) };
      _energyLines[i] = GGame.Create<LineStrip>(lineStripParams);
      transform->Attach(_energyLines[i]->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);
    }

    lineStripParams.z = _z + 1;
    lineStripParams.color = kWhiteColor;
    CreateRegularPolygonVertices(lineStripParams.initialPoints, kPieceType_Count, _minEnergyDisplayedRadius);
    _energySpiderChart = GGame.Create<LineStrip>(lineStripParams);

    transform->Attach(_fullEnergyBorder->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);
    transform->Attach(_thresholdEnergyBorder->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);
    transform->Attach(_energySpiderChart->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    SpriteParams spriteParams;
    spriteParams.layer = _layer;
    spriteParams.z = _z + 2;
    spriteParams.spriteSheet = true;
    spriteParams.spriteSheetName = kMatchThreeSpritesheet;

    for (size_t i = 0; i < kPieceType_Count; i++)
    {
      spriteParams.textureName = kPiecesTextures[i];
      const auto position = CreateRegularPolygonVertex(kPieceType_Count, _maxEnergyDisplayedRadius, i);
      _energySymbols[i] = GGame.Create<Sprite>(spriteParams);
      _energySymbols[i]->transform->SetSize(20, 20);
      transform->Attach(_energySymbols[i]->transform, position, kAnchor_Center, kAnchor_Center);
    }

    TextParams textParams;
    textParams.layer = params.layer;
    textParams.fontName = kVeraFontBold;
    textParams.fontSize = 32;
    textParams.text = std::to_string(_displayedScore);
    textParams.color = kLightGreyColor;
    textParams.z = params.z;

    _text = GGame.Create<Text>(textParams);
    transform->Attach(_text->transform, kZeroVector2D_i32, kAnchor_Center, kAnchor_Center);

    PulseAnimationParams pulseParams;
    pulseParams.expandStartingSpeed = 1.0f;
    pulseParams.expandMinMaxSpeed = 1.2f;
    pulseParams.expandAcceleration = 0.5f;
    pulseParams.contractStartingSpeed = 1.2f;
    pulseParams.contractMinMaxSpeed = 1.0f;
    pulseParams.contractAcceleration = -0.5f;
    pulseParams.relativeFinalSize = 1.5f;
    pulseParams.startImmidiatelly = false;

    _energySymbolsPulse.Initialize(pulseParams);

    std::fill(std::begin(_energy), std::end(_energy), kMinEnergy);

    GPersistence.RegisterGameSaveListener(this);
  }

  Vector2D_i32 ScoreMeter::CreateRegularPolygonVertex(const int32_t sides, const float radius, const size_t index)
  {
    const auto offset = -0.75f * 2 * kFPI / sides;
    const auto sideAngle = (2 * kFPI * index / sides);
    const auto x = radius * std::cos(offset + sideAngle);
    const auto y = radius * std::sin(offset + sideAngle);
    return { static_cast<int32_t>(x), static_cast<int32_t>(y) };
  }

  void ScoreMeter::CreateRegularPolygonVertices(std::vector<Vector2D_i32>& vertices, const int32_t sides, const float radius)
  {
    vertices.clear();
    // To close the loop we need the starting point twice
    vertices.resize(sides + 1);
    for (size_t i = 0; i < vertices.size(); i++) {
      // Repeat starting point as the last element
      const auto polygonVertexIndex = i < vertices.size() - 1 ? i : 0;
      vertices[i] = CreateRegularPolygonVertex(sides, radius, polygonVertexIndex);
    }
  }

  void ScoreMeter::Score(const std::vector<MatchInfo>& matches)
  {
    TextParams textParams;
    textParams.layer = _layer;
    textParams.fontName = kVeraFontBold;
    textParams.fontSize = 24;
    textParams.text = "+1";
    textParams.z = _z + 100;

    for (const auto& match : matches)
    {
      textParams.color = kPiecesColor[match.type];

      FloatingPoint point;
      point.text = GGame.Create<Text>(textParams);
      point.text->transform->SetCenterPosition(match.position.x, match.position.y);
      point.currentPos = match.position.Convert<float>();
      point.wantedPos = transform->GetCenterPosition().Convert<float>();
      point.speed = 10.0f;
      point.acceleration = 200.0f;
      point.maxSpeed = 1000.0f;
      point.type = match.type;

      _floatingPoints.push_back(point);
    }
  }

  void ScoreMeter::UpdateFloatingPoint(FloatingPoint& point)
  {
    point.speed = std::min(point.maxSpeed, point.speed + GTime.deltaTime * point.acceleration);
    const auto moveTowardsResult = MoveTowardsDone(point.currentPos, point.wantedPos, point.speed  *GTime.deltaTime);
    point.currentPos = moveTowardsResult.first;

    const auto currentPos = point.currentPos.Convert<int32_t>();
    point.text->transform->SetCenterPosition(currentPos);

    if (moveTowardsResult.second)
    {
      _wantedScore += 1;
      _scoreChanged = true;
      _gameSaveDirty = true;

      for (size_t i = 0; i < kPieceType_Count; i++)
      {
        if (i != point.type) _energy[i] += kMatchEnergyContribution;
        if (_energy[i] >= kThresholdEnergy)
        {
          _energySymbolsPulse.Register(_energySymbols[i]);
          if (_energySymbolsPulse.GetState() == kPulseAnimationState_Stopped)
          {
            _energySymbolsPulse.Start();
          }
        }
      }

      point.text->Destroy();
    }
  }

  void ScoreMeter::UpdateEnergySpiderChart()
  {
    std::vector<Vector2D_i32> points(kPieceType_Count);

    for (size_t i = 0; i < kPieceType_Count; i++)
    {
      float radius = 0.0f;
      if (_energy[i] <= kThresholdEnergy)
      {
        const auto t = (_energy[i] - kMinEnergy) / (kMaxEnergy - kMinEnergy);
        radius = Interpolate(_minEnergyDisplayedRadius, _thresholdEnergyDisplayedRadius, t);
      }
      else
      {
        const auto t = (_energy[i] - kThresholdEnergy) / (kMaxEnergy - kThresholdEnergy);
        radius = Interpolate(_thresholdEnergyDisplayedRadius, _maxEnergyDisplayedRadius, t);
      }

      points[i] = CreateRegularPolygonVertex(kPieceType_Count, radius, i);
    }

    // To close the loop repeat the start
    points.push_back(points[0]);

    _energySpiderChart->SetPoints(points);
  }

  void ScoreMeter::Update()
  {
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

    if (_scoreChanged)
    {
      UpdateEnergySpiderChart();
      _scoreChanged = false;
    }

    _energySymbolsPulse.Update();
  }

  bool ScoreMeter::IsPieceTypeCharged(const PieceType type) const
  {
    return _energy[type] >= kThresholdEnergy;
  }

  void ScoreMeter::ResetPieceTypeEnergy(const PieceType type)
  {
    _energy[type] = kMinEnergy;
    _energySymbolsPulse.Unregister(_energySymbols[type]);
    UpdateEnergySpiderChart();
    _gameSaveDirty = true;
  }

  void ScoreMeter::GameSaveLoaded(const json& save)
  {
    if (const auto root = save.find("scoreMeter"); root != save.end())
    {
      if (const auto energy = root->find("energy"); energy != root->end())
      {
        for (size_t type = 0; type < kPieceType_Count; type++)
        {
          _energy[type] = (*energy)[type];
        }
      }

      if (const auto score = root->find("score"); score != root->end())
      {
        _wantedScore = *score;
        _displayedScore = _wantedScore;
        _currentScore = static_cast<float>(_wantedScore);
      }
    }
  }

  void ScoreMeter::GameSaveWriteRequested(json& save)
  {
    _gameSaveDirty = false;
    auto& root = save["scoreMeter"];
    root["energy"] = _energy;
    root["score"] = _currentScore;
  }

  GameSaveAutoSaveRequestReply ScoreMeter::GameSaveAutoSaveRequest()
  {
    return _gameSaveDirty ? kGameSaveAutoSaveRequestReply_Changed : kGameSaveAutoSaveRequestReply_NotChanged;
  }
}