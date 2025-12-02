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
    db.setDatabaseName("gestion");//inserer le nom de la source de données
    db.setUserName("bilel");//inserer nom de l'utilisateur
    db.setPassword("bilel");//inserer mot de passe de cet utilisateur

    if (db.open()) {
        qDebug() << "✅ Connexion établie à la base:" << db.databaseName();
        return true;
    } else {
        qDebug() << "❌ Échec de la connexion:" << db.lastError().text();
        return false;
    }
}
