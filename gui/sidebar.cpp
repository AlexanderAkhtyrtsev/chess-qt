#include "sidebar.h"

Sidebar::Sidebar(Board *brd, QWidget *parent) : QWidget(parent)
{
    board = brd;

    layout = new QVBoxLayout;
    anim = new QPropertyAnimation(this, "pos");

    lbl_logo = new QLabel("Chess", this);
        lbl_logo->setAlignment(Qt::AlignHCenter);
        lbl_logo->setFont(QFont("Arial",32));
        lbl_logo->setMargin(5);

    btn_continue = new QPushButton("Continue", this);
    btn_newGame = new QPushButton("New Game", this);
    btn_exit = new QPushButton("Exit", this);
    spacer = new QSpacerItem(40,40, QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(lbl_logo);
    layout->addWidget(btn_continue);
    layout->addWidget(btn_newGame);
    layout->addSpacerItem(spacer);
    layout->addWidget(btn_exit);

    setLayout(layout);
    setGeometry(0, 0, 100, parent->height());

    QObject::connect(btn_continue, SIGNAL(clicked()), SLOT(Hide()));
    QObject::connect(btn_newGame, SIGNAL(clicked(bool)), SLOT(newGame()));
    QObject::connect(btn_exit, SIGNAL(clicked(bool)), SLOT(Exit()));
}

Sidebar::~Sidebar()
{
    delete layout;
    delete anim;
}

void Sidebar::Show()
{
    if ( pos().x() < 0 ) {
        anim->setStartValue(QPoint(-width(), 0));
        anim->setEndValue(QPoint(0,0));
        anim->setDuration(200);
        anim->start();
    }
}

void Sidebar::Hide()
{
    if (!pos().x()) {
        anim->setStartValue(QPoint(0,0));
        anim->setEndValue(QPoint(-width(), 0));
        anim->setDuration(200);
        anim->start();
    }
}

void Sidebar::Toggle()
{
    if( pos().x() ) Show();
    else Hide();
}

//----------------------------------------------------

void Sidebar::Exit()
{
    exit(0);
}

void Sidebar::newGame()
{
    Toggle();
    board->newGame();
}

void Sidebar::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.save();

        painter.setBrush(QBrush(QColor(230,230,250)));
        painter.setPen(Qt::NoPen);
        painter.setOpacity(0.95);
        painter.drawRect(rect());

    painter.restore();
    painter.end();
}
