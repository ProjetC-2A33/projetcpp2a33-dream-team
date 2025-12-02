#include "connection.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection()
{

}

bool Connection::createconnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    QString connectionString =
        "Driver={Oracle in XE};"   // 👈 ton pilote exact
        "Dbq=XE;"                  // 👈 le nom du service Oracle (souvent 'XE' pour Oracle Express)
        "Uid=mohamed;"             // 👈 ton nom d’utilisateur Oracle
        "Pwd=mohamed;";            // 👈 ton mot de passe Oracle

    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qDebug() << "❌ Erreur :" << db.lastError().text();
        qDebug() << "Driver error:" << db.lastError().driverText();
        qDebug() << "Database error:" << db.lastError().databaseText();
        return false;
    }

    qDebug() << "✅ Connexion Oracle réussie !";
    return true;
}

