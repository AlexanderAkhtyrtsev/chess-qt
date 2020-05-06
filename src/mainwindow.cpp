#include <QApplication>
#include "mainwindow.h"


void MainWindow::test()
{}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(800, 600);
    this->setStyleSheet("background-color: white");
    menu = new MenuWidget(this);
    view = new GraphicsView;
    newGameDialog = new  NewGameDialog(this);
    showMenu();

    this->setMinimumSize(MIN_GRID_SIZE * view->board->size.width(),
                         MIN_GRID_SIZE * view->board->size.height());
}


MainWindow::~MainWindow()
{
    //delete menu;
    delete view;
}

void MainWindow::showMenu()
{
    this->setCentralWidget(menu);
}

void MainWindow::startGame()
{
    this->setCentralWidget(newGameDialog);
}


void MainWindow::resizeEvent(QResizeEvent *pe)
{
    pe->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *pe)
{
    auto board = view->board;
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
    player[1] = Human;
    player[0] = Human;
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


MenuWidget::MenuWidget(MainWindow *mainWindow)
{
    // TODO: OPTIMIZE THIS SHIT (!)
    vbl_menu = new QVBoxLayout;
    QGraphicsOpacityEffect *oe = new QGraphicsOpacityEffect(this);
    oe->setOpacity(0);
    QPropertyAnimation *anim = new QPropertyAnimation(oe, "opacity");
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setDuration(1000);

    // icon
    pix = new QPixmap(":/img/pieces.png");
    int dx = pix->width()/6,
        dy = pix->height()/2;

    qsrand(time(0));
    int r = qrand() % 5;

    QPixmap pixCopy = pix->copy(r * dx,0, dx, dy);
    auto pixHeight = rect().size().height()/5;
    pixCopy = pixCopy.scaled(pixHeight, pixHeight);

    lbl_image = new QLabel;
    lbl_image->setPixmap(pixCopy);
    lbl_image->setAlignment(Qt::AlignHCenter);

    vbl_menu->setAlignment(Qt::AlignVCenter);
    vbl_menu->addWidget(lbl_image);
    vbl_menu->addWidget(btn_newGame  = new QPushButton("Play Chess"));
    vbl_menu->addWidget(btn_stat     = new QPushButton("Statistics"));
    vbl_menu->addWidget(btn_settings = new QPushButton("Settings"));
    vbl_menu->addWidget(btn_help = new QPushButton("Help"));
    vbl_menu->addWidget(btn_quit     = new QPushButton("Quit"));

    QObject::connect(btn_quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(btn_newGame, SIGNAL(clicked()), mainWindow, SLOT(startGame()));



    btn_newGame->setObjectName("playgamebtn");
    this->setStyleSheet("QPushButton { padding: 8px; font-size: 18pt; border: none; color: #666;} "
                        "QPushButton#playgamebtn {font-size:24pt}"
                        "QPushButton:pressed, QPushButton:hover {color: black; }"
                        );

    setLayout(vbl_menu);
    this->setGraphicsEffect(oe);
    anim->start();
}

MenuWidget::~MenuWidget()
{
    delete lbl_image;
    delete pix;
    delete vbl_menu;
    delete btn_newGame;
    delete btn_stat;
    delete btn_settings;
    delete btn_quit;
    delete btn_help;
}
