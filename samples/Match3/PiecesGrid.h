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

  class PiecesGrid : public IGameObject
  {
  public:
    PiecesGrid(const PiecesGridParams& params);
    void Update() override;

    void SetCenterPosition(const int32_t x, const int32_t y);

  private:
    size_t PiecesIndexFromColumnRow(const size_t row, const size_t column) const;
    size_t PiecesIndexFromPiece(Piece* piece) const;
    std::pair<size_t, size_t> ColumnRowFromPiecesIndex(const size_t piecesIndex) const;
    bool PiecesAdjacent(const size_t piecesIndexA, const size_t piecesIndexB) const;
    bool PiecesAdjacent(Piece* pieceA, Piece* pieceB) const;

    void EnableInput(const bool enable);
    void EvaluateMatches();

    size_t  _rows;
    size_t  _columns;
    int32_t _pieceWidth;
    int32_t _pieceHeight;
    int32_t _piecesSpacing;

    bool _inputEnabled;

    std::vector<Piece*> _pieces;
    std::unordered_map<Piece*, size_t> _pieceToPiecesIndex;
  };
}
