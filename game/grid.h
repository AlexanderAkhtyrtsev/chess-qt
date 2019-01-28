#pragma once

#include "gui/mainwindow.h"
#include "board.h"
#include "piece.h"

class Grid : public QGraphicsItem
{

public:
    bool is_white;
    int light;
    int x, y;
    Grid(bool w = false, int _x = 0, int _y = 0, Board* = 0);
    ~Grid();
    void Highlight(int = 1);
    // QGraphicsItem interface
    QRectF boundingRect() const;
    Piece* piece;
    Board* board;
    Grid *Offset(int x, int y);
    QString name();
    bool is_attacked(bool);

    bool empty() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};
