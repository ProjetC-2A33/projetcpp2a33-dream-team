#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	// Récupère l'instance unique du singleton et tente la connexion
	Connection& c = Connection::createInstance();
	if (!c.createConnection()) {
		QMessageBox::critical(nullptr, QObject::tr("Base de données"), QObject::tr("Échec de la connexion."));
		return 1;
	}

	MainWindow w;
	w.show();
	return a.exec();
}


