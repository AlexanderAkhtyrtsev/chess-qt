#pragma once

#include "board.h"

class GameTimer : public QGraphicsObject
{
    Board *board;
    QTimer *timer;
public:
    GameTimer(Board *brd, QGraphicsObject *parent = 0);
public slots:
    void start();
    void stop();
    void reset();

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};
