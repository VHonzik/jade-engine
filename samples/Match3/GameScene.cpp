#include "GameScene.h"

#include "EngineConstants.h"
#include "FTC.h"
#include "Game.h"
#include "PiecesGrid.h"
#include "ScoreMeter.h"
#include "Transform.h"

#include <numeric>

namespace MatchThree
{
  void GameScene::Start()
  {
    _score = 0;

    PiecesGridParams piecesGridParams;
    piecesGridParams.layer = kObjectLayer_ui;
    piecesGridParams.columns = 14;
    piecesGridParams.rows = 8;
    piecesGridParams.pieceWidth = 50;
    piecesGridParams.pieceHeight = 50;
    piecesGridParams.piecesSpacing = 10;

    _grid = GGame.Create<PiecesGrid>(piecesGridParams);
    _grid->transform->SetCenterPosition(GGame.GetHalfWidth(), GGame.GetHeight() - 10 - _grid->transform->GetHeight() / 2);

    ScoreMeterParams scoreParams;
    scoreParams.layer = kObjectLayer_ui;
    scoreParams.sides = kPieceType_Count;
    scoreParams.width = 100;
    scoreParams.height = 100;
    scoreParams.z = 0;

    _scoreMeter = GGame.Create<ScoreMeter>(scoreParams);
    _scoreMeter->transform->SetCenterPosition(GGame.GetHalfWidth(), 100);
  }


  void GameScene::Update()
  {
    _grid->UpdateGrid();

    const auto& matches = _grid->GetMatches();
    if (matches.size() > 0)
    {
      _scoreMeter->Score(matches);
      _grid->ResetMatches();
    }
  }
}