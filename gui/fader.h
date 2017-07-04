#pragma once

#include <QObject>
#include <QWidget>

class Fader : public QWidget
{
    Q_OBJECT
public:
    explicit Fader(QWidget *parent = 0);
    void fade(bool);
signals:

public slots:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

