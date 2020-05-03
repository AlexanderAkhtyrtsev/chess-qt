#pragma once
#include "board.h"

class NewGameDialog : public QWidget
{
    Q_OBJECT
private:
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *anim;
    QComboBox *cb_player[2];
    QCheckBox *flipBoard;
    QVBoxLayout *vbl;
    QHBoxLayout *hbl[2];
    QLabel *lbl[2];
    QPushButton *btn_startGame;
public:
    NewGameDialog(Board* = 0,  QWidget *parent = nullptr);
    Board *board;
    void fadeIn();
    ~NewGameDialog();
};
