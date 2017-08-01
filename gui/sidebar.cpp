#include "sidebar.h"


Sidebar::Sidebar(Board *brd, QWidget *parent) : Window(parent)
{
    board = brd;

    layout = new QVBoxLayout;

    lbl_logo = new QLabel("Chess", this);
        lbl_logo->setAlignment(Qt::AlignHCenter);
        lbl_logo->setFont(QFont("Arial",32));
        lbl_logo->setMargin(5);

    btn_continue    = new QPushButton("Continue", this);
    btn_newGame     = new QPushButton("New Game", this);
    btn_options     = new QPushButton("Options", this);
    btn_help        = new QPushButton("Help", this);
    btn_about       = new QPushButton("About", this);
    btn_exit        = new QPushButton("Exit", this);
    spacer = new QSpacerItem(40,40, QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(lbl_logo);

    layout->addWidget(btn_continue);
    layout->addWidget(btn_newGame);
    layout->addWidget(btn_options);
    layout->addWidget(btn_help);

    layout->addSpacerItem(spacer);
    layout->addWidget(btn_about);
    layout->addWidget(btn_exit);


    setLayout(layout);
    optionsWnd = new OptionsWindow(board, parent);
    optionsWnd->resize(size());

    helpWnd = new HelpWnd(parent);
    helpWnd->resize(size());

    QObject::connect(btn_continue,  SIGNAL(clicked()), SLOT(hide()));
    QObject::connect(btn_newGame,   SIGNAL(clicked()), SLOT(newGame()));
    QObject::connect(btn_options,   SIGNAL(clicked()), SLOT(showOptions()));
    QObject::connect(btn_help,      SIGNAL(clicked()), helpWnd, SLOT(show()));
    QObject::connect(btn_exit,      SIGNAL(clicked()), SLOT(Exit()));
    setOpacity(1);
}

Sidebar::~Sidebar()
{
    delete layout;
}


void Sidebar::Toggle()
{
    if( isHidden() ) show();
    else hide();
}

void Sidebar::showOptions()
{
    optionsWnd->show();
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

