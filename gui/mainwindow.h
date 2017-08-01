#pragma once
#include <QtWidgets>
#include "game/board.h"
#include "graphicsview.h"
#include "sidebar.h"
#include "fader.h"
#include "messagebox.h"

class GraphicsView;
class Sidebar;
class Messagebox;

const QSize MIN_WINDOW_SIZE = QSize(384, 576);

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicsView *view;
    Board *board;
    Sidebar *sidebar;
    Messagebox *box;
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    void mouseReleaseEvent(QMouseEvent *);
};


class Options {
public:
    Options();
    bool flipBoard;

};
