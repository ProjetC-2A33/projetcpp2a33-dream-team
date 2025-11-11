<<<<<<< HEAD
#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projet");//inserer le nom de la source de données
db.setUserName("nour");//inserer nom de l'utilisateur
db.setPassword("nourAMD44");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}
=======
#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
connection::connection()
{

}
bool connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projet");//inserer le nom de la source de données ODBC
db.setUserName("fatma");//inserer nom de l'utilisateur
db.setPassword("fatma");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}
>>>>>>> createur
