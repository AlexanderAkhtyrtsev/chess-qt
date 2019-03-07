#include "gui/mainwindow.h"
#include <QApplication>

//TODO: split board & free figures

int main(int argc, char *argv[])
{
    qsrand(static_cast<quint32>(time(nullptr)));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
