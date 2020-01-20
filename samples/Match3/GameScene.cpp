#include "GameScene.h"

#include "Game.h"
#include "Input.h"
#include "Piece.h"

#include <cassert>

namespace MatchThree
{
  void GameScene::Start()
  {
    PieceParams pieceParams;
    pieceParams.layer = kObjectLayer_ui;
    pieceParams.type = kPieceType_RedDiamond;
    pieceParams.width = 50;
    pieceParams.height = 50;
    pieceParams.z = 0;

    _pieces.push_back(GGame.Create<Piece>(pieceParams));
    _pieces[_pieces.size() - 1]->SetCenterPosition(GGame.GetHalfWidth() - 30, GGame.GetHalfHeight());

    pieceParams.z = 5;
    pieceParams.type = kPieceType_BlueOctagon;
    _pieces.push_back(GGame.Create<Piece>(pieceParams));
    _pieces[_pieces.size() - 1]->SetCenterPosition(GGame.GetHalfWidth() + 30, GGame.GetHalfHeight());

    _inputEnabled = true;
  }

  void GameScene::Update()
  {
    const auto hoveredPiece = std::find_if(std::cbegin(_pieces), std::cend(_pieces), [](const auto& piece)
    {
      return piece->IsHovered();
    });

    const auto firstSelected = std::find_if(std::begin(_pieces), std::end(_pieces), [](auto& piece)
    {
      return piece->IsSelected();
    });

    auto secondSelected = std::end(_pieces);
    if (firstSelected != std::end(_pieces))
    {
      secondSelected = std::find_if(std::next(firstSelected), std::end(_pieces), [](auto& piece)
      {
        return piece->IsSelected();
      });
    }

    if (_inputEnabled && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
    {
      // Only one piece is selected and mouse was clicked while it is not hovered => deselect it
      if (firstSelected != std::end(_pieces) && !(*firstSelected)->IsHovered() && secondSelected == std::end(_pieces))
      {
        (*firstSelected)->Deselect();
      }
      // Two pieces are selected => swap their position
      else if (firstSelected != std::end(_pieces) && secondSelected != std::end(_pieces))
      {
        EnableInput(false);

        (*firstSelected)->MoveCenterTo((*secondSelected)->GetCenterX(), (*secondSelected)->GetCenterY());
        (*secondSelected)->MoveCenterTo((*firstSelected)->GetCenterX(), (*firstSelected)->GetCenterY());
      }
    }
    else if (!_inputEnabled)
    {
      assert(firstSelected != std::end(_pieces) && secondSelected != std::end(_pieces));

      const auto firstMoving = std::find_if(std::cbegin(_pieces), std::cend(_pieces), [](const auto& piece)
      {
        return piece->IsMoving();
      });

      // Nothing is moving re-enable the input
      if (firstMoving == std::cend(_pieces))
      {
        EnableInput(true);
        (*firstSelected)->Deselect();
        (*secondSelected)->Deselect();
      }
    }
  }

  void GameScene::EnableInput(const bool enable)
  {
    _inputEnabled = enable;
    std::for_each(std::begin(_pieces), std::end(_pieces), [this](const auto& piece)
    {
      piece->EnableInput(_inputEnabled);
    });
  }
}