#include "mainwindow.h"
#include "../game/grid.h"

void MainWindow::test()
{

}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    resize( MIN_WINDOW_SIZE );
    vbl = new QVBoxLayout();
    vbl->setMargin(0);

    view = new GraphicsView;
    board = new Board(view);
    board->window = this;
    view->board = board;
    view->setScene(board);


    // Menu bar
    menuBar = new QMenuBar(this);
    QMenu *menu = new QMenu(tr("&Game"));

    menu->addAction("New Game",this->board, SLOT(newGame()));
    menu->addAction("Settings");
    menu->addAction("About Qt", qApp, &qApp->aboutQt);
    menuBar->addMenu(menu);


    vbl->addWidget(view);

     vbl->addWidget(menuBar);
    this->setLayout(vbl);


}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
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
    case Qt::Key_A:
        board->doAutoMove();
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
    player[1] = AI_Hard;
    player[0] = Human;
}
