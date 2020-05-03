#pragma once
#include "board.h"

class GraphicsView : public QGraphicsView
{
private:
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *anim;
public:
    GraphicsView(QWidget *parent = nullptr);
    Board *board;
    void fadeIn();
    ~GraphicsView();
protected:
    void resizeEvent(QResizeEvent *);
};
