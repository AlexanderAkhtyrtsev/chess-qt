#include "optionswindow.h"


OptionsWindow::OptionsWindow(Board *brd, QWidget *parent) : Window(parent)
{
    _hide();
    board = brd;
    btn_apply = new QPushButton("Apply");
    cb_flipBoard = new QCheckBox("Flip board after move (two players)");
    cb_flipBoard->setChecked(board->options->flipBoard);


    vbl = new QVBoxLayout;
    vbl->addWidget(cb_flipBoard);
    vbl->addStretch();
    vbl->addWidget(btn_apply);

    setLayout(vbl);
    QObject::connect(btn_apply, SIGNAL(clicked()), this, SLOT(applyChanges()));
}

void OptionsWindow::applyChanges()
{
    board->options->flipBoard = cb_flipBoard->isChecked();
    hide();
}
