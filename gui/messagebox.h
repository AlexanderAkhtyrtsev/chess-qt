#pragma once

#include <QtWidgets>

class Messagebox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)
public:
    Messagebox(QString message = "Hello, World!", QWidget *parent = 0);
//    ~Messagebox();
private:
    qreal opacity;
    QString m_message;
    QPropertyAnimation *anim;
    qreal getOpacity() const;
signals:
    void clicked();
    // QWidget interface
public slots:
    void setOpacity(qreal);
    void setText(QString);
    void show();
    void hide();
    void animEnd();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};
