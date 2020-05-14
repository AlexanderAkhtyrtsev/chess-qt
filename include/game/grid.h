#pragma once

#include "mainwindow.h"
#include "board.h"
#include "piece.h"


//TODO: optimize: private members
class Grid : public QGraphicsItem
{
private:
    int light;
public:
    Piece *piece;
    LGrid *lgrid;
    Board *board;

    Grid(LGrid *lgrid, Board* = nullptr);
    ~Grid();

    void highlight(int = 1);
    operator LGrid () const;

    Grid *offset(int dx, int dy) const;
    static Grid *get(LGrid *, Board *board);

    // QWidget interface
    QRectF boundingRect() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};





class LGrid {
public:
    LGrid(int _x = 0, int _y = 0, LBoard *l_board = nullptr);
    LGrid *offset(int x, int y);
    bool is_attacked(bool w);
    QString name() const;
    bool empty() const;
    vector<LPiece *> attackedBy(bool w);
    int x, y;
    LPiece* lpiece;
    LBoard* lboard;
};

