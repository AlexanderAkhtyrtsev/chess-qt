#pragma once
#include <QtWidgets>
#include "board.h"
#include "graphicsview.h"
class GraphicsView;


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicsView *view;
    Board* board;
    QHBoxLayout *hbl;
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
};

