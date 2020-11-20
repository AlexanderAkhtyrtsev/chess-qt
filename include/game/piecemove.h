#pragma once

class LPiece;
class LGrid;

class PieceMove
{
public:
    PieceMove(LPiece *_piece = nullptr,
              LGrid *_from = nullptr,
              LGrid *_to = nullptr,
              LPiece *rem = nullptr,
              bool _extra = false,
              bool fake = false);

    LPiece *lpiece;
    LGrid *from, *to;
    LPiece *removed;
    int isNull() const;
    bool extra, fake;
    bool operator==(PieceMove &);
};

