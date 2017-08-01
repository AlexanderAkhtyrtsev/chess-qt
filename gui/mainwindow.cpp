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
    sidebar = new Sidebar(board, this);
    box = new Messagebox("Check Mate", this);
    sidebar->resize( MIN_WINDOW_SIZE );
}


MainWindow::~MainWindow()
{
    delete board;
    delete view;
}





void MainWindow::resizeEvent(QResizeEvent *pe)
{
    view->resize(size());
    QSize sidebarSize( qMin( MIN_WINDOW_SIZE.width(), width() ), height() );
    sidebar->resize(sidebarSize);
    sidebar->optionsWnd->resize(sidebarSize);
    sidebar->helpWnd->resize(sidebarSize);
    box->resize(size());
    pe->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{

#ifdef Q_OS_ANDROID
    static int backKeyPressed = 0;
#endif

    switch (pe->key())
    {
    case Qt::Key_Backspace:
    case Qt::Key_Back:
        board->undoMove();
#ifdef Q_OS_ANDROID
        if ( !board->moves->size() ){
            backKeyPressed ++;
            if (backKeyPressed > 2) exit(0);
        } else {
            backKeyPressed = 0;
        }
#endif
        break;
    case Qt::Key_Escape:
        sidebar->show();
        break;
    }
    pe->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *pe)
{
    if (pe->button() == Qt::RightButton) {
        sidebar->show();
    }
}

Options::Options()
{
    flipBoard = false;
}
