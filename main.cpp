#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(gestionCreateur);
    MainWindow w;
    connection c;
    c.createconnect();
    w.show();
    return a.exec();
}
