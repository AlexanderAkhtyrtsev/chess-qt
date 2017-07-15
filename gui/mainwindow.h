#pragma once
#include <QtWidgets>
#include "game/board.h"
#include "graphicsview.h"
#include "sidebar.h"
#include "gui/messagebox.h"

class GraphicsView;
class Sidebar;

const QSize MIN_WINDOW_SIZE = QSize(320, 480);

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicsView *view;
    Board* board;
    Sidebar *sidebar;
    Messagebox *box;
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *);
};

