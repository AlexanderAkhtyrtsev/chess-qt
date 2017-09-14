#include "figure.h"
#include "freefigures.h"


Figure::Figure(Figure::Type _type, bool white, Grid *place, Board *_board)
: QGraphicsObject()
{

    type = _type;
    is_white = white;
    in_game = true;
    board = _board;
    next_move = 0;
    moves = new vector<FigureMove *>;
    grid = 0;
    placeTo(place);
    setZValue(1);
}

void Figure::placeTo(Grid *gridp)
{
    if( grid && !grid->empty() ) { grid->figure = 0; }
    grid = board->grids[gridp->x][gridp->y];
    grid->figure = this;
    setPos(grid->pos().x(), grid->pos().y());
}


void Figure::move(Grid *to)
{
    if( !to->empty() && to->figure->is_white == is_white ) return;

    if ( board->options->flipBoard ) {
        board->reverse(board->move);
    }

    board->ReplaceElements();

    anim = new QPropertyAnimation(this, "pos");
    anim->setDuration( 500 );
    anim->setStartValue(grid->pos());
    anim->setEndValue(to->pos());
    anim->setEasingCurve(QEasingCurve::InQuad);
    anim->start();


    next_move = to;
    board->selected = 0;
    board->move = !board->move;
    board->figures_selectable = false;
    QObject::connect(anim, SIGNAL(finished()), SLOT(moveEnd()));
    board->ResetHighligtedGrids();
    setZValue(2);
}

bool Figure::move_valid(Grid *g)
{
    Figure *prev = g->figure;
    Grid *gprev = grid;
    bool ret = true;

    if( prev ) prev->in_game = false;

    grid->figure = 0;
    grid = g;
    g->figure = this;

    if ( board->is_check(is_white) ) ret = false;

    grid = gprev;
    g->figure = prev;
    grid->figure = this;

    if( prev ) prev->in_game = true;
    return ret;
}


void Figure::Select()
{
    if( !in_game || board->move != is_white ||  !board->figures_selectable ) return;
        board->ResetHighligtedGrids();
    if ( !getGrids().size() ) return;
        board->selected = this;
        grid->update(grid->boundingRect());
        vector<Grid *> mv = getGrids();
        for(unsigned i=0; i<mv.size(); i++)
            mv[i]->Highlight();
        grid->Highlight();
}

void Figure::Remove()
{
    grid->figure = 0;
    grid = 0;
    in_game = false;
    board->free_figures[is_white]->addFigure(this);
}

vector<Grid *> Figure::getGrids(bool getAttacked)
{

    struct behavior
    {
        bool    extra,
                diagonal,
                straight;
    };

    behavior fb[7] = // Figure behavior
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
            int up = is_white ? 1 : -1;
            if ( !getAttacked )
            {

                if ( (g=grid->Offset(0, up)) !=0 && g->empty() && move_valid(g))
                    moves.push_back(g);

                if ( grid->y == (is_white?1:6) && grid->Offset(0, up)->empty() && (g=grid->Offset(0, up*2))->empty() && move_valid(g) ) // first pos
                    moves.push_back(g);
            }

            // *** //
            if( (g=grid->Offset(-1, up))  &&  (getAttacked || (!g->empty() && g->figure->is_white != is_white && move_valid(g)) ))
                moves.push_back(g);


            if( (g=grid->Offset(1, up)) && ( getAttacked || (!g->empty() && g->figure->is_white != is_white && move_valid(g)) ) )
                    moves.push_back(g);

            if ( (g=grid->Offset(1, 0)) &&
                !g->empty() &&
                g->figure->type == Pawn &&
                g->figure->is_white != is_white &&
                g->figure->moves->size() == 1 &&
                 board->moves->at(board->moves->size() - 1) == g->figure->moves->at(g->figure->moves->size()-1) &&
                g->figure->grid->y == (is_white ? 4 : 3) &&
                (g = g->Offset(0, up)) &&
                g->empty() &&
                move_valid(g)) moves.push_back(g);


            if ( (g=grid->Offset(-1, 0)) &&
                !g->empty() &&
                g->figure->type == Pawn &&
                g->figure->is_white != is_white &&
                g->figure->moves->size() == 1 &&
                board->moves->at(board->moves->size() - 1) == g->figure->moves->at(g->figure->moves->size()-1) &&
                g->figure->grid->y == (is_white ? 4 : 3) &&
                (g = g->Offset(0, up)) &&
                g->empty() &&
                move_valid(g) ) moves.push_back(g);

        }

        else if( type == King)
        {
            int mv[8][2] = {
              {1,0}, {1,1}, {0,1}, {-1,1},
              {-1,0}, {-1,-1}, {0,-1}, {1,-1}
            };

            for(int i=0; i<8; i++)
            {
                Grid* p_grid = grid->Offset(mv[i][0],mv[i][1]);
                if (!p_grid) continue;

                if ( p_grid->empty() && (getAttacked || (!getAttacked && !p_grid->is_attacked(!is_white) && move_valid(p_grid))) )
                {
                  moves.push_back(p_grid);
                }

                else if ( (!p_grid->empty() && p_grid->figure->is_white != is_white && !p_grid->figure->isProtected() && move_valid(p_grid)) )
                    moves.push_back(p_grid);

            }
             // Castling
            if ( !getAttacked && !moved() && !board->is_check(is_white) )
            {
                Figure *Rook;
                Grid *grd;
                int y = grid->y;
                if ( !board->grids[0][y]->empty() &&
                     (Rook = board->grids[0][y]->figure)->type == Figure::Rook &&
                     !Rook->moved() &&
                     (grd=Rook->grid->Offset(1, 0))->empty() && !grd->is_attacked(!is_white) &&
                     (grd=Rook->grid->Offset(2, 0))->empty() && !grd->is_attacked(!is_white) &&
                     (grd=Rook->grid->Offset(3, 0))->empty() && !grd->is_attacked(!is_white)
                     )
                {
                    moves.push_back(grid->Offset(-2, 0));
                }
                if ( !board->grids[7][y]->empty() &&
                     (Rook = board->grids[7][y]->figure)->type == Figure::Rook &&
                     !Rook->moved() &&
                     (grd=Rook->grid->Offset(-1, 0))->empty() && !grd->is_attacked(!is_white) &&
                     (grd=Rook->grid->Offset(-2, 0))->empty() && !grd->is_attacked(!is_white)
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
                         ((g->empty() || g->figure->is_white != is_white) &&
                          move_valid(g))) )
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
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }

            // left && down
            for(int x = grid->x-1, y = grid->y-1; x>=0 && y >= 0; x--, y--)
            {

                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }
            // up && left
            for(int x = grid->x-1, y = grid->y+1; x>=0 && y < 8; x--, y++)
            {

                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }

            for(int x = grid->x+1, y = grid->y-1; x<8 && y >= 0; x++, y--)
            {

                Grid* off = grid->Offset(x-grid->x, y-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off)) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
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
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }
            for(int i=grid->x-1; i>=0; i--) // left
            {
                Grid* off = grid->Offset(i-grid->x, 0);
                if(!off->empty())
                {
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }

            // Y
            for(int i=1; i < 8-grid->y; i++) // top
            {

                Grid* off = grid->Offset(0, i);
                if(!off->empty())
                {
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }
            for(int i=grid->y-1; i >= 0; i--) // bottom
            {

                Grid* off = grid->Offset(0, i-grid->y);
                if(!off->empty())
                {
                    if ( getAttacked || (off->figure->is_white != is_white && move_valid(off) ) )
                      moves.push_back(off);
                   break;
                }
                if( getAttacked || move_valid(off)) moves.push_back(off);
            }
        }
    }

    return moves;
}

// TODO: check
bool Figure::isProtected()
{
    in_game = false;
    grid->figure = 0;
    bool ret = grid->is_attacked(is_white);
    grid->figure = this;
    in_game = true;
    return ret;
}

QRectF Figure::boundingRect() const
{
    return QRectF(0, 0, board->grid_size, board->grid_size);
}

void Figure::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPixmap fig = board->chess_tiles->scaled(board->grid_size * 6, board->grid_size * 2);

    painter->save();
        painter->drawPixmap(0, 0, fig,
                           (type-1) * board->grid_size, is_white ? 0 : board->grid_size,
                           board->grid_size, board->grid_size);
    painter->restore();
}

bool Figure::moved() const
{
    return moves->size() > 0;
}

void Figure::moveEnd()
{
    Figure *rm = 0;
    bool extra = false;
    if ( !next_move ) return;

    // Castling
    if ( type == King )
    {
        if (grid->Offset(2, 0) == next_move)
        {
            Figure *Rook = grid->Offset(3, 0)->figure;
            Rook->placeTo(Rook->grid->Offset(-2, 0));
            extra = true;
        }
        else if (grid->Offset(-2, 0) == next_move)
        {
            Figure *Rook = grid->Offset(-4, 0)->figure;
            Rook->placeTo(Rook->grid->Offset(3, 0));
            extra = true;
        }
    }
    // Pawn's extra move
    else if (type == Pawn)
    {
        Grid *g, *g2;
        Figure *f;
        int up = is_white ? 1 : -1;
        int v[] = {1, -1};
        for(int i=0; i<2; i++)
        if ( (g=grid->Offset(v[i], up)) &&
             g->empty() &&
             g == next_move &&
             !(g2=g->Offset(0, -up))->empty() &&
             (f=g2->figure)->is_white != is_white &&
             f->type == Pawn )
        {
            (rm = f)->Remove();
            extra = true;
            break;
        }
    }

    if (!next_move->empty()) (rm = next_move->figure)->Remove();
    FigureMove *cur_move = new FigureMove(this, grid, next_move, rm);

    grid->figure = 0;
    grid = next_move;
    grid->figure = this;

    if (type == Pawn && grid->y == (is_white ? 7 : 0))
    {
        type = Queen;
        extra = true;
    }

    next_move = 0;
    cur_move->extra = extra;
    board->moves->push_back(cur_move);
    moves->push_back(cur_move);
    board->check_game();    
    board->figures_selectable = true;
    setZValue(1);
}

void Figure::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
    if ( pe->button() == Qt::LeftButton && board->move == is_white ) Select();
    QGraphicsItem::mousePressEvent(pe);
}
