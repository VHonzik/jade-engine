#pragma once

#include "IGameObject.h"
#include "ObjectLayer.h"
#include "Piece.h"

#include <unordered_map>
#include <vector>

using namespace JadeEngine;

namespace MatchThree
{
  struct PiecesGridParams
  {
    ObjectLayer layer;
    size_t      columns;
    size_t      rows;
    int32_t     pieceWidth;
    int32_t     pieceHeight;
    int32_t     piecesSpacing;
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
    void SetCenterPosition(const int32_t x, const int32_t y);

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
    int32_t GridPositionX(const size_t row, const size_t column) const;
    int32_t GridPositionY(const size_t row, const size_t column) const;

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

    int32_t _centerX;
    int32_t _centerY;

    PiecesContainer _pieces;
    std::unordered_map<Piece*, size_t> _pieceToPiecesIndex;
  };
}
