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
    board->grid_size = qMin(w/BOARD_SIZE, h/BOARD_SIZE);
    board->setSceneRect(QRect(0, 0, board->grid_size * BOARD_SIZE, board->grid_size * BOARD_SIZE));
    board->ReplaceElements();
    QGraphicsView::resizeEvent(pe);
}
/*
void GraphicsView::mouseReleaseEvent(QMouseEvent *pe)
{
   //QWidget::mouseReleaseEvent(pe);
}
*/
