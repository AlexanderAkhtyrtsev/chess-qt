#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    view = new GraphicsView(parent);
    board = new Board(view);
    view->board = board;
    view->setScene(board);
    view->resize(320, 480);
    hbl = new QHBoxLayout;
    hbl->addWidget(view);
    setLayout(hbl);
    hbl->setMargin(0);
}


MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *)
{

}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{
    if ( pe->key() == Qt::Key_Backspace || pe->key() == Qt::Key_Back ) board->undoMove();
}
