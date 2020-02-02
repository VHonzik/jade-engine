#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "Piece.h"
#include "Vector2D.h"

#include <array>
#include <unordered_map>
#include <vector>

using namespace JadeEngine;

namespace MatchThree
{
  struct PiecesGridParams
  {
    ObjectLayer   layer;
    size_t        columns;
    size_t        rows;
    int32_t       pieceWidth;
    int32_t       pieceHeight;
    int32_t       piecesSpacing;
    Vector2D_i32  centerPosition;
  };

  enum IterationDirection
  {
    kIterationDirection_Rows,
    kIterationDirection_Columns,
  };

  using PiecesContainer = std::vector<Piece*>;

  class PiecesGrid : public IGameObject
  {
  public:
    PiecesGrid(const PiecesGridParams& params);
    void UpdateGrid();

    const std::vector<MatchInfo>& GetMatches() const;
    void ResetMatches();

    static int32_t GetTotalWidth(size_t columns, int32_t pieceWidth, int32_t piecesSpacing);
    static int32_t GetTotalHeight(size_t rows, int32_t pieceHeight, int32_t piecesSpacing);

    void ForcePieceTypeMatch(const PieceType type);

  private:
    size_t PiecesIndexFromColumnRow(const size_t row, const size_t column) const;
    size_t PiecesIndexFromPiece(Piece* piece) const;
    std::pair<size_t, size_t> ColumnRowFromPiecesIndex(const size_t piecesIndex) const;
    bool PiecesAdjacent(const size_t piecesIndexA, const size_t piecesIndexB) const;
    bool PiecesAdjacent(Piece* pieceA, Piece* pieceB) const;
    bool ColumnAdvance(PiecesContainer::iterator& iter);
    bool RowAdvance(PiecesContainer::iterator& iter);

    void EnableInput(const bool enable);
    void EvaluateMatches();
    void EvaluateMatches(const IterationDirection direction);

    void SwapPiecesInit(const PiecesContainer::iterator& pieceA, const PiecesContainer::iterator& pieceB);
    void SwapPiecesFinish(const PiecesContainer::iterator& pieceA, const PiecesContainer::iterator& pieceB);

    void RefillGrid();

    Piece* CreateRandomPiece(const size_t column);
    Vector2D_i32 GridPosition(const size_t row, const size_t column) const;

    void UpdatePiecesPosition();

    ObjectLayer _layer;

    size_t  _rows;
    size_t  _columns;
    int32_t _pieceWidth;
    int32_t _pieceHeight;
    int32_t _piecesSpacing;

    bool _inputEnabled;

    bool _piecesMoving;
    bool _piecesMatching;
    bool _piecesMovingMatch;
    std::pair<Piece*, Piece*> _swappedPieces;

    PiecesContainer _pieces;
    std::unordered_map<Piece*, size_t> _pieceToPiecesIndex;

    std::vector<MatchInfo> _matches;
  };
}
