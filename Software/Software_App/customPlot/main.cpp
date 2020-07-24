#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
    MainWindow w;

    w.setWindowTitle("Heartbeat Plot V.1.0");
    w.setWindowIcon(QIcon(":/icono.png"));
    w.show();
    return a.exec();
}
