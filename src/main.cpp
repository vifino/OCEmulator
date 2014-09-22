#include "mainwindow.h"
#include "compinstance.h"
#include <QApplication>

#include <iostream>
#include <boost/filesystem.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::string home;

#ifdef __unix__
    home = getenv("HOME");
#endif
#ifdef __WINDOWS__
    home = getenv("APPDATA");
#endif

    std::string OCEDir = home + "/.ocemulator";
    std::string InsDir = OCEDir + "/instances";
    boost::filesystem::create_directories(home + "/.ocemulator/instances");
    CompInstance inst(InsDir + "/test");

    return a.exec();
}
