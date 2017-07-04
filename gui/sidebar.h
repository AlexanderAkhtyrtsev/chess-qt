#pragma once
#include "mainwindow.h"

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent = 0);
    ~Sidebar();
    QVBoxLayout *layout;
    QPropertyAnimation *anim;
    void Show();
    void Hide();
    void Toggle();
protected:
    void paintEvent(QPaintEvent *);
};

