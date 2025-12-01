#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // ⚠️ Chaîne exactement comme dans ton projet CMake, mais avec ton user actuel
    QString connStr =
        "DRIVER={Oracle in XE};"
        "DBQ=XE;"
        "UID=GST_SPONSOR;"
        "PWD=1111;";

    db.setDatabaseName(connStr);

    qDebug() << "Chaîne de connexion utilisée :" << connStr;

    if (db.open()) {
        test = true;
        qDebug() << "Connexion Oracle (QODBC) réussie !";
    } else {
        qDebug() << "Erreur de connexion Oracle :" << db.lastError().text();
        QMessageBox::critical(nullptr,
                              "Erreur BD",
                              "Connexion Oracle échouée :\n" + db.lastError().text());
    }

    return test;
}
