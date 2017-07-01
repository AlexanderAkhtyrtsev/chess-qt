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
    board->grid_size = min(width(), height()) / 8;
    board->setSceneRect(QRect(0, 0, board->grid_size * 8, board->grid_size * 8));
    board->ReplaceElements();
    QGraphicsView::resizeEvent(pe);
}
