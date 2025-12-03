#include "connection.h"

// Initialise l'attribut `db` avec le driver ODBC.
Connection::Connection() {
    db = QSqlDatabase::addDatabase("QODBC");
}

// Ferme proprement la connexion si encore ouverte
Connection::~Connection() {
    if (db.isOpen()) {
        db.close();
    }
}

// Accès à l'instance unique du Singleton
Connection& Connection::createInstance() {
    static Connection instance;
    return instance;
}

bool Connection::createConnection() {
    // Paramètres de connexion
    db.setDatabaseName("projet");//inserer le nom de la source de données ODBC
    db.setUserName("fatma");//inserer nom de l'utilisateur
    db.setPassword("fatma");//inserer mot de passe de cet utilisateur

    if (db.open()) {
        qDebug() << "Connexion établie";
        return true;
    }

    qDebug() << "Échec de la connexion :" << db.lastError().text();
    return false;
}

QSqlDatabase& Connection::getDatabase() {
    return db;
}


