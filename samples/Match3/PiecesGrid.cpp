#include "PiecesGrid.h"

#include "Piece.h"
#include "Game.h"
#include "Input.h"

#include <algorithm>
#include <cassert>

namespace MatchThree
{
  PiecesGrid::PiecesGrid(const PiecesGridParams& params)
    : _columns        (params.columns)
    , _rows           (params.rows)
    , _inputEnabled   (true)
    , _pieceWidth     (params.pieceWidth)
    , _pieceHeight    (params.pieceHeight)
    , _piecesSpacing  (params.piecesSpacing)
  {
    assert(_columns > 0 && _rows > 0);

    GGame.StartBatchCreate();

    PieceParams pieceParams;
    pieceParams.layer = params.layer;
    pieceParams.type = kPieceType_BlueOctagon;
    pieceParams.z = 0;
    pieceParams.width = _pieceWidth;
    pieceParams.height = _pieceHeight;

    _pieces.resize(_rows * _columns);

    for (size_t row = 0; row < _rows; row++)
    {
      for (size_t column = 0; column < _columns; column++)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);
        pieceParams.z = static_cast<int32_t>(column * 5);
        pieceParams.type = static_cast<PieceType>(GGame.RandomNumber(0, kPieceType_Count - 1));

        auto& piece = _pieces[piecesIndex];
        piece = GGame.Create<Piece>(pieceParams);

        _pieceToPiecesIndex[piece] = piecesIndex;

        const auto x = static_cast<int32_t>((_pieceWidth + _piecesSpacing) * column);
        const auto y = static_cast<int32_t>((_pieceHeight + _piecesSpacing) * row);

        piece->SetCenterPosition(x, y);
      }
    }

    GGame.EndBatchCreate();
    EvaluateMatches();
  }

  void PiecesGrid::Update()
  {
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
      // Only one piece is selected and mouse was clicked while it is not hovered => non-piece clicked => deselect the selected one
      if (firstSelected != std::end(_pieces) && !(*firstSelected)->IsHovered() && secondSelected == std::end(_pieces))
      {
        (*firstSelected)->Deselect();
      }
      // Two pieces are selected
      else if (firstSelected != std::end(_pieces) && secondSelected != std::end(_pieces))
      {
        // Selected piece are adjacent => Swap their position
        if (PiecesAdjacent(*firstSelected, *secondSelected))
        {
          EnableInput(false);

          (*firstSelected)->MoveCenterTo((*secondSelected)->GetCenterX(), (*secondSelected)->GetCenterY());
          (*secondSelected)->MoveCenterTo((*firstSelected)->GetCenterX(), (*firstSelected)->GetCenterY());
        }
        // Two non-adjacent pieces selected, deselect the one that was just clicked
        else
        {
          const auto lastSelected = (*firstSelected)->IsHovered() ? firstSelected : secondSelected;
          assert((*lastSelected)->IsHovered());
          (*lastSelected)->FlashError();
          (*lastSelected)->Deselect();
        }

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

        // Swap Z so swapping always have the same animation (right over left)
        const auto firstZ = (*firstSelected)->GetZ();
        (*firstSelected)->SetZ((*secondSelected)->GetZ());
        (*secondSelected)->SetZ(firstZ);

        // Update pieces index map
        const auto firstPiecesIndex = _pieceToPiecesIndex[(*firstSelected)];
        _pieceToPiecesIndex[(*firstSelected)] = _pieceToPiecesIndex[(*secondSelected)];
        _pieceToPiecesIndex[(*secondSelected)] = firstPiecesIndex;

        // Swap them in _pieces vector
        std::iter_swap(firstSelected, secondSelected);

        EvaluateMatches();
      }
    }
  }

  void PiecesGrid::EnableInput(const bool enable)
  {
    _inputEnabled = enable;
    std::for_each(std::begin(_pieces), std::end(_pieces), [this](const auto& piece)
    {
      piece->EnableInput(_inputEnabled);
    });
  }

  size_t PiecesGrid::PiecesIndexFromColumnRow(const size_t row, const size_t column) const
  {
    return row * _columns + column;
  }

  std::pair<size_t, size_t> PiecesGrid::ColumnRowFromPiecesIndex(const size_t piecesIndex) const
  {
    return { piecesIndex / _columns, piecesIndex % _columns };
  }

  size_t PiecesGrid::PiecesIndexFromPiece(Piece* piece) const
  {
    const auto found = _pieceToPiecesIndex.find(piece);
    assert(found != std::cend(_pieceToPiecesIndex));
    return found->second;
  }

  bool PiecesGrid::PiecesAdjacent(Piece* pieceA, Piece* pieceB) const
  {
    return PiecesAdjacent(PiecesIndexFromPiece(pieceA), PiecesIndexFromPiece(pieceB));
  }

  bool PiecesGrid::PiecesAdjacent(const size_t piecesIndexA, const size_t piecesIndexB) const
  {
    const auto columnRowPairA = ColumnRowFromPiecesIndex(piecesIndexA);
    const auto columnRowPairB = ColumnRowFromPiecesIndex(piecesIndexB);
    const auto rowDistance = std::abs(static_cast<int32_t>(columnRowPairA.first) - static_cast<int32_t>(columnRowPairB.first));
    const auto columnDistance = std::abs(static_cast<int32_t>(columnRowPairA.second) - static_cast<int32_t>(columnRowPairB.second));

    assert((rowDistance + columnDistance) > 0);
    return (rowDistance + columnDistance) == 1;
  }

  void PiecesGrid::SetCenterPosition(const int32_t x, const int32_t y)
  {
    const auto totalWidth = _columns * (_pieceWidth + _piecesSpacing) - _piecesSpacing;
    const auto totalHeight = _rows * (_pieceWidth + _piecesSpacing) - _piecesSpacing;

    const auto startX = x - totalWidth / 2;
    const auto startY = y - totalHeight / 2;

    for (size_t row = 0; row < _rows; row++)
    {
      for (size_t column = 0; column < _columns; column++)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);
        auto& piece = _pieces[piecesIndex];

        const auto x = static_cast<int32_t>(startX + (_pieceWidth + _piecesSpacing) * column);
        const auto y = static_cast<int32_t>(startY + (_pieceWidth + _piecesSpacing) * row);

        piece->SetCenterPosition(x, y);
      }
    }
  }

  void PiecesGrid::EvaluateMatches()
  {
    // Search for row matches
    PieceType currentType = kPieceType_Count;
    size_t    currentTypeCount = 1;
    bool      matchFound = false;
    size_t    matchStart = 0;
    size_t    matchSize = 0;

    for (size_t row = 0; row < _rows; row++)
    {
      for (size_t column = 0; column < _columns; column++)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);
        auto& piece = _pieces[piecesIndex];

        const auto pieceType = piece->GetType();
        if (currentType != pieceType)
        {
          currentType = pieceType;
          currentTypeCount = 1;

          if (matchFound)
          {
            for (size_t matchColumnn = matchStart; matchColumnn < matchStart + matchSize; matchColumnn++)
            {
              const auto matchPiecesIndex = PiecesIndexFromColumnRow(row, matchColumnn);
              auto& matchPiece = _pieces[matchPiecesIndex];
              matchPiece->Show(false);
            }
            matchFound = false;
            matchStart = 0;
            matchSize = 0;
          }
        }
        else
        {
          currentTypeCount++;
          if (currentTypeCount == 3)
          {
            matchFound = true;
            matchStart = column - 2;
          }

          if (currentTypeCount >= 3)
          {
            matchSize = currentTypeCount;
          }
        }
      }

      currentType = kPieceType_Count;
      currentTypeCount = 1;

      if (matchFound)
      {
        for (size_t matchColumnn = matchStart; matchColumnn < matchStart + matchSize; matchColumnn++)
        {
          const auto matchPiecesIndex = PiecesIndexFromColumnRow(row, matchColumnn);
          auto& matchPiece = _pieces[matchPiecesIndex];
          matchPiece->Show(false);
        }
        matchFound = false;
        matchStart = 0;
        matchSize = 0;
      }
    }
  }
}