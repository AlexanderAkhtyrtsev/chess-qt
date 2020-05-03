#include "graphicsview.h"


GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    board = new Board(this);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setScene(board);

    opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(1);
    setGraphicsEffect(opacityEffect);
}

void GraphicsView::fadeIn()
{
    anim = new QPropertyAnimation(opacityEffect, "opacity");
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setDuration(200);
    anim->start();
}

GraphicsView::~GraphicsView()
{
    delete board;
}

void GraphicsView::resizeEvent(QResizeEvent *pe)
{
    int w = width(), h = height();
    board->grid_size = qMin(int(w/board->size.width()*0.95),
                              h/board->size.height());

    board->setSceneRect(QRect(0, 0,
                              board->grid_size * board->size.width(),
                              board->grid_size * board->size.height()));

    board->replaceElements();
    QGraphicsView::resizeEvent(pe);
}
