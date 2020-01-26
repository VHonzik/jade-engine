#include "GameScene.h"

#include "Game.h"
#include "PiecesGrid.h"


namespace MatchThree
{
  void GameScene::Start()
  {
    PiecesGridParams piecesGridParams;
    piecesGridParams.layer = kObjectLayer_ui;
    piecesGridParams.columns = 12;
    piecesGridParams.rows = 8;
    piecesGridParams.pieceWidth = 50;
    piecesGridParams.pieceHeight = 50;
    piecesGridParams.piecesSpacing = 10;

    _grid = GGame.Create<PiecesGrid>(piecesGridParams);
    _grid->SetCenterPosition(GGame.GetHalfWidth(), GGame.GetHalfHeight());
  }

  void GameScene::Update()
  {
    _grid->UpdateGrid();
  }
}