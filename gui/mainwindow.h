#pragma once
#include <QtWidgets>
#include "game/board.h"
#include "graphicsview.h"
#include "sidebar.h"

class GraphicsView;
class Sidebar;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicsView *view;
    Board* board;
    Sidebar *sidebar;
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
};

