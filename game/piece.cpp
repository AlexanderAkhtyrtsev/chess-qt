#include "piece.h"
#include "freepieces.h"


Piece::Piece(Piece::Type _type, bool white, Grid *place, Board *_board)
: QGraphicsObject()
{
    type = _type;
    isWhite = white;
    inGame = true;
    board = _board;
    currentMove = 0;
    moves = new vector<PieceMove *>;
    grid = 0;
    placeTo(place);
    setZValue(1);
}

void Piece::placeTo(Grid *gridp, bool show)
{
    if( this->grid && !this->grid->empty() ) {
        this->grid->piece = 0;
    }
    this->grid = board->grids[gridp->x][gridp->y];
    this->grid->piece = this;
    if (show) {
        setPos(this->grid->pos().x(), this->grid->pos().y());
    }
}


void Piece::makeMove(Grid *gridTo, bool show)
{

    if( board->move != this->isWhite && !gridTo->empty() && gridTo->piece->isWhite == isWhite ){
        qDebug() << "makeMove ERROR: wrong move";
        return;
    }

    if (show) {
        if ( board->options->flipBoard ) {
            board->reverse(board->move);
        }

        board->ReplaceElements();
        board->selected = 0;
    }

    currentMove = new PieceMove(this, this->grid, gridTo);

    // ** GUI **
    if (show) {
        board->pieces_selectable = false;
        this->animateTo(gridTo);
        QObject::connect(anim, SIGNAL(finished()), SLOT(moveEnd()));
        setZValue(2);

        // Castling
        if ( type == King )
        {
            if (grid->Offset(2, 0) == currentMove->to)
            {
                Piece *Rook = grid->Offset(3, 0)->piece;
                Rook->animateTo(Rook->grid->Offset(-2, 0), true);
                currentMove->extra = true;
            }
            else if (grid->Offset(-2, 0) == currentMove->to)
            {
                Piece *Rook = grid->Offset(-4, 0)->piece;
                Rook->animateTo(Rook->grid->Offset(3, 0), true);
                currentMove->extra = true;
            }
        }

    }
    else {
        moveEnd(false);
    }
    // *******
    if (show){
        board->ResetHighligtedGrids();
    }
}

void Piece::animateTo(Grid *gridTo, bool place)
{
    anim = new QPropertyAnimation(this, "pos");

    anim->setDuration( qMax(400, qMax(qAbs(grid->x-gridTo->x), qAbs(grid->y-gridTo->y))*100) );
    anim->setStartValue(this->grid->pos());
    anim->setEndValue(gridTo->pos());
    anim->setEasingCurve(QEasingCurve::InQuad);
    anim->start();
    if (place){
        this->placeTo(gridTo, false);
    }
}

bool Piece::isMoveValid(Grid *g)
{
    Piece *prev = g->piece;
    Grid *gprev = grid;
    bool ret = true;

    if( prev ) prev->inGame = false;

    grid->piece = 0;
    grid = g;
    g->piece = this;

    if ( board->is_check(isWhite) ) ret = false;

    grid = gprev;
    g->piece = prev;
    grid->piece = this;

    if( prev ) prev->inGame = true;
    return ret;
}


void Piece::select()
{
    if( !inGame || board->move != isWhite ||  !board->pieces_selectable ) return;
        board->ResetHighligtedGrids();
    if ( !getGrids().size() ) return;
        board->selected = this;
        grid->update(grid->boundingRect());
        vector<Grid *> mv = getGrids();
        for(unsigned i=0; i<mv.size(); i++)
            mv[i]->Highlight();
        grid->Highlight();
}

void Piece::remove(bool show)
{
    this->grid->piece = 0;
    this->grid = 0;
    inGame = false;
    if (show) board->free_pieces[isWhite]->addPiece(this);
}

vector<Grid *> Piece::getGrids(bool getAttacked)
{

    struct behavior
    {
        bool    extra,
                diagonal,
                straight;
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

                if ( (g=grid->Offset(0, up)) !=0 && g->empty() && isMoveValid(g))
                    moves.push_back(g);

                if ( grid->y == (isWhite?1:6) && grid->Offset(0, up)->empty() && (g=grid->Offset(0, up*2))->empty() && isMoveValid(g) ) // first pos
                    moves.push_back(g);
            }

            // *** //
            if( (g=grid->Offset(-1, up))  &&  (getAttacked || (!g->empty() && g->piece->isWhite != isWhite && isMoveValid(g)) ))
                moves.push_back(g);


            if( (g=grid->Offset(1, up)) && ( getAttacked || (!g->empty() && g->piece->isWhite != isWhite && isMoveValid(g)) ) )
                    moves.push_back(g);

            if ( (g=grid->Offset(1, 0)) &&
                !g->empty() &&
                g->piece->type == Pawn &&
                g->piece->isWhite != isWhite &&
                g->piece->moves->size() == 1 &&
                 board->moves->at(board->moves->size() - 1) == g->piece->moves->at(g->piece->moves->size()-1) &&
                g->piece->grid->y == (isWhite ? 4 : 3) &&
                (g = g->Offset(0, up)) &&
                g->empty() &&
                isMoveValid(g)) moves.push_back(g);


            if ( (g=grid->Offset(-1, 0)) &&
                !g->empty() &&
                g->piece->type == Pawn &&
                g->piece->isWhite != isWhite &&
                g->piece->moves->size() == 1 &&
                board->moves->at(board->moves->size() - 1) == g->piece->moves->at(g->piece->moves->size()-1) &&
                g->piece->grid->y == (isWhite ? 4 : 3) &&
                (g = g->Offset(0, up)) &&
                g->empty() &&
                isMoveValid(g) ) moves.push_back(g);

        }

        else if(type == King)
        {
            int mv[8][2] = {
              { 1,0}, { 1, 1}, {0, 1}, {-1, 1},
              {-1,0}, {-1,-1}, {0,-1}, { 1,-1}
            };

            for(int i=0; i<8; i++)
            {
                Grid* p_grid = grid->Offset(mv[i][0],mv[i][1]);
                if (!p_grid) continue;

                if ( p_grid->empty() && (getAttacked || (!getAttacked && !p_grid->is_attacked(!isWhite) && isMoveValid(p_grid))) )
                {
                  moves.push_back(p_grid);
                }

                else if ( (!p_grid->empty() && p_grid->piece->isWhite != isWhite && !p_grid->piece->isProtected() && isMoveValid(p_grid)) )
                    moves.push_back(p_grid);

            }
             // Castling
            if ( !getAttacked && !moved() && !board->is_check(isWhite) )
            {
                Piece *Rook;
                Grid *grd;
                int y = grid->y;
                if ( !board->grids[0][y]->empty() &&
                     (Rook = board->grids[0][y]->piece)->type == Piece::Rook &&
                     !Rook->moved() &&
                     (grd=Rook->grid->Offset(1, 0))->empty() && !grd->is_attacked(!isWhite) &&
                     (grd=Rook->grid->Offset(2, 0))->empty() && !grd->is_attacked(!isWhite) &&
                     (grd=Rook->grid->Offset(3, 0))->empty() && !grd->is_attacked(!isWhite)
                     )
                {
                    moves.push_back(grid->Offset(-2, 0));
                }
                if ( !board->grids[7][y]->empty() &&
                     (Rook = board->grids[7][y]->piece)->type == Piece::Rook &&
                     !Rook->moved() &&
                     (grd=Rook->grid->Offset(-1, 0))->empty() && !grd->is_attacked(!isWhite) &&
                     (grd=Rook->grid->Offset(-2, 0))->empty() && !grd->is_attacked(!isWhite)
                     )
                {
                    moves.push_back(grid->Offset(2, 0));
                }
            }

        }

        else // Knight
        {
            int offsets[8][2] = { {1,2}, {2,1}, {-1,2},  {-2, 1},{-1, -2}, {1, -2},{-2, -1}, {2, -1}};
            for(int i=0; i<8; i++) {
                Grid* g = grid->Offset( offsets[i][0], offsets[i][1]);
                if(g && (getAttacked ||
                         ((g->empty() || g->piece->isWhite != isWhite) &&
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
            for(int x = grid->x+1, y = grid->y+1; x<8 && y < 8; x++, y++)
            {
                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }

            // left && down
            for(int x = grid->x-1, y = grid->y-1; x>=0 && y >= 0; x--, y--)
            {

                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
            // up && left
            for(int x = grid->x-1, y = grid->y+1; x>=0 && y < 8; x--, y++)
            {

                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }

            for(int x = grid->x+1, y = grid->y-1; x<8 && y >= 0; x++, y--)
            {

                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }


        }
        if ( fb[type].straight )
        {

            // X
            for(int i=1; i < 8-grid->x; i++) // right
            {
                Grid* off = grid->Offset(i, 0);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
            for(int i=grid->x-1; i>=0; i--) // left
            {
                Grid* off = grid->Offset(i-grid->x, 0);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }

            // Y
            for(int i=1; i < 8-grid->y; i++) // top
            {

                Grid* off = grid->Offset(0, i);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
            for(int i=grid->y-1; i >= 0; i--) // bottom
            {

                Grid *off = grid->Offset(0, i-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->piece->isWhite != isWhite && isMoveValid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || isMoveValid(off)) moves.push_back(off);
            }
        }
    }

    return moves;
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
    grid->piece = 0;
    bool ret = grid->is_attacked(isWhite);
    grid->piece = this;
    inGame = true;
    return ret;
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
                           (type-1) * board->grid_size, isWhite ? 0 : board->grid_size,
                           board->grid_size, board->grid_size);
    painter->restore();
}

bool Piece::moved() const
{
    return moves->size() > 0;
}

void Piece::moveEnd(bool show)
{
    if ( currentMove->isNull() ) return;
    Piece *rm = 0;


    //Pawn's extra move
    if (type == Pawn)
    {
        Grid *g, *g2;
        Piece *f;
        int up = isWhite ? 1 : -1;
        int v[] = {1, -1};
        for(int i=0; i<2; i++)
        if ( (g=grid->Offset(v[i], up)) &&
             g->empty() &&
             g == currentMove->to &&
             !(g2=g->Offset(0, -up))->empty() &&
             (f=g2->piece)->isWhite != isWhite &&
             f->type == Pawn )
        {
            (rm = f)->remove(show);
            currentMove->extra = true;
            break;
        }
    }

    if (!currentMove->to->empty()) (rm = currentMove->to->piece)->remove(show);
    if (rm) currentMove->removed = rm;
    this->placeTo(currentMove->to, show);

    if (type == Pawn && grid->y == (isWhite ? 7 : 0))
    {
        type = Queen;
        currentMove->extra = true;
    }
    board->move = !board->move;
    board->moves->push_back(currentMove);
    this->moves->push_back(currentMove);
    if (show) {
        board->ResetHighligtedGrids();
    }
    board->check_game(show);    // check here
    if (show)
    {
        currentMove->from->Highlight(2);
        currentMove->to->Highlight(2);
        board->pieces_selectable = true;
        setZValue(1);
        if (!board->move) QTimer::singleShot(300, board, &Board::computerMove);
    }
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
    if ( pe->button() == Qt::LeftButton && board->move == isWhite ) select();
    QGraphicsItem::mousePressEvent(pe);
}
