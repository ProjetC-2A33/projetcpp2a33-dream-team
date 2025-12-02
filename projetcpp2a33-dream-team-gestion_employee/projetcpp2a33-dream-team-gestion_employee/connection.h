#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Connection {
public:
    Connection();
    ~Connection();

    static Connection& createInstance();
    bool createConnection();

private:
    QSqlDatabase db;
};

#endif // CONNECTION_H
