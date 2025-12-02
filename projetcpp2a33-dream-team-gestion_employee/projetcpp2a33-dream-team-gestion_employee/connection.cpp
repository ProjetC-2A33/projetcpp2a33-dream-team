#include "connection.h"

Connection::Connection() {
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection::~Connection() {
    if (db.isOpen())
        db.close();
}

Connection& Connection::createInstance() {
    static Connection instance;
    return instance;
}

bool Connection::createConnection() {
    db.setDatabaseName("base"); // Nom de ton DSN ODBC
    db.setUserName("issam");
    db.setPassword("issamm");

    if (db.open()) {
        qDebug() << "✅ Connexion établie à la base:" << db.databaseName();
        return true;
    } else {
        qDebug() << "❌ Échec de la connexion:" << db.lastError().text();
        return false;
    }
}
