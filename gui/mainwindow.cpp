#include "mainwindow.h"
#include "../game/grid.h"

void MainWindow::test()
{
    /*
    time_t t1, t2;
    t1 = QDateTime::currentMSecsSinceEpoch();
    for(int i=0; i<20; i++) board->is_check(1);
    t2 = QDateTime::currentMSecsSinceEpoch();
 //*/
    qDebug() << "score:" << board->getCurrentScore() << "points";


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

    hbl->addWidget(list_sel1);
    hbl->addWidget(btn_newGame);
    hbl->addWidget(btn_automove);
    vbl->setMargin(0);
    view = new GraphicsView();
    board = new Board(view);
    board->window = this;
    view->board = board;
    view->setScene(board);
    vbl->addLayout(hbl);
    vbl->addWidget(view);
    this->setLayout(vbl);
    QObject::connect(btn_newGame, SIGNAL(clicked()), this->board, SLOT(newGame()));
    QObject::connect(btn_automove, SIGNAL(clicked()), this->board, SLOT(doAutoMove()));
}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
}

int MainWindow::sel1val()
{
    return list_sel1->currentData().toInt();
}


void MainWindow::resizeEvent(QResizeEvent *pe)
{
    //view->resize(this->board->grid_size*10,this->board->grid_size*10);
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

void MainWindow::mouseReleaseEvent(QMouseEvent)
{

}

Options::Options()
{
    flipBoard = false;
}
