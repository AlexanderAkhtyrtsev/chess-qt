#pragma once
#include <QtWidgets>
#include "game/board.h"
#include "graphicsview.h"
#include "game/gametimer.h"

class GraphicsView;
class GameTimer;
const QSize MIN_WINDOW_SIZE = QSize(800, 600);

class MainWindow : public QWidget
{
    Q_OBJECT
    QVBoxLayout *vbl;
    void test();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GraphicsView *view;
    Board *board;
    QMenuBar *menuBar;
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
};


class Options {
public:
    Options();
    bool flipBoard;
    enum  PlayerType {Human, AI_Stupid = 1, AI_Medium, AI_Hard, AI_Very_Hard};
    PlayerType player[2];
};
