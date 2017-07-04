#pragma once

#include <QtWidgets>
#include <vector>
#include <algorithm>
using std::vector;
class Figure;
class Grid;
class FigureMove;

class Board : public QGraphicsScene
{

public:
    Board(QWidget* = 0);
    ~Board();
    int grid_size;
    bool move;
    bool reverse;
    bool figures_selectable;
    Grid* grids[8][8];
    int data[8][8];
    QPixmap* chess_tiles;
    vector<FigureMove *> *moves;
    vector<Figure *> *figures;
    vector<Figure *> *figures_w, *figures_b;
    Figure *selected, *King[2];
    Board* ResetHighligtedGrids();
    void PaintGrids(bool = 0);
    void ReplaceElements();
    void update_data(); // ??
    bool is_check(bool w);
    void check_game();
    void undoMove();
    void newGame();
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
