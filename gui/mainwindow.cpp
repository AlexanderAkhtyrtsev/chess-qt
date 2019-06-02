#include "mainwindow.h"
#include "../game/grid.h"
#include "../game/gametimer.h"

void MainWindow::test()
{
    /*
    time_t t1, t2;
    t1 = QDateTime::currentMSecsSinceEpoch();
    for(int i=0; i<20; i++) board->is_check(1);
    t2 = QDateTime::currentMSecsSinceEpoch();
 //*/
    qDebug() << "score:" << board->lboard->getCurrentScore() << "points";
    qDebug() << "check_w:" << board->lboard->is_check(true);
    qDebug() << "check_b:" << board->lboard->is_check(false);


}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    resize( MIN_WINDOW_SIZE );
    list_sel1 = new QComboBox;

    list_sel1->addItem("Search depth: 0", 0);
    list_sel1->addItem("Search depth: 1", 1);
    list_sel1->addItem("Search depth: 2", 2);
    list_sel1->addItem("Search depth: 3", 3);
    list_sel1->addItem("Search depth: 4", 4);
    list_sel1->addItem("Search depth: 5", 5);
    list_sel1->setCurrentIndex(3);

    btn_newGame = new QPushButton("New game");
    btn_automove = new QPushButton("Do automove");
    vbl = new QVBoxLayout();
    hbl = new QHBoxLayout();
    hbl_timers = new QHBoxLayout();

    hbl->addWidget(list_sel1);
    hbl->addWidget(btn_newGame);
    hbl->addWidget(btn_automove);
    vbl->setMargin(0);


    view = new GraphicsView();
    board = new Board(view);
    board->window = this;
    view->board = board;
    view->setScene(board);

    timer[0] = new GameTimer(board, this);
    timer[1] = new GameTimer(board, this);
   /* hbl_timers->addWidget(timer[1]);
    hbl_timers->addWidget(timer[0]);
    hbl_timers->setAlignment(timer[0], Qt::AlignRight);
    */




    vbl->addLayout(hbl_timers);
    vbl->addWidget(view);
    vbl->addLayout(hbl);
    this->setLayout(vbl);
    QObject::connect(btn_newGame, SIGNAL(clicked()), this->board, SLOT(newGame()));
    QObject::connect(btn_automove, SIGNAL(clicked()), this->board, SLOT(doAutoMove()));

    timer[0]->raise();
    timer[1]->raise();
}


MainWindow::~MainWindow()
{
    delete timer[0];
    delete timer[1];
    delete board;
    delete view;
    delete hbl_timers;
}

int MainWindow::sel1val()
{
    return list_sel1->currentData().toInt();
}


void MainWindow::resizeEvent(QResizeEvent *pe)
{
    //view->resize(this->board->grid_size*10,this->board->grid_size*10);
    timer[0]->resize(this->width()/2, int(this->height() * 0.05));
    timer[1]->resize(this->width()/2, int(this->height() * 0.05));

    timer[1]->move(0, 0);
    timer[0]->move(this->width()-timer[0]->width(), 0);
    timer[0]->setAlignment(Qt::AlignRight);
    timer[1]->setAlignment(Qt::AlignLeft);
    pe->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{


    switch (pe->key())
    {
    case Qt::Key_Backspace:
    case Qt::Key_Back:
        board->undoMove();
        break;
    case Qt::Key_Space:
        board->reverse( !board->reverse() );
    break;
    case Qt::Key_T: test();
        break;
    case Qt::Key_N: board->newGame();
        break;
    }
    pe->accept();
}

Options::Options()
{
    flipBoard = false;
}
