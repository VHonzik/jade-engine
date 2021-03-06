#include "PiecesGrid.h"

#include "Piece.h"
#include "Game.h"
#include "Input.h"
#include "Transform.h"

#include <algorithm>
#include <cassert>

namespace
{
  const auto kSwappingMovementSpeed = 200.0f;
  const auto kRefillMovementSpeed = 500.0f;
}

namespace MatchThree
{
  PiecesGrid::PiecesGrid(const PiecesGridParams& params)
    : _columns              (params.columns)
    , _rows                 (params.rows)
    , _inputEnabled         (true)
    , _pieceWidth           (params.pieceWidth)
    , _pieceHeight          (params.pieceHeight)
    , _piecesSpacing        (params.piecesSpacing)
    , _piecesMoving         (false)
    , _piecesMatching       (false)
    , _piecesMovingMatch    (false)
    , _swappedPieces        { nullptr, nullptr }
    , _layer                (params.layer)
  {
    assert(_columns > 0 && _rows > 0);

    transform->Initialize({0, 0}, { GetTotalWidth(_columns, _pieceWidth, _piecesSpacing), GetTotalHeight(_rows, _pieceHeight, _piecesSpacing)});
    transform->SetCenterPosition(params.centerPosition);

    GGame.StartBatchCreate();

    _pieces.resize(_rows * _columns);

    for (size_t row = 0; row < _rows; row++)
    {
      for (size_t column = 0; column < _columns; column++)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);

        auto& piece = _pieces[piecesIndex];
        piece = CreateRandomPiece(column);
        _pieceToPiecesIndex[piece] = piecesIndex;

        piece->TeleportCenterTo(GridPosition(row, column));
      }
    }

    GGame.EndBatchCreate();
    EvaluateMatches();
  }

  int32_t PiecesGrid::GetTotalWidth(size_t columns, int32_t pieceWidth, int32_t piecesSpacing)
  {
    return static_cast<int32_t>(columns * (pieceWidth + piecesSpacing) - piecesSpacing);
  }

  int32_t PiecesGrid::GetTotalHeight(size_t rows, int32_t pieceHeight, int32_t piecesSpacing)
  {
    return static_cast<int32_t>(rows * (pieceHeight + piecesSpacing) - piecesSpacing);
  }

  Piece* PiecesGrid::CreateRandomPiece(const size_t column)
  {
    PieceParams pieceParams;
    pieceParams.layer = _layer;
    pieceParams.type = static_cast<PieceType>(GGame.RandomNumber(0, kPieceType_Count - 1));
    pieceParams.z = static_cast<int32_t>(column * 5);
    pieceParams.width = _pieceWidth;
    pieceParams.height = _pieceHeight;

    return GGame.Create<Piece>(pieceParams);
  }

  void PiecesGrid::SwapPiecesInit(const PiecesContainer::iterator& pieceA, const PiecesContainer::iterator& pieceB)
  {
    (*pieceA)->MoveCenterTo((*pieceB)->transform->GetCenterPosition(), kSwappingMovementSpeed);
    (*pieceB)->MoveCenterTo((*pieceA)->transform->GetCenterPosition(), kSwappingMovementSpeed);
    _piecesMoving = true;
  }

  void PiecesGrid::SwapPiecesFinish(const PiecesContainer::iterator& pieceA, const PiecesContainer::iterator& pieceB)
  {
    _swappedPieces = { (*pieceA), (*pieceB) };

    (*pieceA)->Deselect();
    (*pieceB)->Deselect();

    // Swap Z so swapping always have the same animation (right over left)
    const auto firstZ = (*pieceA)->GetZ();
    (*pieceA)->SetZ((*pieceB)->GetZ());
    (*pieceB)->SetZ(firstZ);

    // Update pieces index map
    const auto firstPiecesIndex = _pieceToPiecesIndex[(*pieceA)];
    _pieceToPiecesIndex[(*pieceA)] = _pieceToPiecesIndex[(*pieceB)];
    _pieceToPiecesIndex[(*pieceB)] = firstPiecesIndex;

    // Swap them in _pieces vector
    std::iter_swap(pieceA, pieceB);
  }

  void PiecesGrid::UpdateGrid()
  {
    _swappedPieces = { nullptr, nullptr };

    if (transform->IsDirty(kDirtyFlag_CenterPosition))
    {
      UpdatePiecesPosition();
    }

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
        // Selected piece are adjacent => Swap their position and disable input
        if (PiecesAdjacent(*firstSelected, *secondSelected))
        {
          EnableInput(false);
          SwapPiecesInit(firstSelected, secondSelected);
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
    else if (_piecesMoving)
    {
      assert(firstSelected != std::end(_pieces) && secondSelected != std::end(_pieces));

      const auto firstMoving = std::find_if(std::cbegin(_pieces), std::cend(_pieces), [](const auto& piece)
      {
        return piece->IsMoving();
      });

      // Nothing is moving, swap the pieces and evaluate matches
      if (firstMoving == std::cend(_pieces))
      {
        _piecesMoving = false;
        SwapPiecesFinish(firstSelected, secondSelected);
        EvaluateMatches();

        // Not match found
        if (!_piecesMatching)
        {
          EnableInput(true);
        }
      }
    }
    else if (_piecesMatching)
    {
      bool done = true;
      for (auto iter = std::begin(_pieces); iter != std::end(_pieces); ++iter)
      {
        auto& piece = *iter;
        if (piece->IsFlashing() && piece->IsFlashingMatchDone())
        {
          piece->Destroy();
          _pieceToPiecesIndex.erase(piece);
          piece = nullptr;
        }
        else if (piece->IsFlashing())
        {
          done = false;
        }
      }

      if (done)
      {
        _piecesMatching = false;
        RefillGrid();
      }
    }
    else if (_piecesMovingMatch)
    {
      const auto firstMoving = std::find_if(std::cbegin(_pieces), std::cend(_pieces), [](const auto& piece)
      {
        return piece->IsMoving();
      });

      // Nothing is moving, done with matches
      if (firstMoving == std::cend(_pieces))
      {
        _piecesMovingMatch = false;
        EvaluateMatches();

        // Not match found
        if (!_piecesMatching)
        {
          EnableInput(true);
        }
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

  void PiecesGrid::UpdatePiecesPosition()
  {
    for (size_t row = 0; row < _rows; row++)
    {
      for (size_t column = 0; column < _columns; column++)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);
        auto& piece = _pieces[piecesIndex];

        piece->TeleportCenterTo(GridPosition(row, column));
      }
    }
  }

  bool PiecesGrid::ColumnAdvance(PiecesContainer::iterator& iter)
  {
    ++iter;
    const auto index = std::distance(std::begin(_pieces), iter);
    return (index % _columns) == 0;
  }

  bool PiecesGrid::RowAdvance(PiecesContainer::iterator& iter)
  {
    auto index = std::distance(std::begin(_pieces), iter);
    auto row_column = ColumnRowFromPiecesIndex(index);
    bool nextColumnn = false;

    row_column.first += 1;
    if (row_column.first == _rows)
    {
      row_column.first = 0;
      row_column.second++;

      nextColumnn = true;

      if (row_column.second == _columns)
      {
        iter = std::end(_pieces);
        return false;
      }
    }

    index = PiecesIndexFromColumnRow(row_column.first, row_column.second);
    iter = std::next(std::begin(_pieces), index);

    return nextColumnn;
  }

  void PiecesGrid::EvaluateMatches(const IterationDirection direction)
  {
    auto iter = std::begin(_pieces);
    bool newLine = true;

    std::vector<Piece*> possibleMatches;
    std::vector<Piece*> matches;

    while (iter != std::end(_pieces))
    {
      const auto& piece = *iter;
      const auto pieceType = piece->GetType();

      if (!newLine && (possibleMatches.size() == 0 || possibleMatches[0]->GetType() == pieceType))
      {
        possibleMatches.push_back(piece);
      }
      else
      {
        if (possibleMatches.size() >= 3)
        {
          std::copy(std::cbegin(possibleMatches), std::cend(possibleMatches), std::back_inserter(matches));

        }
        possibleMatches.clear();
        possibleMatches.push_back(piece);
      }

      switch (direction)
      {
      case kIterationDirection_Columns:
        newLine = ColumnAdvance(iter);
        break;
      default:
        assert(direction == kIterationDirection_Rows);
        newLine = RowAdvance(iter);
        break;
      }
    }

    // We could have reached end while inside a match sequence
    if (possibleMatches.size() >= 3)
    {
      std::copy(std::cbegin(possibleMatches), std::cend(possibleMatches), std::back_inserter(matches));
    }

    std::transform(std::cbegin(matches), std::cend(matches), std::back_inserter(_matches), [&](const Piece* piece)
    {
      return MatchInfo{ piece->GetType(), piece->transform->GetCenterPosition(), _swappedPieces.first == piece || _swappedPieces.second == piece };
    });

    for (auto& piece : matches)
    {
      piece->FlashMatch();
    }

    if (!matches.empty())
    {
      _piecesMatching = true;
    }
  }

  void PiecesGrid::EvaluateMatches()
  {
    EvaluateMatches(kIterationDirection_Columns);
    EvaluateMatches(kIterationDirection_Rows);
  }

  void PiecesGrid::RefillGrid()
  {
    GGame.StartBatchCreate();
    _piecesMovingMatch = true;
    for (size_t column = 0; column < _columns; ++column)
    {
      std::vector<Piece*> rowPieces;
      // reverse iterating on size_t so the condition uses the fact row will wrap around
      for (size_t row = _rows - 1; row < _rows; --row)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);
        const auto& piece = _pieces[piecesIndex];
        if (piece != nullptr) rowPieces.push_back(piece);
        _pieceToPiecesIndex.erase(piece);
      }

      auto nextFreePiece = std::begin(rowPieces);

      // reverse iterating on size_t so the condition uses the fact row will wrap around
      for (size_t row = _rows - 1; row < _rows; --row)
      {
        const auto piecesIndex = PiecesIndexFromColumnRow(row, column);
        auto& piece = _pieces[piecesIndex];

        const auto wantedPos = GridPosition(row, column);

        if (nextFreePiece != std::end(rowPieces))
        {
          piece = *nextFreePiece;
          piece->MoveCenterTo(wantedPos, kRefillMovementSpeed);
          ++nextFreePiece;
        }
        else
        {
          piece = CreateRandomPiece(column);
          const auto startY = - static_cast<int32_t>((_rows - 1 - row)) * (_pieceHeight + _piecesSpacing);
          piece->TeleportCenterTo({ wantedPos.x, startY });
          piece->MoveCenterTo(wantedPos, kRefillMovementSpeed);
        }

        _pieceToPiecesIndex[piece] = piecesIndex;
      }
    }
    GGame.EndBatchCreate();
  }

  Vector2D_i32 PiecesGrid::GridPosition(const size_t row, const size_t column) const
  {
    const auto startX = transform->GetCenterX() - transform->GetWidth() / 2;
    const auto startY = transform->GetCenterY() - transform->GetHeight() / 2;
    return { static_cast<int32_t>(startX + (_pieceWidth + _piecesSpacing) * column), static_cast<int32_t>(startY + (_pieceHeight + _piecesSpacing) * row) };
  }

  const std::vector<MatchInfo>& PiecesGrid::GetMatches() const
  {
    return _matches;
  }

  void PiecesGrid::ResetMatches()
  {
    _matches.clear();
  }

  void PiecesGrid::ForcePieceTypeMatch(const PieceType type)
  {
    auto iter = std::begin(_pieces);
    bool atLeastOneMatched = false;

    while (iter != std::end(_pieces))
    {
      const auto& piece = *iter;
      if (piece->GetType() == type)
      {
        atLeastOneMatched = true;
        piece->FlashMatch();
        _matches.push_back({piece->GetType(), piece->transform->GetCenterPosition()});
      }

      ColumnAdvance(iter);
    }

    if (atLeastOneMatched)
    {
      _piecesMatching = true;
    }
  }
}