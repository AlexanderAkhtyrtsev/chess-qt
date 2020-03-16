#pragma once
#include "board.h"

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    Board *board;
protected:
    void resizeEvent(QResizeEvent *);
};
