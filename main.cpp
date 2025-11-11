#include "marque.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Initialiser la connexion à la base de données
    Connection c;
    bool test = c.createconnect();
    
    if (!test) {
        QMessageBox::critical(nullptr, "Erreur de connexion", 
                              "Impossible de se connecter à la base de données!");
        return -1;
    }
    
    marque w;
    w.show();
    return a.exec();
}
