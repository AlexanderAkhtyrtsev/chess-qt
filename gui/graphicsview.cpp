#include "graphicsview.h"

inline int min(int a, int b)
{
    return a < b ? a : b;
}



GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    board = 0;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::resizeEvent(QResizeEvent *pe)
{
    int w = width(), h = height();
    board->grid_size = min(w/8, h/12);
    board->setSceneRect(QRect(0, 0, board->grid_size * 8, board->grid_size * 12));
    board->ReplaceElements();
    QGraphicsView::resizeEvent(pe);
}
