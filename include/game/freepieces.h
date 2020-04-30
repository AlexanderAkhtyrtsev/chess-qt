#pragma once
#include "piece.h"

class FreePieces
{
private:
    Board *m_board;
    vector<Piece *> *m_pieces;
public:
    FreePieces(Board *);
    ~FreePieces();
    Piece *addPiece(Piece *);
    Piece *removePiece(Piece *);
    void reset();
    unsigned pieceCount() const;
    void update() const;
};
