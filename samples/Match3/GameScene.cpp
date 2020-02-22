#include "GameScene.h"

#include "EngineConstants.h"
#include "Input.h"
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
    piecesGridParams.layer = kObjectLayer_UI;
    piecesGridParams.columns = 14;
    piecesGridParams.rows = 8;
    piecesGridParams.pieceWidth = 50;
    piecesGridParams.pieceHeight = 50;
    piecesGridParams.piecesSpacing = 10;
    const auto gridTotalHeight = PiecesGrid::GetTotalHeight(piecesGridParams.rows, piecesGridParams.pieceHeight, piecesGridParams.piecesSpacing);
    piecesGridParams.centerPosition = { GGame.GetHalfWidth(), GGame.GetHeight() - 10 - gridTotalHeight / 2 };

    _grid = GGame.Create<PiecesGrid>(piecesGridParams);

    ScoreMeterParams scoreParams;
    scoreParams.layer = kObjectLayer_UI;
    scoreParams.width = 180;
    scoreParams.height = 180;
    scoreParams.z = 0;

    _scoreMeter = GGame.Create<ScoreMeter>(scoreParams);
    _scoreMeter->transform->SetCenterPosition(GGame.GetHalfWidth(), 110);

    GPersistence.RegisterGameSaveListener(this);
    GPersistence.StartAutoSaveRequest(60.0f);
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

    if (GInput.KeybindPressed(SampleKeybinding::ExitToMenu))
    {
      GGame.PlayScene(kScene_MainMenu);
    }
  }


  void GameScene::GameSaveStateChange(const GameSaveState previousState, const GameSaveState newState)
  {
    if (newState == kGameSaveState_Found)
    {
      GPersistence.LoadGameSave();
    }
    else if (newState == kGameSaveState_NotFound)
    {
      GPersistence.WriteGameSave();
    }
  }

  GameSaveAutoSaveRequestReply GameScene::GameSaveAutoSaveRequest()
  {
    return GetActive() ? kGameSaveAutoSaveRequestReply_NotChanged : kGameSaveAutoSaveRequestReply_Block;
  }
}