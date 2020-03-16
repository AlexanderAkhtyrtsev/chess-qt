#pragma once

#include <QTimer>
#include <QLabel>

class Board;

class GameTimer : public QLabel
{
    Q_OBJECT
private:
    Board *board;
    QTimer *timer;
    quint32 passed;
    qint64 t_tick, t_start;
public:
    GameTimer(Board *brd, QWidget *parent = nullptr); // ????
    ~GameTimer();
    quint32 time() const;
    void setTime(quint32);
    QSize sizeHint() const;
public slots:
    GameTimer *start();
    GameTimer *stop();
    GameTimer *reset();
    void tick();
    void showTime();
protected:
    void resizeEvent(QResizeEvent *event);
};
