#ifndef FADER_H
#define FADER_H

#include <QtWidgets>

class Fader : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)
    QPropertyAnimation *anim;
    qreal m_opacity;

public:
    explicit Fader(qreal _opacityDepth = 0.8, QWidget *parent = 0);
    qreal opacityDepth;
    unsigned animDuration;

    qreal getOpacity() const;

signals:

public slots:

    // QWidget interface
    void setOpacity(qreal opacity);
    void animEnd();
    void show();
    void _show();
    void hide();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // FADER_H
