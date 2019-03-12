#pragma once
#include "gui/mainwindow.h"
class Board;

class GameTimer : public QWidget
{
    Q_OBJECT
private:
    Board *board;
    QTimer *timer;
    quint32 passed;
    qint64 t_tick, t_start;
public:
    GameTimer(Board *brd, QWidget *parent = nullptr);
    ~GameTimer();
    quint32 time() const;
    void setTime(quint32);
public slots:
    GameTimer *start();
    GameTimer *stop();
    GameTimer *reset();
    void tick();
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

    // QWidget interface
public:
    QSize sizeHint() const;
};
