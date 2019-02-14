#pragma once
#include "game/board.h"

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = 0);
    Board *board;
protected:
    void resizeEvent(QResizeEvent *);
/*
    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *);
    */
};

