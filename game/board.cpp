#include "board.h"
#include "figure.h"
#include "freefigures.h"


const int Board::fig_pos[8][8] = {
    { 5, 4, 3, 2, 1, 3, 4, 5 },
    { 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    {-6,-6,-6,-6,-6,-6,-6,-6 },
    {-5,-4,-3,-2,-1,-3,-4,-5 }
};

Board::Board(QWidget* parent) :
    QGraphicsScene(parent)
{
    grid_size = parent->width() / 8;
    setSceneRect(0, 0, 8*grid_size, 8*grid_size);
    chess_tiles = new QPixmap(":/img/chess.png");

    PaintGrids();

    selected            = 0;
    move                = true; // true - white
    _reverse            = false;
    figures_selectable  = true;
    figures             = new vector<Figure     *>;
    figures_w           = new vector<Figure     *>;
    figures_b           = new vector<Figure     *>;
    moves               = new vector<FigureMove *>;

    options = new Options;

    free_figures[0] = new FreeFigures(this);
    free_figures[1] = new FreeFigures(this); // white

    Figure *temp_fig;
     for(int i=0; i<8; i++)
         for(int j=0; j<8; j++)
         {
             if ( !fig_pos[i][j] ) continue;
             temp_fig = new Figure((Figure::Type) (fig_pos[i][j] > 0 ? fig_pos[i][j] : -fig_pos[i][j]), fig_pos[i][j] > 0, grids[j][i], this);
             figures->push_back(temp_fig);
             (temp_fig->is_white ? figures_w : figures_b)->push_back(temp_fig);
         }

    King[1] = figures->at(4);
    King[0] = figures->at(28);

     for(unsigned i=0; i<figures->size(); i++){
         addItem(figures->at(i));
         //if( figures->at(i)->type != Figure::King )  figures->at(i)->Remove();
     }

}

Board::~Board()
{
    delete chess_tiles;
    delete figures;
    delete figures_w;
    delete figures_b;
    // delete grids;
}

// TODO: check this one
void Board::newGame()
{
    selected = 0;
    move  = true; // true - white
    figures_selectable = true;
    moves->clear();
    reverse(!move);
    free_figures[0] = new FreeFigures(this);
    free_figures[1] = new FreeFigures(this);

    Figure *p_figure;
    int f_count = 0;
    for(int x = 0; x < 8; x++)
    {
        for(int y=0; y<8; y++)
        {
            if ( !fig_pos[x][y] ) continue;
            p_figure = figures->at(f_count);
            p_figure->in_game = true;
            p_figure->placeTo(grids[y][x]);
            if (p_figure->type == Figure::Queen && qAbs( fig_pos[x][y] ) == Figure::Pawn ){
                p_figure->type = Figure::Pawn;
            }
            f_count ++;
        }
    }
    ResetHighligtedGrids();
}


bool Board::is_check(bool w)
{
    return King[w]->grid->is_attacked(!w);
}

void Board::check_game()
{
    QString messagebox_text = "Check mate!";
    if ( is_check(0) ) King[0]->grid->Highlight();
    else if ( is_check(1) ) King[1]->grid->Highlight();
    else messagebox_text = "Draw!";
    bool game_over = true;
    vector<Figure *> *figures = move ? figures_w : figures_b;
    Figure *temp;
    for(unsigned i = 0; i < figures->size(); i++)
    {
        temp = figures->at(i);
        if ( temp->in_game && temp->getGrids().size() > 0 )
        {
            game_over = false;
            break;
        }
    }

    // TODO : check if draw

    if (game_over) {
        window->box->setText(messagebox_text);
        window->box->show();
    }
}

void Board::undoMove()
{
    if (!moves->size()) return;
    FigureMove* last = moves->at(moves->size() - 1);


    if (last->extra)
    {
        // Castling
        if (last->figure->type == Figure::King)
        {
            if ( last->figure->grid->x == 2)
            {
                Figure *Rook = last->figure->grid->Offset(1,0)->figure;
                Rook->placeTo(Rook->grid->Offset(-3,0));
            }
            else if ( last->figure->grid->x == 6)
            {
                Figure *Rook = last->figure->grid->Offset(-1,0)->figure;
                Rook->placeTo(Rook->grid->Offset(2,0));
            }
        }
        else
        {
            last->figure->type = Figure::Pawn;
        }
    }
    last->figure->placeTo(last->from);
    last->to->figure = 0;

    if ( last->removed )
    {
        int offsetY = 0;

        if (last->extra && last->figure->type == Figure::Pawn && last->to->y != ( last->figure->is_white ? 7 : 0 )) {
            offsetY = last->figure->is_white ? -1 : 1;
        }

        last->removed->in_game = true;
        last->removed->placeTo(last->to->Offset(0, offsetY));
        free_figures[last->removed->is_white]->removeFigure(last->removed);
    }
   last->figure->moves->erase(last->figure->moves->end()-1);
   moves->erase( moves->end() - 1 );
   move = !move;
   if (!moves->size()) reverse( !move );
   ResetHighligtedGrids();
}



Board *Board::ResetHighligtedGrids()
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            grids[i][j]->Highlight(0);
    if ( is_check(move) )
         King[move]->grid->Highlight();
    return this;
}

bool Board::reverse() const
{
    return _reverse;
}

bool Board::reverse(bool __reverse)
{
    _reverse = __reverse;
    ReplaceElements();
    free_figures[0]->update();
    free_figures[1]->update();
    return _reverse;
}

void Board::PaintGrids(bool rm)
{

    if( rm )
        for(int i=0; i<8; i++)for(int j=0; j<8; j++)
            this->removeItem(grids[i][j]);

    bool w = !1;
    for(int i=0; i<8; i++)
    {

        for(int j=0; j<8; j++)
        {
            grids[i][j] = new Grid(w, i, j, this);
            addItem(grids[i][j]);
            w = !w;
        }
        w = !w;
    }
}

void Board::ReplaceElements()
{
    for(unsigned i=0; i<8; i++)
        for(unsigned j=0; j<8; j++)
            grids[i][j]->setPos(grid_size * i, grid_size * (2 + (_reverse ? j : 7-j)));

    for(unsigned i=0; i<figures->size(); i++)
    {
        Figure* f = figures->at(i);
        if ( f->in_game ) f->placeTo(f->grid);
    }
    free_figures[0]->update();
    free_figures[1]->update();
}


void Board::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}



FigureMove::FigureMove(Figure *_figure, Grid *_from, Grid *_to, Figure *rem, bool _extra)
: figure(_figure), from(_from), to(_to), removed(rem), extra(_extra)
{

}
