#pragma once

#include "Animations.h"
#include "IGameObject.h"
#include "ObjectLayer.h"
#include "SampleConstants.h"
#include "Vector2D.h"

#include <array>
#include <vector>

using namespace JadeEngine;

namespace JadeEngine
{
  class Text;
  class LineStrip;
  class Sprite;
}

namespace MatchThree
{
  struct ScoreMeterParams
  {
    ObjectLayer layer;
    int32_t     width;
    int32_t     height;
    int32_t     z;
  };

  struct FloatingPoint
  {
    Text*         text;
    Vector2D_f32  currentPos;
    Vector2D_f32  wantedPos;
    float         speed;
    float         acceleration;
    float         maxSpeed;
    PieceType     type;
  };

  class ScoreMeter : public IGameObject
  {
  public:
    ScoreMeter(const ScoreMeterParams& params);
    void Score(const std::vector<MatchInfo>& matches);
    void Update() override;
    bool IsPieceTypeCharged(const PieceType type) const;
    void ResetPieceTypeEnergy(const PieceType type);
  private:
    Vector2D_i32 CreateRegularPolygonVertex(const int32_t sides, const float radius, const size_t index);
    void CreateRegularPolygonVertices(std::vector<Vector2D_i32>& vertices, const int32_t sides, const float radius);
    void UpdateFloatingPoint(FloatingPoint& point);
    void UpdateEnergySpiderChart();

    LineStrip* _fullEnergyBorder;
    LineStrip* _thresholdEnergyBorder;
    LineStrip* _energySpiderChart;
    std::array<LineStrip*, kPieceType_Count> _energyLines;
    std::array<Sprite*, kPieceType_Count> _energySymbols;
    PulseAnimation _energySymbolsPulse;

    Text* _text;

    std::vector<FloatingPoint> _floatingPoints;

    int32_t _wantedScore;
    float _currentScore;
    int32_t _displayedScore;
    ObjectLayer _layer;
    bool _scoreChanged;

    std::array<float, kPieceType_Count> _energy;

    float _maxEnergyDisplayedRadius;
    float _minEnergyDisplayedRadius;
    float _thresholdEnergyDisplayedRadius;
  };
}
