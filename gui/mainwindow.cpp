#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    view = new GraphicsView(this);
    board = new Board(view);
    board->window = this;
    view->setScene(board);
    view->board = board;
    sidebar = new Sidebar(board, this);
    box = new Messagebox("Check Mate", this);
    sidebar->resize( MIN_WINDOW_SIZE );
    sidebar->move(-MIN_WINDOW_SIZE.width(), 0);
    resize( MIN_WINDOW_SIZE );

}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
}



void MainWindow::resizeEvent(QResizeEvent *)
{
    view->resize(size());
    sidebar->resize( qMin( MIN_WINDOW_SIZE.width(), width() ), height() );
    box->resize(size());
}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
    case Qt::Key_Backspace:
    case Qt::Key_Back:
        board->undoMove();
        break;
    case Qt::Key_Escape:
        sidebar->Toggle();
        break;
    }
    box->hide();
    pe->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *pe)
{
    box->hide();
    if (pe->button() == Qt::RightButton) {
        sidebar->Toggle();
    }
}
