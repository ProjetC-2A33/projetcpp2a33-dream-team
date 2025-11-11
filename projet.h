#ifndef PROJET_H
#define PROJET_H

#include <QString>
#include <QDate>
#include <QVector>
#include <QGroupBox>
#include <QTableWidget>

class Projet {
public:
	Projet();
	Projet(const QString& id, const QString& email, const QDate& deadline, double budget, const QString& typeProjet);

	// Getters
	const QString& getId() const { return m_id; }
	const QString& getEmail() const { return m_email; }
	const QDate& getDeadline() const { return m_deadline; }
	double getBudget() const { return m_budget; }
	const QString& getTypeProjet() const { return m_typeProjet; }

	// Setters
	void setId(const QString& value) { m_id = value; }
	void setEmail(const QString& value) { m_email = value; }
	void setDeadline(const QDate& value) { m_deadline = value; }
	void setBudget(double value) { m_budget = value; }
	void setTypeProjet(const QString& value) { m_typeProjet = value; }

	// CRUD Operations - Static methods
	// Usage dans le contrôleur UI:
	// 1. Ajouter: Créer Projet depuis les widgets (lineEdit_43, l2_13, dateTimeEdit_13, l2_17, groupBox)
	//    puis appeler Projet::add(p). Après succès, appeler refreshTable().
	// 2. Modifier: Récupérer ID_P depuis tableWidget_2->currentItem(), créer Projet avec nouvelles valeurs,
	//    puis appeler Projet::update(p). Après succès, appeler refreshTable().
	// 3. Supprimer: Récupérer ID_P depuis tableWidget_2->currentItem(), puis appeler Projet::removeById(id).
	//    Après succès, appeler refreshTable().
	// 4. Refresh: Projet::fillTable(ui->tableWidget_2, Projet::fetchAll(filterText))
	// 5. Export PDF: QFileDialog::getSaveFileName() puis Projet::exportTableToPdf(ui->tableWidget_2, filePath)

	// Ajouter un projet
	static bool add(const Projet& p);

	// Modifier un projet
	static bool update(const Projet& p);

	// Supprimer un projet par ID
	static bool removeById(const QString& id);

	// Récupérer tous les projets (optionnel: filtre par ID ou email)
	static QVector<Projet> fetchAll(const QString& filter = QString());

	// Remplir le tableau avec les données
	static void fillTable(QTableWidget* table, const QVector<Projet>& rows);

	// Exporter le tableau en PDF
	static bool exportTableToPdf(QTableWidget* table, const QString& filePath);

	static QString typeProjetFromGroupBox(const QGroupBox* box);

private:
	QString m_id;          // ID_P VARCHAR2(20)
	QString m_email;       // E_MAIL VARCHAR2(20)
	QDate m_deadline;      // DEADLINE DATE
	double m_budget;       // BUDGET NUMBER
	QString m_typeProjet;  // TYPE_PROJET VARCHAR2(20)

	// Validation helper
	static bool isValid(const Projet& p);
};

#endif // PROJET_H
