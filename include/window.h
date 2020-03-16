#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>

class Window : public QWidget
{
    Q_OBJECT

    QPropertyAnimation *anim;
    float m_opacity;
public:
    explicit Window(QWidget *parent = 0, float opacity = 0.9);

    float opacity() const;
    void setOpacity(float opacity);

signals:

public slots:
    void show();
    void _show();
    void hide();
    void _hide();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WINDOW_H
