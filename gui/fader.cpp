#include "fader.h"

Fader::Fader(qreal _opacityDepth, QWidget *parent) : QWidget(parent)
{
    opacityDepth = _opacityDepth;
    animDuration = 500;
    resize(parent->size());
    QWidget::hide();
}

void Fader::show()
{
    QTimer::singleShot(animDuration, this, SLOT(_show()));
}

void Fader::_show()
{
    setOpacity(0);
    QWidget::show();
    anim = new QPropertyAnimation(this, "opacity");
    anim->setStartValue(0.0);
    anim->setEndValue(opacityDepth);
    anim->setDuration(animDuration);
    anim->start();
}

void Fader::hide()
{
    if ( m_opacity != opacityDepth ) return;
    anim = new QPropertyAnimation(this, "opacity");
    anim->setStartValue(opacityDepth);
    anim->setEndValue(0.0);
    anim->setDuration(animDuration);
    anim->start();
    connect(anim, SIGNAL(finished()), this, SLOT(animEnd()));
}




qreal Fader::getOpacity() const
{
    return m_opacity;
}

void Fader::setOpacity(qreal opacity)
{
    m_opacity = opacity;
    repaint();
}

void Fader::animEnd()
{
    QWidget::hide();
}

void Fader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);


    painter.setBrush(QBrush(QColor(0,0,0)));
    painter.setOpacity(m_opacity);
    painter.drawRect(rect());

    painter.end();
}
