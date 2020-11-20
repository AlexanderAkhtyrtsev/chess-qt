#include "piecemove.h"
#include "board.h"
#include "piece.h"

PieceMove::PieceMove(LPiece *_piece, LGrid *_from, LGrid *_to, LPiece *rem, bool _extra, bool fake)
: lpiece(_piece), from(_from), to(_to), removed(rem), extra(_extra), fake(fake)
{
}

/***
*  PieceMove::isNull returns lpiece=1, grid_from=2, grid_to=3
*/
int PieceMove::isNull() const
{
    return (!this->lpiece ? 1 : !this->from ? 2 : !this->to ? 3 : 0);
}

bool PieceMove::operator==(PieceMove &otherMove)
{
    return (this->extra == otherMove.extra &&
            this->lpiece->index == otherMove.lpiece->index &&
            this->from->name() == otherMove.from->name() &&
            this->to->name() == otherMove.to->name()
            // && removed == removed
            );
}

