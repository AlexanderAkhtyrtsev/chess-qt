#include "fader.h"

Fader::Fader(QWidget *parent) : QWidget(parent)
{

}

void Fader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
        painter->setFont(QFont("Arial", 18, 1, true));
        painter->drawText(width()/2, height()/2, "Hello, World!");
}
