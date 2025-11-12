#include "projet.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCheckBox>
#include <QHeaderView>
#include <QPdfWriter>
#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTableWidgetItem>
#include <QPageSize>
#include <QPageLayout>

// Constructeur par défaut
Projet::Projet() : m_budget(0.0) {
}

// Constructeur avec paramètres
Projet::Projet(const QString& id, const QString& email, const QDate& deadline, double budget, const QString& typeProjet)
	: m_id(id), m_email(email), m_deadline(deadline), m_budget(budget), m_typeProjet(typeProjet) {
}

// Validation d'un projet
bool Projet::isValid(const Projet& p) {
	if (p.m_id.isEmpty() || p.m_id.length() > 8) {
		qDebug() << "Erreur: ID_P invalide (vide ou > 8 caractères)";
		return false;
	}
	// Vérifier que l'ID ne contient que des caractères alphanumériques
	QRegularExpression idRegex("^[A-Za-z0-9]+$");
	if (!idRegex.match(p.m_id).hasMatch()) {
		qDebug() << "Erreur: ID_P contient des symboles invalides (seulement alphanumériques autorisés)";
		return false;
	}
	if (p.m_email.isEmpty() || p.m_email.length() > 20) {
		qDebug() << "Erreur: E_MAIL invalide (vide ou > 20 caractères)";
		return false;
	}
	// Validation email basique
	QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
	if (!emailRegex.match(p.m_email).hasMatch()) {
		qDebug() << "Erreur: Format email invalide";
		return false;
	}
	if (p.m_budget < 0) {
		qDebug() << "Erreur: Budget négatif";
		return false;
	}
	if (p.m_typeProjet.length() > 20) {
		qDebug() << "Erreur: TYPE_PROJET > 20 caractères";
		return false;
	}
	return true;
}

// Ajouter un projet
bool Projet::add(const Projet& p) {
	if (!isValid(p)) {
		return false;
	}

	QSqlDatabase& db = Connection::createInstance().getDatabase();
	if (!db.isOpen()) {
		qDebug() << "Erreur: Base de données non connectée";
		return false;
	}

	QSqlQuery query(db);
	query.prepare("INSERT INTO LAYTH.PROJETS (ID_P, E_MAIL, DEADLINE, BUDGET, TYPE_PROJET) "
		"VALUES (:id, :email, :deadline, :budget, :typeProjet)");
	
	query.bindValue(":id", p.m_id);
	query.bindValue(":email", p.m_email);
	query.bindValue(":deadline", p.m_deadline);
	query.bindValue(":budget", p.m_budget);
	query.bindValue(":typeProjet", p.m_typeProjet);

	if (!query.exec()) {
		qDebug() << "Erreur lors de l'ajout:" << query.lastError().text();
		return false;
	}

	qDebug() << "Projet ajouté avec succès: ID_P =" << p.m_id;
	return true;
}

// Modifier un projet
bool Projet::update(const Projet& p) {
	if (!isValid(p)) {
		return false;
	}

	QSqlDatabase& db = Connection::createInstance().getDatabase();
	if (!db.isOpen()) {
		qDebug() << "Erreur: Base de données non connectée";
		return false;
	}

	QSqlQuery query(db);
	query.prepare("UPDATE LAYTH.PROJETS SET "
		"E_MAIL = :email, "
		"DEADLINE = :deadline, "
		"BUDGET = :budget, "
		"TYPE_PROJET = :typeProjet "
		"WHERE ID_P = :id");

	query.bindValue(":id", p.m_id);
	query.bindValue(":email", p.m_email);
	query.bindValue(":deadline", p.m_deadline);
	query.bindValue(":budget", p.m_budget);
	query.bindValue(":typeProjet", p.m_typeProjet);

	if (!query.exec()) {
		qDebug() << "Erreur lors de la modification:" << query.lastError().text();
		return false;
	}

	if (query.numRowsAffected() == 0) {
		qDebug() << "Aucun projet trouvé avec ID_P =" << p.m_id;
		return false;
	}

	qDebug() << "Projet modifié avec succès: ID_P =" << p.m_id;
	return true;
}

// Supprimer un projet par ID
bool Projet::removeById(const QString& id) {
	if (id.isEmpty()) {
		qDebug() << "Erreur: ID_P vide";
		return false;
	}

	QSqlDatabase& db = Connection::createInstance().getDatabase();
	if (!db.isOpen()) {
		qDebug() << "Erreur: Base de données non connectée";
		return false;
	}

	QSqlQuery query(db);
	query.prepare("DELETE FROM LAYTH.PROJETS WHERE ID_P = :id");
	query.bindValue(":id", id);

	if (!query.exec()) {
		qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
		return false;
	}

	if (query.numRowsAffected() == 0) {
		qDebug() << "Aucun projet trouvé avec ID_P =" << id;
		return false;
	}

	qDebug() << "Projet supprimé avec succès: ID_P =" << id;
	return true;
}

// Récupérer tous les projets (optionnel: filtre par ID ou email)
QVector<Projet> Projet::fetchAll(const QString& filter) {
	QVector<Projet> projets;
	
	QSqlDatabase& db = Connection::createInstance().getDatabase();
	if (!db.isOpen()) {
		qDebug() << "Erreur: Base de données non connectée";
		return projets;
	}

	QSqlQuery query(db);
	QString sql = "SELECT ID_P, E_MAIL, DEADLINE, BUDGET, TYPE_PROJET FROM LAYTH.PROJETS";
	
	if (!filter.isEmpty()) {
		sql += " WHERE ID_P LIKE :filter OR E_MAIL LIKE :filter";
		query.prepare(sql);
		query.bindValue(":filter", "%" + filter + "%");
	} else {
		query.prepare(sql);
	}

	if (!query.exec()) {
		qDebug() << "Erreur lors de la récupération:" << query.lastError().text();
		return projets;
	}

	while (query.next()) {
		Projet p;
		p.m_id = query.value(0).toString();
		p.m_email = query.value(1).toString();
		p.m_deadline = query.value(2).toDate();
		p.m_budget = query.value(3).toDouble();
		p.m_typeProjet = query.value(4).toString();
		projets.append(p);
	}

	qDebug() << "Projets récupérés:" << projets.size();
	return projets;
}

// Remplir le tableau avec les données
void Projet::fillTable(QTableWidget* table, const QVector<Projet>& rows) {
	if (!table) {
		qDebug() << "Erreur: TableWidget est null";
		return;
	}

	// Configuration des colonnes
	table->setColumnCount(5);
	table->setHorizontalHeaderLabels(QStringList() << "ID_P" << "E_MAIL" << "DEADLINE" << "BUDGET" << "TYPE_PROJET");
	
	// Supprimer les lignes existantes
	table->setRowCount(0);

	// Ajouter les données
	for (int i = 0; i < rows.size(); ++i) {
		const Projet& p = rows[i];
		table->insertRow(i);
		
		table->setItem(i, 0, new QTableWidgetItem(p.m_id));
		table->setItem(i, 1, new QTableWidgetItem(p.m_email));
		table->setItem(i, 2, new QTableWidgetItem(p.m_deadline.toString("yyyy-MM-dd")));
		table->setItem(i, 3, new QTableWidgetItem(QString::number(p.m_budget, 'f', 2)));
		table->setItem(i, 4, new QTableWidgetItem(p.m_typeProjet));
	}

	// Ajuster la largeur des colonnes
	table->resizeColumnsToContents();
	table->horizontalHeader()->setStretchLastSection(true);
}

// Helper: Extraire le type de projet depuis le GroupBox
QString Projet::typeProjetFromGroupBox(const QGroupBox* box) {
	if (!box) {
		return QString();
	}

	QStringList types;
	QList<QCheckBox*> checkboxes = box->findChildren<QCheckBox*>();
	
	for (QCheckBox* checkbox : checkboxes) {
		if (checkbox->isChecked()) {
			QString text = checkbox->text().trimmed();
			if (!text.isEmpty()) {
				types.append(text);
			}
		}
	}

	QString result = types.join(", ");
	// Limiter à 20 caractères pour correspondre à VARCHAR2(20)
	if (result.length() > 20) {
		result = result.left(20);
	}
	
	return result;
}

// Exporter le tableau en PDF
bool Projet::exportTableToPdf(QTableWidget* table, const QString& filePath) {
	if (!table) {
		qDebug() << "Erreur: TableWidget est null";
		return false;
	}

	if (filePath.isEmpty()) {
		qDebug() << "Erreur: Chemin de fichier vide";
		return false;
	}

	QPdfWriter pdfWriter(filePath);
	pdfWriter.setPageSize(QPageSize::A4);
	pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

	QPainter painter(&pdfWriter);
	painter.setPen(Qt::black);

	// Configuration de la police
	QFont headerFont("Arial", 12, QFont::Bold);
	QFont cellFont("Arial", 10);
	
	int rowHeight = 30;
	int headerHeight = 40;
	int currentY = 50;
	int margin = 50;
	int pageWidth = pdfWriter.width() - 2 * margin;
	int colCount = table->columnCount();
	int colWidth = pageWidth / colCount;

	// En-tête du document
	painter.setFont(QFont("Arial", 16, QFont::Bold));
	painter.drawText(margin, currentY, "Liste des Projets");
	currentY += 40;

	// En-tête du tableau
	painter.setFont(headerFont);
	painter.fillRect(margin, currentY, pageWidth, headerHeight, QColor(200, 200, 200));
	painter.drawRect(margin, currentY, pageWidth, headerHeight);
	
	for (int col = 0; col < colCount; ++col) {
		QString headerText = table->horizontalHeaderItem(col) ? 
			table->horizontalHeaderItem(col)->text() : QString("Col %1").arg(col + 1);
		painter.drawText(margin + col * colWidth + 5, currentY + 25, headerText);
		if (col < colCount - 1) {
			painter.drawLine(margin + (col + 1) * colWidth, currentY, margin + (col + 1) * colWidth, currentY + headerHeight);
		}
	}
	currentY += headerHeight;

	// Données du tableau
	painter.setFont(cellFont);
	int rowCount = table->rowCount();
	
	for (int row = 0; row < rowCount; ++row) {
		// Vérifier si on dépasse la page
		if (currentY + rowHeight > pdfWriter.height() - 50) {
			pdfWriter.newPage();
			currentY = 50;
		}

		painter.drawRect(margin, currentY, pageWidth, rowHeight);
		
		for (int col = 0; col < colCount; ++col) {
			QTableWidgetItem* item = table->item(row, col);
			QString text = item ? item->text() : QString();
			
			// Tronquer le texte si trop long
			if (text.length() > 30) {
				text = text.left(27) + "...";
			}
			
			painter.drawText(margin + col * colWidth + 5, currentY + 20, text);
			
			if (col < colCount - 1) {
				painter.drawLine(margin + (col + 1) * colWidth, currentY, 
					margin + (col + 1) * colWidth, currentY + rowHeight);
			}
		}
		
		currentY += rowHeight;
	}

	painter.end();
	qDebug() << "PDF exporté avec succès:" << filePath;
	return true;
}
