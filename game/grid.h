#pragma once

#include "gui/mainwindow.h"
#include "board.h"
#include "figure.h"

class Grid : public QGraphicsItem
{

public:
    bool is_white, light;
    int x, y;
    Grid(bool w = false, int _x = 0, int _y = 0, Board* = 0);
    ~Grid();
    void Highlight(bool = true);
    // QGraphicsItem interface
    QRectF boundingRect() const;
    Figure* figure;
    Board* board;
    Grid *Offset(int x, int y);

    bool is_attacked(bool);

    bool empty() const;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};
