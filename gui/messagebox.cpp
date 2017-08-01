#include "messagebox.h"


Messagebox::Messagebox(QString message, QWidget *parent) : QWidget(parent)
{
    m_message = message;


    fader = new Fader(0.8, this);
    box = new DialBox(this);

    lbl = new QLabel(message);
    lbl->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    QFont lbl_font(lbl->font().family(), 25);
    lbl->setFont(lbl_font);

    btn_ok = new QPushButton("OK");

    layout = new QVBoxLayout;
    layout->addWidget(lbl);
    layout->addWidget(btn_ok);
    box->setLayout(layout);
    resize(parent->size());
    _hide();
    QObject::connect(btn_ok, SIGNAL(clicked()), this, SLOT(hide()));
}




void Messagebox::resizeEvent(QResizeEvent *)
{
    fader->resize(parentWidget()->size());
    QSize boxSize(width() * 0.8, height() * 0.3);
    box->resize( boxSize );

    box->move( fader->width()/2 - boxSize.width()/2, fader->height() /2 - boxSize.height() / 2 );
}

void Messagebox::setText(QString text)
{
    lbl->setText(text);
}

void Messagebox::show()
{
    QWidget::show();
    fader->show();
    box->show();
}

void Messagebox::hide()
{
    QTimer::singleShot(500, this, SLOT(_hide()));
    fader->hide();
    box->hide();
}

void Messagebox::_hide()
{
    QWidget::hide();
}
