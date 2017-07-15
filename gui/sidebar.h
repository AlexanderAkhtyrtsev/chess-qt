#pragma once
#include "mainwindow.h"

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(Board *, QWidget *parent = 0);
    ~Sidebar();
    QVBoxLayout *layout;
    Board *board;
    QPropertyAnimation *anim;
    void Toggle();
    QLabel *lbl_logo;
    QPushButton *btn_continue, *btn_newGame, *btn_exit;
    QSpacerItem *spacer;
public slots:
    void Exit();
    void newGame();
    void Show();
    void Hide();
protected:
    void paintEvent(QPaintEvent *);
};

