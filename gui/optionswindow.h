#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "mainwindow.h"

class OptionsWindow : public Window
{
    Q_OBJECT
    QCheckBox *cb_flipBoard;
    QPushButton *btn_apply;
    QVBoxLayout *vbl;
    Board *board;
public:
    OptionsWindow(Board *brd, QWidget *parent = 0);
public slots:
    void applyChanges();

};
#endif // OPTIONSWINDOW_H
