#pragma once
#include "piece.h"

class FreePieces
{
public:
    FreePieces(Board *);
    ~FreePieces();
    Piece *addPiece(Piece *);
    Piece *removePiece(Piece *);
    void reset();
    unsigned pieceCount() const;
    void update() const;
private:
    Board *m_board;
    QVector<Piece *> *m_pieces;
};
