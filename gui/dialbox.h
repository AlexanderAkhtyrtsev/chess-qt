#pragma once

#include <QtWidgets>
#include "fader.h"

class DialBox : public QWidget
{
    Q_OBJECT
    QPropertyAnimation *anim;
    Fader *fader;
public:
    explicit DialBox(QWidget *parent = 0);

public slots:
    void show();
    void _show();
    void hide();
    void animEnd();
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
};
