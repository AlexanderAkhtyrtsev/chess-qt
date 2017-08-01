#include "helpwnd.h"

HelpWnd::HelpWnd(QWidget *parent) : Window(parent)
{
    vbl = new QVBoxLayout(this);
    lbl_text = new QLabel("Help will be here.", this);
    scrollArea = new QScrollArea(this);
    lbl_text->setWordWrap(true);
    lbl_text->setAlignment(Qt::AlignJustify);

    scrollArea->resize(size());
    scrollArea->setWidget(lbl_text);

    btn_close = new QPushButton("Close", this);

    vbl->addWidget(scrollArea);
    vbl->addWidget(btn_close);
    setLayout(vbl);

    QObject::connect(btn_close, SIGNAL(clicked()), this, SLOT(hide()));
    _hide();
}

HelpWnd::~HelpWnd()
{

}
