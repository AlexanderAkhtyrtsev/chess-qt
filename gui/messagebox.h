#pragma once


#include "fader.h"
#include "dialbox.h"

#include <QSpacerItem>

class Messagebox : public QWidget
{
    Q_OBJECT
public:
    Messagebox(QString message = "Hello, World!", QWidget *parent = 0);
//    ~Messagebox();
private:
    QString m_message;
    Fader *fader;
    DialBox *box;
    QVBoxLayout *layout;
    QPushButton *btn_ok;
    QSpacerItem *spacer;
    QLabel *lbl;

public slots:
    void setText(QString);
    void show();
    void hide();
    void _hide();
protected:
    void resizeEvent(QResizeEvent *);
};
