#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
public:
    Connection();
    bool createconnect();   // Ouvre la connexion Oracle via ODBC
};

#endif // CONNECTION_H
