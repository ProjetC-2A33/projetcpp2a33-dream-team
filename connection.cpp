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
	db.setDatabaseName("Source_Projet2A");
	db.setUserName("layth");
	db.setPassword("layth");

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


