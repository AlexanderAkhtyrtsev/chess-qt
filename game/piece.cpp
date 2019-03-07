#include "piece.h"
#include "freepieces.h"


namespace L { // BEGIN NAMESPACE L

Piece::Piece(Piece::Type pieceType, bool white, Grid *initGrid, Board *l_board)
    : type(pieceType), isWhite(white), lgrid(initGrid), lboard(l_board)
{
    index = -1;
    type = pieceType;
    inGame = true;
    moves = new vector<PieceMove *>;
}

Piece::~Piece()
{
    delete moves;
}
bool Piece::isMoveValid(Grid *g)
{
    Piece *prev = g->lpiece;
    Grid *gprev = lgrid;
    bool ret = true;

    if( prev ) prev->inGame = false;

    lgrid->lpiece = nullptr;
    lgrid = g;
    g->lpiece = this;

    if ( lboard->is_check(isWhite) ) ret = false;

    lgrid = gprev;
    g->lpiece = prev;
    lgrid->lpiece = this;

    if( prev ) prev->inGame = true;
    return ret;
}


void Piece::remove()
{
    this->inGame = false;
    this->lgrid->lpiece = nullptr;
    this->lgrid = nullptr;
}

void Piece::placeTo(Grid *lgrid_to)
{
    assert(lgrid_to != nullptr);
    if (this->lgrid != nullptr)
        this->lgrid->lpiece = nullptr;

    this->lgrid = lgrid_to;
    this->lgrid->lpiece = this;
}


void Piece::clearMoves()
{
    for(PieceMove *it : *moves) {
        delete it;
    }
    moves->clear();
}

// TODO: check
bool Piece::isProtected()
{
    inGame = false;
    lgrid->lpiece = nullptr;
    bool ret = lgrid->is_attacked(isWhite);
    lgrid->lpiece = this;
    inGame = true;
    return ret;
}


void Piece::makeMove(L::Grid *gridTo)
{

    if( lboard->move != isWhite && !gridTo->empty() && gridTo->lpiece->isWhite == isWhite ){
        qDebug() << "makeMove ERROR: wrong move";
        return;
    }

    lboard->currentMove = new PieceMove(this, lgrid, gridTo);
    assert ( !lboard->currentMove->isNull() );
    // Castling

    if (type == King)
    {
        if (lgrid->offset(2, 0) == lboard->currentMove->to)
        {
            Piece *Rook = lgrid->offset(3, 0)->lpiece;
            Rook->placeTo(Rook->lgrid->offset(-2, 0));
            lboard->currentMove->extra = true;
        }
        else if (lgrid->offset(-2, 0) == lboard->currentMove->to)
        {
            Piece *Rook = lgrid->offset(-4, 0)->lpiece;
            Rook->placeTo(Rook->lgrid->offset(3, 0));
            lboard->currentMove->extra = true;
        }
    }

    L::Piece *rm = nullptr;

    //Pawn's extra move
    if (type == Pawn)
    {
        L::Grid *g, *g2;
        L::Piece *f;
        int up = isWhite ? 1 : -1,
            v[] = {1, -1};

        for(int i=0; i<2; i++)

        if ( (g=lgrid->offset(v[i], up)) &&
             g->empty() &&
             g == lboard->currentMove->to &&
             !(g2=g->offset(0, -up))->empty() &&
             (f=g2->lpiece)->type == Pawn &&
              f->isWhite != isWhite
              )
        {
            (rm = f)->remove();
            lboard->currentMove->extra = true;
            break;
        }
    }

    if (!lboard->currentMove->to->empty()) (rm = lboard->currentMove->to->lpiece)->remove();
    if (rm) lboard->currentMove->removed = rm;
    this->placeTo(lboard->currentMove->to);

    if (type == Pawn && lgrid->y == (isWhite ? 7 : 0))
    {
        type = Queen;
        lboard->currentMove->extra = true;
    }
    lboard->move = !lboard->move;
    lboard->moves->push_back(lboard->currentMove);
    this->moves->push_back(lboard->currentMove);
   // lboard->check_game();    // check here
}


bool Piece::moved() const
{
    return moves->size() > 0;
}

vector<Grid *> Piece::getGrids(bool getAttacked) {

    struct behavior
    {
        bool extra, diagonal, straight;
    };

    behavior fb[7] = // Piece behavior
    {
        {0,0,0}, // how do u think ?)))
        {1,0,0}, // King
        {0,1,1}, // Queen
        {0,1,0}, // Bishop
        {1,0,0}, // Knight
        {0,0,1}, // Rook
        {1,0,0} // Pawn
    };

    vector<Grid *> moves;

    if( fb[type].extra )
    {

        if (type == Pawn)
        {

            Grid* g;
            int up = isWhite ? 1 : -1;
            if ( !getAttacked )
            {

                if ( (g=lgrid->offset(0, up)) !=nullptr && g->empty() && isMoveValid(g)){
                    moves.push_back(g);
                }

                // first pos
                if ( lgrid->y == (isWhite?1:6) &&
                     lgrid->offset(0, up)->empty() &&
                     (g=lgrid->offset(0, up*2))->empty() && isMoveValid(g) ){
                    moves.push_back(g);
                }
            }

            // *** //

            if((g=lgrid->offset( 1, up)) && (getAttacked || (!g->empty() && g->lpiece->isWhite != isWhite && isMoveValid(g)) ))
                moves.push_back(g);

            if((g=lgrid->offset(-1, up)) && (getAttacked || (!g->empty() && g->lpiece->isWhite != isWhite && isMoveValid(g)) ))
                moves.push_back(g);



            if ( (g=lgrid->offset(1, 0)) &&
                !g->empty() &&
                g->lpiece->type == Pawn &&
                g->lpiece->isWhite != isWhite &&
                g->lpiece->moves->size() == 1 &&
                 lboard->moves->at(lboard->moves->size() - 1) == g->lpiece->moves->at(g->lpiece->moves->size()-1) &&
                g->lpiece->lgrid->y == (isWhite ? 4 : 3) &&
                (g = g->offset(0, up)) &&
                g->empty() &&
                isMoveValid(g)){ moves.push_back(g); }


            if ( (g=lgrid->offset(-1, 0)) &&
                !g->empty() &&
                g->lpiece->type == Pawn &&
                g->lpiece->isWhite != isWhite &&
                g->lpiece->moves->size() == 1 &&
                lboard->moves->at(lboard->moves->size() - 1) == g->lpiece->moves->at(g->lpiece->moves->size()-1) &&
                g->lpiece->lgrid->y == (isWhite ? 4 : 3) &&
                (g = g->offset(0, up)) &&
                g->empty() &&
                 isMoveValid(g) ){moves.push_back(g);}

        }

        else if(type == King)
        {
            int mv[8][2] = {
              { 1,0}, { 1, 1}, {0, 1}, {-1, 1},
              {-1,0}, {-1,-1}, {0,-1}, { 1,-1}
            };

            for(int i=0; i<8; i++)
            {
                Grid* p_grid = lgrid->offset(mv[i][0], mv[i][1]);
                if (!p_grid) continue;
                if (getAttacked) {moves.push_back(p_grid); continue;}
                if ( p_grid->empty() && (!getAttacked && !p_grid->is_attacked(!isWhite) && isMoveValid(p_grid)))
                {
                    moves.push_back(p_grid);
                }

                else if ( (!p_grid->empty() && p_grid->lpiece->isWhite != isWhite && !p_grid->lpiece->isProtected() && isMoveValid(p_grid)) )
                {
                    moves.push_back(p_grid);
                }
            }

             // Castling
            if ( !getAttacked && !moved() && !lboard->is_check(isWhite) )
            {
                Piece *Rook;
                Grid *grd;
                int y = lgrid->y;
                if ( !lboard->grids[0][y]->empty() &&
                     (Rook = lboard->grids[0][y]->lpiece)->type == Piece::Rook &&
                     !Rook->moved() &&
                     (grd=Rook->lgrid->offset(1, 0))->empty() && !grd->is_attacked(!isWhite) &&
                     (grd=Rook->lgrid->offset(2, 0))->empty() && !grd->is_attacked(!isWhite) &&
                     (grd=Rook->lgrid->offset(3, 0))->empty() && !grd->is_attacked(!isWhite)
                     )
                {
                    moves.push_back(lgrid->offset(-2, 0));
                }
                if ( !lboard->grids[7][y]->empty() &&
                     (Rook = lboard->grids[7][y]->lpiece)->type == Piece::Rook &&
                     !Rook->moved() &&
                     (grd=Rook->lgrid->offset(-1, 0))->empty() && !grd->is_attacked(!isWhite) &&
                     (grd=Rook->lgrid->offset(-2, 0))->empty() && !grd->is_attacked(!isWhite)
                     )
                {
                    moves.push_back(lgrid->offset(2, 0));
                }
            }
        }

        else // Knight
        {
            int offsets[8][2] = { {1,2}, {2,1}, {-1,2},  {-2, 1},{-1, -2}, {1, -2},{-2, -1}, {2, -1}};
            for(int i=0; i<8; i++) {
                Grid* g = lgrid->offset(offsets[i][0], offsets[i][1]);
                if(g && (getAttacked ||
                         ((g->empty() || g->lpiece->isWhite != isWhite) &&
                          isMoveValid(g))) )
                {
                    moves.push_back(g);
                }
           }
        }
    }
    else
    {

        if ( fb[type].diagonal )
        {
            // right & up
            for(int x = lgrid->x+1, y = lgrid->y+1; x<8 && y < 8; x++, y++)
            {
                Grid* off = lgrid->offset(x-lgrid->x, y-lgrid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }

            // left && down
            for(int x = lgrid->x-1, y = lgrid->y-1; x>=0 && y >= 0; x--, y--)
            {

                Grid* off = lgrid->offset(x-lgrid->x, y-lgrid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
            // up && left
            for(int x = lgrid->x-1, y = lgrid->y+1; x>=0 && y < 8; x--, y++)
            {

                Grid* off = lgrid->offset(x-lgrid->x, y-lgrid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }

            for(int x = lgrid->x+1, y = lgrid->y-1; x<8 && y >= 0; x++, y--)
            {

                Grid* off = lgrid->offset(x-lgrid->x, y-lgrid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }


        }
        if ( fb[type].straight )
        {

            // X
            for(int i=1; i < 8-lgrid->x; i++) // right
            {
                Grid* off = lgrid->offset(i, 0);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
            for(int i=lgrid->x-1; i>=0; i--) // left
            {
                Grid* off = lgrid->offset(i-lgrid->x, 0);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }

            // Y
            for(int i=1; i < 8-lgrid->y; i++) // top
            {

                Grid* off = lgrid->offset(0, i);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
            for(int i=lgrid->y-1; i >= 0; i--) // bottom
            {

                Grid *off = lgrid->offset(0, i-lgrid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->lpiece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
        }
    }

    return moves;
}

} // END NAMESPACE L


Piece::Piece(L::Piece *l_piece, Board *_board)
: QGraphicsObject(),  lpiece(l_piece), board(_board)
{
    anim = new QPropertyAnimation(this, "pos");
    setZValue(1);
}

Piece::~Piece()
{
    // delete anim;
}

void Piece::placeTo(Grid *grid_to)
{
    this->lpiece->placeTo(grid_to->lgrid);
    this->grid->piece = nullptr;
    this->grid = grid_to;
    this->grid->piece = this;
    this->setPos(grid->pos());
}


void Piece::makeMove(Grid *gridTo)
{

    if ( board->options->flipBoard ) {
        board->reverse(board->lboard->move);
    }
    this->lpiece->makeMove(gridTo->lgrid);
    assert ( !lpiece->lboard->currentMove->isNull() );

    board->selected = nullptr;
    board->piecesSelectable = false;

    this->setZValue(2);
    this->animateTo(gridTo, true);

    // Castling
    if ( lpiece->lboard->currentMove->extra && lpiece->type == L::Piece::King)
    {
        if (grid->offset(2, 0)->lgrid == lpiece->lboard->currentMove->to)
        {
            Piece *Rook = grid->offset(3, 0)->piece;
            Rook->animateTo(Rook->grid->offset(-2, 0));
        }
        else if (grid->offset(-2, 0)->lgrid == lpiece->lboard->currentMove->to)
        {
            Piece *Rook = grid->offset(-4, 0)->piece;
            Rook->animateTo(Rook->grid->offset(3, 0));
        }

    }
    board->ResetHighligtedGrids();
}

void Piece::moveEnd()
{
    assert ( !lpiece->lboard->currentMove->isNull() );

    Piece *rm = Piece::get(lpiece->lboard->currentMove->removed, board);

    if (rm) rm->remove();

    this->placeTo(Grid::get(lpiece->lboard->currentMove->to, board));


    Grid::get(lpiece->lboard->currentMove->from, board)->Highlight(2);
    Grid::get(lpiece->lboard->currentMove->to, board)->Highlight(2);
    board->piecesSelectable = true;
    this->setZValue(1);
    anim->disconnect();
    if (!board->lboard->move) {
        QTimer::singleShot(300, board, &Board::computerMove);
    }

}

void Piece::animateTo(Grid *gridTo, bool moveEnd)
{

    if (moveEnd){
        QObject::connect(anim, SIGNAL(finished()), SLOT(moveEnd()));
    }
    anim->setDuration( qMax(400, qMax(qAbs(grid->lgrid->x - gridTo->lgrid->x), qAbs(grid->lgrid->y - gridTo->lgrid->y)) * 100) );
    anim->setStartValue(this->grid->pos());
    anim->setEndValue(gridTo->pos());
    anim->setEasingCurve(QEasingCurve::InQuad);
    anim->start();
}

void Piece::select()
{
    if( !lpiece->inGame || board->lboard->move != lpiece->isWhite || !board->piecesSelectable ) return;

    board->ResetHighligtedGrids();
    vector<L::Grid *> availMoves = lpiece->getGrids();
    board->selected = this;
    grid->Highlight();
    if ( availMoves.size() )
    {
        for(unsigned i=0; i<availMoves.size(); i++)
        {
            Grid::get(availMoves[i], board)->Highlight();
        }
    }
}

void Piece::remove()
{
    // l_remove
    board->free_pieces[lpiece->isWhite]->addPiece(this);
}

Piece *Piece::get(L::Piece *lp, Board *board)
{
    if (!lp || !board || lp->index < 0) return nullptr;
    return board->pieces->at(quint32(lp->index));
}


QRectF Piece::boundingRect() const
{
    return QRectF(0, 0, board->grid_size, board->grid_size);
}

void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPixmap fig = board->chess_tiles->scaled(board->grid_size * 6, board->grid_size * 2);

    painter->save();
        painter->drawPixmap(0, 0, fig,
                           (lpiece->type-1) * board->grid_size, lpiece->isWhite ? 0 : board->grid_size,
                           board->grid_size, board->grid_size);
    painter->restore();
}



void Piece::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
    if ( pe->button() == Qt::LeftButton && board->lboard->move == lpiece->isWhite ) this->select();
    QGraphicsItem::mousePressEvent(pe);
}
