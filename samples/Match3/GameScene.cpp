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
    const auto gridTotalHeight = PiecesGrid::GetTotalHeight(piecesGridParams.rows, piecesGridParams.pieceHeight, piecesGridParams.piecesSpacing);
    piecesGridParams.centerPosition = { GGame.GetHalfWidth(), GGame.GetHeight() - 10 - gridTotalHeight / 2 };

    _grid = GGame.Create<PiecesGrid>(piecesGridParams);

    ScoreMeterParams scoreParams;
    scoreParams.layer = kObjectLayer_ui;
    scoreParams.width = 180;
    scoreParams.height = 180;
    scoreParams.z = 0;

    _scoreMeter = GGame.Create<ScoreMeter>(scoreParams);
    _scoreMeter->transform->SetCenterPosition(GGame.GetHalfWidth(), 110);
  }

  void GameScene::Update()
  {
    _grid->UpdateGrid();

    const auto matches = _grid->GetMatches();
    if (matches.size() > 0)
    {
      _scoreMeter->Score(matches);
      for (const auto& match : matches)
      {
        if (match.initiatedByPlayerSwap && _scoreMeter->IsPieceTypeCharged(match.type))
        {
          _grid->ForcePieceTypeMatch(match.type);
          _scoreMeter->ResetPieceTypeEnergy(match.type);
          break;
        }
      }
      _grid->ResetMatches();
    }
  }
}