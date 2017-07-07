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
    resize(320,480);
}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int w = width();
    int h = height();
    view->resize(w, h);
    int sw;
    if ( w <= 320 ) sw = w;
    else sw = w / 1.5;
    sidebar->resize(sw, h);
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
    pe->accept();
}
