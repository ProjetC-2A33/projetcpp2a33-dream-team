#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Connexion Oracle
    Connection c;
    if (!c.createconnect()) {
        // Le message d'erreur détaillé est déjà affiché dans createconnect()
        return -1;
    }

    qDebug() << "Connexion Oracle OK, lancement de la fenêtre principale...";

    // 2) Affichage direct de MainWindow (PAS de LoginDialog pour l'instant)
    MainWindow w;
    w.show();

    return a.exec();
}
