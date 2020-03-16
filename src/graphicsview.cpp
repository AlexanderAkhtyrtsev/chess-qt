#include "graphicsview.h"


GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    board = nullptr;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::resizeEvent(QResizeEvent *pe)
{
    int w = width(), h = height(); 
    board->grid_size = qMin(int(w/board->size.width()*0.95), h/board->size.height());
    board->setSceneRect(QRect(0, 0, board->grid_size * board->size.width(), board->grid_size * board->size.height()));
    board->replaceElements();
    QGraphicsView::resizeEvent(pe);
}
/*
void GraphicsView::mouseReleaseEvent(QMouseEvent *pe)
{
   //QWidget::mouseReleaseEvent(pe);
}
*/
