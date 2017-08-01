#pragma once
#include "mainwindow.h"
#include "window.h"
#include "optionswindow.h"
#include "helpwnd.h"

class OptionsWindow;
class HelpWnd;

class Sidebar : public Window
{

    Q_OBJECT
public:
    explicit Sidebar(Board *, QWidget *parent = 0);
    ~Sidebar();
    QVBoxLayout *layout;
    Board *board;
    void Toggle();
    QLabel *lbl_logo;

    QPushButton *btn_continue,
                *btn_newGame,
                *btn_options,
                *btn_help,
                *btn_about,
                *btn_exit;

    QSpacerItem *spacer;

    OptionsWindow *optionsWnd;
    HelpWnd *helpWnd;
public slots:
    void showOptions();
    void Exit();
    void newGame();
};

