#pragma once

#include "mainwindow.h"
#include "board.h"
#include "piece.h"



class Grid : public QGraphicsItem
{
    int light;
public:
    Piece *piece;
    L::Grid *lgrid;
    Board *board;
    Grid(L::Grid *lgrid, Board* = nullptr);
    ~Grid();
    void Highlight(int = 1);
    operator L::Grid () const;
    Grid *offset(int dx, int dy);
    static Grid *get(L::Grid *, Board *board);
    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};


namespace L {

class Grid {
public:
    Grid(int _x = 0, int _y = 0, L::Board *l_board = nullptr);
    L::Grid *offset(int x, int y);
    bool is_attacked(bool w);
    QString name();
    bool empty() const;
    vector<Piece *> attackedBy(bool w);
    int x, y;
    L::Piece* lpiece;
    L::Board* lboard;
};

}

