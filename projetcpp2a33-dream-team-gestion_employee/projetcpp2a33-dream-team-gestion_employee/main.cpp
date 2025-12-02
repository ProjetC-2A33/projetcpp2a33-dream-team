#include "smartstudio.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize database connection FIRST
    Connection &conn = Connection::createInstance();
    if (!conn.createConnection()) {
        QMessageBox::critical(nullptr, "Database Error",
                              "Cannot connect to database!\nThe application will now exit.");
        return -1;
    }

    qDebug() << "Database connection established successfully";

    // Now create and show the login page
    smartstudio w;
    w.show();

    return a.exec();
}
