#include "mainwindow.h"
#include"logon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MainWindow w;
    LogOn *k=new LogOn;
    k->show();
    //w.show();
    return a.exec();
}
