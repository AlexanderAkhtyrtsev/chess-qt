#pragma once
#include <QtWidgets>
#include "board.h"
#include "graphicsview.h"
#include "dialogs.h"

class MenuWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QVBoxLayout *vbl_menu, *vbl_game;
    GraphicsView *view;
    MenuWidget *menu;
    NewGameDialog *newGameDialog;
    void test();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void showMenu();
    void startGame();
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
};


class Options {
public:
    Options();
    bool flipBoard;
    enum  PlayerType {Human, AI_Stupid = 1, AI_Medium, AI_Hard, AI_VeryHard};
    PlayerType player[2];
};



class MenuWidget : public QWidget{
    Q_OBJECT
public:
    MenuWidget(MainWindow * = 0);
    ~MenuWidget();
private:
    QVBoxLayout *vbl_menu;
    QLabel *lbl_image;
    QPixmap *pix;
    QPushButton *btn_newGame, *btn_help, *btn_quit, *btn_stat, *btn_settings;
};

