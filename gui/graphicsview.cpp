#include "graphicsview.h"


GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    board = 0;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::resizeEvent(QResizeEvent *pe)
{
    int w = width(), h = height();
    board->grid_size = qMin(w/10, h/10);
    board->setSceneRect(QRect(0, 0, board->grid_size * 10, board->grid_size * 10));
    board->ReplaceElements();
    QGraphicsView::resizeEvent(pe);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *pe)
{
    QWidget::mouseReleaseEvent(pe);
}
