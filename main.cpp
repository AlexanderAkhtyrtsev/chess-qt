#include "gui/mainwindow.h"
#include <QApplication>

//TODO: split board & free figures

int main(int argc, char *argv[])
{
    qsrand(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
