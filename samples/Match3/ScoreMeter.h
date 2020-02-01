#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "SampleConstants.h"
#include "Vector2D.h"

#include <vector>

using namespace JadeEngine;

namespace JadeEngine
{
  class Text;
  class LineStrip;
}

namespace MatchThree
{
  struct ScoreMeterParams
  {
    ObjectLayer layer;
    int32_t     sides;
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
  };

  class ScoreMeter : public IGameObject
  {
  public:
    ScoreMeter(const ScoreMeterParams& params);
    void Score(const std::vector<MatchInfo>& matches);
    void Update() override;
  private:
    void CreateRegularPolygonVertices(std::vector<Vector2D_i32>& vertices, const int32_t sides, const int32_t radius);
    void UpdateFloatingPoint(FloatingPoint& point);
    LineStrip* _fullEnergyBorder;
    LineStrip* _thresholdEnergyBorder;
    Text* _text;

    std::vector<FloatingPoint> _floatingPoints;

    int32_t _wantedScore;
    float _currentScore;
    int32_t _displayedScore;
    ObjectLayer _layer;
  };
}
