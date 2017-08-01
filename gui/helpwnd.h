#ifndef HELPWND_H
#define HELPWND_H

#include "window.h"

class HelpWnd : public Window
{
    QVBoxLayout *vbl;
    QLabel *lbl_text;
    QScrollArea *scrollArea;
     QPushButton *btn_close;
public:
    HelpWnd(QWidget *parent = 0);
    ~HelpWnd();

};

#endif // HELPWND_H
