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
    delete opacityEffect;
}

void GraphicsView::resizeEvent(QResizeEvent *pe)
{
    int w = width(),
        h = height();

    auto edgeMultiplier = 0.95;
    board->grid_size = qMax(MIN_GRID_SIZE, qMin(int(w / board->size.width() * edgeMultiplier),
                            int(h / board->size.height())));

    board->setSceneRect(QRect(0, 0,
                              board->grid_size * board->size.width()-5,
                              board->grid_size * board->size.height()-5));


    board->replaceElements();
    QGraphicsView::resizeEvent(pe);
}
