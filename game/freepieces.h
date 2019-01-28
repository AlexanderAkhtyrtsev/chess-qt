#pragma once
#include "Piece.h"
#include <map>
using std::map;

class FreePieces
{
    Board *m_board;
    vector<Piece *> *m_pieces;
public:
    FreePieces(Board *);
    ~FreePieces();
    Piece *addPiece(Piece *);
    Piece *removePiece(Piece *);
    void update();
};
