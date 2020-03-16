#pragma once
#include "piece.h"

class FreePieces
{
    Board *m_board;
    vector<Piece *> *m_pieces;
public:
    FreePieces(Board *);
    ~FreePieces();
    Piece *addPiece(Piece *);
    Piece *removePiece(Piece *);
    unsigned pieceCount() const;
    void update();
};
