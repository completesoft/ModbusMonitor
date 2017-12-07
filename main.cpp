#include "mainwindow.h"
#include <QApplication>

#include "dialogsettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    DialogSettings d;
//    d.show();

    MainWindow w;
    w.show();

    return a.exec();
}
