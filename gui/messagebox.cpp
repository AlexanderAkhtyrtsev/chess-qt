#include "messagebox.h"


Messagebox::Messagebox(QString message, QWidget *parent) : QWidget(parent)
{
    QObject::connect(this, SIGNAL(clicked()), SLOT(hide()));
    resize(parent->size());
    m_message = message;
    setOpacity(0);
    QWidget::hide();
}

qreal Messagebox::getOpacity() const
{
    return opacity;
}

void Messagebox::setOpacity(qreal _opacity)
{
    opacity = _opacity;
    repaint();
}

void Messagebox::show()
{
    setOpacity(0);
    QWidget::show();
    anim = new QPropertyAnimation(this, "opacity");
    anim->setStartValue(0.0);
    anim->setEndValue(0.9);
    anim->setDuration(500);
    anim->start();
}

void Messagebox::hide()
{
    anim = new QPropertyAnimation(this, "opacity");
    anim->setStartValue(0.9);
    anim->setEndValue(0.0);
    anim->setDuration(500);
    anim->start();
    connect(anim, SIGNAL(finished()), this, SLOT(animEnd()));
}

void Messagebox::animEnd()
{
    QWidget::hide();
}

void Messagebox::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    QFont fnt(painter.font());
    fnt.setPointSize(32);

    QFontMetrics fm(fnt);

    painter.setBrush(QBrush(QColor(50,0,0)));
    painter.setOpacity(opacity);
    painter.drawRect(rect());
    painter.setPen(Qt::white);

    int fw = fm.width(m_message);
    if (fw > width() * 0.8) {
        fnt.setPointSize((32 * width() * 0.8) / fw);
    }
    painter.setFont(fnt);
    fm = QFontMetrics(fnt);

    painter.drawText(QPoint(width() / 2 - fm.width(m_message) / 2, height() / 2 ), m_message);

    painter.end();
}

void Messagebox::mousePressEvent(QMouseEvent *pe)
{
    if (pe->button() == Qt::LeftButton ) emit clicked();
}

void Messagebox::setText(QString text)
{
    m_message = text;
    repaint();
}
