#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>

// Classe Connection
// Implémente un Singleton pour gérer une connexion unique à la base de données.
class Connection {
public:
    // Retourne la référence unique du Singleton
    static Connection& createInstance();

    // Configure et ouvre la base de données
    bool createConnection();

    // Retourne la référence à la base de données
    QSqlDatabase& getDatabase();

private:
    // Attribut de connexion persistant
    QSqlDatabase db;

    // Ctor/Dtor privés pour garantir l'unicité de l'instance
    Connection();
    ~Connection();

    // Interdire la copie et l'affectation
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
};

#endif // CONNECTION_H


