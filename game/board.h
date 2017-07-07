#pragma once

#include <QtWidgets>
#include <vector>
#include <algorithm>



using std::vector;
class Figure;
class Grid;
class FigureMove;
class FreeFigures;


class Board : public QGraphicsScene
{
    bool _reverse;
public:
    Board(QWidget* = 0);
    ~Board();
    static const int fig_pos[8][8];
    void newGame();
    void PaintGrids(bool = 0);
    void ReplaceElements();
    bool is_check(bool w);
    void check_game();
    void undoMove();
    Board *ResetHighligtedGrids();

    int grid_size;
    bool move;
    bool reverse() const; // getter
    bool reverse(bool);     // setter
    bool figures_selectable;
    Grid* grids[8][8];
    QPixmap* chess_tiles;
    vector<FigureMove *> *moves;
    vector<Figure *> *figures;
    vector<Figure *> *figures_w, *figures_b;
    Figure *selected, *King[2];
    FreeFigures *free_figures[2];
    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class FigureMove
{
public:
    FigureMove(Figure *_figure, Grid *_from, Grid *_to, Figure *rem , bool _extra = false);
    Figure *figure;
    Grid *from, *to;
    Figure *removed;
    bool extra;
};
