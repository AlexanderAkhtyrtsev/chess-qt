#include "sidebar.h"

Sidebar::Sidebar(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout;
    setLayout(layout);
    setGeometry(-100,0,200, parent->height());
    anim = new QPropertyAnimation(this, "pos");


}

Sidebar::~Sidebar()
{
    delete layout;
    delete anim;
}

void Sidebar::Show()
{
    move(-width(), 0);
    show();
    anim->setStartValue( QPoint(-width(), 0) );
    anim->setEndValue(QPoint(0,0));
    anim->setDuration(250);
    anim->start();
}

void Sidebar::Hide()
{
    anim->setStartValue(QPoint(0,0));
    anim->setEndValue(QPoint(-width(),0));
    anim->setDuration(250);
    anim->start();
}

void Sidebar::Toggle()
{
    if( pos().x() ) Show();
    else Hide();
}

void Sidebar::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.save();

        painter.setBrush(QBrush(QColor(230,230,250)));
        painter.setPen(Qt::NoPen);
        painter.setOpacity(0.9);
        painter.drawRect(0, 0, width(), height());

    painter.restore();
    painter.end();
}
