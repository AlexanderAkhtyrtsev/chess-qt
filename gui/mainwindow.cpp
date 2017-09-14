#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize( MIN_WINDOW_SIZE );
    view = new GraphicsView(this);
    board = new Board(view);
    board->window = this;
    view->setScene(board);
    view->board = board;
}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
}





void MainWindow::resizeEvent(QResizeEvent *pe)
{
    view->resize(size());
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

    }
    pe->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *pe)
{

}

Options::Options()
{
    flipBoard = false;
}
