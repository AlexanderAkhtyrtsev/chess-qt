#include "dialogs.h"


NewGameDialog::NewGameDialog(Board *, QWidget *parent) : QWidget(parent)
{
    cb_player[0] = new QComboBox;
    cb_player[1] = new QComboBox;

    cb_player[0]->addItem("Human");
    cb_player[1]->addItem("Human");
    cb_player[0]->addItem("CPU");
    cb_player[1]->addItem("CPU");

    lbl[0] = new QLabel("Player 1");
    lbl[1] = new QLabel("Player 2");

    flipBoard = new QCheckBox("Flip board after move");

    vbl = new QVBoxLayout;

    hbl[0] = new QHBoxLayout;
    hbl[0]->addWidget(lbl[0]);
    hbl[0]->addWidget(cb_player[0]);

    hbl[1] = new QHBoxLayout;
    hbl[1]->addWidget(lbl[1]);
    hbl[1]->addWidget(cb_player[1]);


    btn_startGame = new QPushButton("PLAY");

    vbl->addLayout(hbl[0]);
    vbl->addLayout(hbl[1]);
    vbl->addWidget(flipBoard);
    vbl->addWidget(btn_startGame);
    setLayout(vbl);
    vbl->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    this->setStyleSheet("* {color:black; font-size: 18pt; }");

}

NewGameDialog::~NewGameDialog()
{}
