#pragma once
#include <QtWidgets>
#include "game/board.h"
#include "graphicsview.h"

class GraphicsView;

const QSize MIN_WINDOW_SIZE = QSize(400, 600);

class MainWindow : public QWidget
{
    Q_OBJECT
    QVBoxLayout *vbl;
    QHBoxLayout *hbl;
    QPushButton *btn_newGame, *btn_automove;
    QComboBox *list_sel1;
    void test();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphicsView *view;
    Board *board;
    int sel1val();
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    void mouseReleaseEvent(QMouseEvent);
};


class Options {
public:
    Options();
    bool flipBoard;

};
