#include "gametimer.h"

GameTimer::GameTimer(Board *brd, QGraphicsObject *parent) : QGraphicsObject(parent)
{
    board = brd;
}

QRectF GameTimer::boundingRect() const
{
    return QRectF( 0, 0, board->grid_size * 4, board->grid_size );
}

void GameTimer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();


    painter->restore();
}
