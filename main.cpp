#include "mainwindow.h"
#include "compinstance.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CompInstance inst;

    return a.exec();
}
