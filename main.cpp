#include "gui/mainwindow.h"
#include <QApplication>
#include "game/gametimer.h"
//TODO: split board & free figures

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
