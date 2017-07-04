#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    view = new GraphicsView(this);
    board = new Board(view);

    view->board = board;
    view->setScene(board);
    sidebar = new Sidebar(this);
    sidebar->hide();
    resize(500,500);
}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    view->resize(width(),height());
    sidebar->resize(sidebar->width(), height());
}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{
    if ( pe->key() == Qt::Key_Backspace || pe->key() == Qt::Key_Back ) board->undoMove();
    else if (pe->key() == Qt::Key_Escape ) sidebar->Toggle();
}
