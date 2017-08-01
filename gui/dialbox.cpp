#include "dialbox.h"

DialBox::DialBox(QWidget *parent) : QWidget(parent)
{
    QWidget::hide();
}

void DialBox::show()
{
    QTimer::singleShot(500, this, SLOT(_show()));
}

void DialBox::_show()
{
    int endValue = height();
    resize(width(), 0);
    QWidget::show();

    anim = new QPropertyAnimation(this, "size", this);
    anim->setDuration(500);
    anim->setStartValue(size());
    anim->setEndValue(QSize(width(), endValue));
    anim->start();
}

void DialBox::hide()
{
    anim = new QPropertyAnimation(this, "size", this);
    anim->setDuration(500);
    anim->setStartValue(QSize(width(), height()));
    anim->setEndValue(QSize(width(), 0));
    anim->start();
    QObject::connect(anim, SIGNAL(finished()), this, SLOT(animEnd()));
}

void DialBox::animEnd()
{
    resize(width(), anim->startValue().toSize().height());
    QWidget::hide();
}

void DialBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap));
    painter.drawRoundedRect(rect(), 6, 6);
}

void DialBox::resizeEvent(QResizeEvent *)
{
    QWidget *p = parentWidget();
    move( p->width()/2 - width() / 2, p->height() / 2 - height() / 2 );
}
