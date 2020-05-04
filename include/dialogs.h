#pragma once
#include "board.h"

class ListSelect;
class MainWindow;
class NewGameDialog : public QWidget
{
    Q_OBJECT
private:
    ListSelect *cpuLevel, *playerSide;
    QPushButton *btn_play;
public:
    NewGameDialog(MainWindow* = 0,  QWidget *parent = nullptr);
    MainWindow *mainWindow;
//    void fadeIn();
    ~NewGameDialog();
public slots:
    void playBtnPressed();
};


class ListSelect : public QWidget
{
    Q_OBJECT
private:
    QStringList list;
    QList<QPushButton *> *list_selections;
    QVBoxLayout *vbl;
    QHBoxLayout *hbl;
    QPushButton *optionSelected;
    QLabel *lbl_header;
public:
    ListSelect(QString header, QStringList list_options, QWidget *parent = nullptr);
    ~ListSelect();
    void select(QPushButton*);
public slots:
    void select();
    unsigned selected();
};
