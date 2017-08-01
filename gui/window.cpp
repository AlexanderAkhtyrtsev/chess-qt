#include "window.h"

Window::Window(QWidget *parent, float opacity) : QWidget(parent)
{
    if (parent) {
        resize(parent->size());
    }
    m_opacity = opacity;
}

void Window::show()
{
    if (isHidden()) {
        anim = new QPropertyAnimation(this, "pos", this);
        anim->setStartValue(QPoint(-width(), 0));
        anim->setEndValue(QPoint(0,0));
        anim->setDuration(500);
        anim->start();
        _show();
    }
}

void Window::_show()
{
    QWidget::show();
}

void Window::hide()
{
    if (!isHidden()) {
        anim = new QPropertyAnimation(this, "pos", this);
        anim->setStartValue(QPoint(0,0));
        anim->setEndValue(QPoint(-width(), 0));
        anim->setDuration(500);
        anim->start();
        QObject::connect(anim, SIGNAL(finished()), this, SLOT(_hide()));
    }
}

void Window::_hide()
{
    QWidget::hide();
}


void Window::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
//    painter.save();

        painter.setBrush(QBrush(QColor(230,230,250)));
        painter.setPen(Qt::NoPen);
        painter.setOpacity(m_opacity);
        painter.drawRect(rect());

//    painter.restore();
    painter.end();
}


float Window::opacity() const
{
    return m_opacity;
}

void Window::setOpacity(float opacity)
{
    m_opacity = opacity;
    repaint();
}
