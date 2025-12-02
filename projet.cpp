#include "projet.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCheckBox>
#include <QHeaderView>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QPrinter>
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
    if (p.m_id.isEmpty() || p.m_id.length() > 20) {
        qDebug() << "Erreur: ID_P invalide (vide ou > 20 caractères)";
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
    if (p.m_typeProjet.length() > 50) {
        qDebug() << "Erreur: TYPE_PROJET > 50 caractères";
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
        s_lastError = "Validation invalide";
        return false;
    }

    QSqlDatabase& db = Connection::createInstance().getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Erreur: Base de données non connectée";
        s_lastError = "DB fermée";
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
        s_lastError = query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "Aucun projet trouvé avec ID_P =" << p.m_id;
        s_lastError = QString("Aucun projet trouvé avec ID_P = %1").arg(p.m_id);
        return false;
    }

    qDebug() << "Projet modifié avec succès: ID_P =" << p.m_id;
    s_lastError.clear();
    return true;
}

bool Projet::updateById(const QString& oldId, const Projet& p) {
    if (!isValid(p)) { s_lastError = "Validation invalide"; return false; }
    QSqlDatabase& db = Connection::createInstance().getDatabase();
    if (!db.isOpen()) { s_lastError = "DB fermée"; return false; }
    QSqlQuery query(db);
    query.prepare("UPDATE LAYTH.PROJETS SET "
                  "ID_P = :newId, "
                  "E_MAIL = :email, "
                  "DEADLINE = :deadline, "
                  "BUDGET = :budget, "
                  "TYPE_PROJET = :typeProjet "
                  "WHERE ID_P = :oldId");
    query.bindValue(":newId", p.m_id);
    query.bindValue(":oldId", oldId);
    query.bindValue(":email", p.m_email);
    query.bindValue(":deadline", p.m_deadline);
    query.bindValue(":budget", p.m_budget);
    query.bindValue(":typeProjet", p.m_typeProjet);
    if (!query.exec()) { s_lastError = query.lastError().text(); return false; }
    if (query.numRowsAffected() == 0) { s_lastError = QString("Aucun projet trouvé avec ID_P = %1").arg(oldId); return false; }
    s_lastError.clear();
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
        QString where = " WHERE ID_P LIKE :f OR E_MAIL LIKE :f OR TYPE_PROJET LIKE :f OR TO_CHAR(BUDGET) LIKE :f OR TO_CHAR(DEADLINE, 'YYYY-MM-DD') LIKE :f";
        bool okNum = false; filter.toDouble(&okNum);
        QDate date = QDate::fromString(filter, "yyyy-MM-dd");
        if (okNum) {
            where += " OR BUDGET = :bExact";
        }
        if (date.isValid()) {
            where += " OR DEADLINE = :dExact";
        }
        sql += where;
    }
    query.prepare(sql);
    if (!filter.isEmpty()) {
        query.bindValue(":f", "%" + filter + "%");
        bool okNum = false; double num = filter.toDouble(&okNum);
        QDate date = QDate::fromString(filter, "yyyy-MM-dd");
        if (okNum) {
            query.bindValue(":bExact", num);
        }
        if (date.isValid()) {
            query.bindValue(":dExact", date);
        }
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
		
        QTableWidgetItem *idItem = new QTableWidgetItem(p.m_id);
        idItem->setData(Qt::UserRole, p.m_id);
        idItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        table->setItem(i, 0, idItem);
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
    pdfWriter.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::black);

    QFont titleFont("Arial", 20, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont cellFont("Arial", 10);

    int margin = 10;
    int currentY = margin;
    int pageWidth = pdfWriter.width() - 2 * margin;
    int colCount = table->columnCount();
    QVector<int> colWidths(colCount);
    int equal = pageWidth / (colCount ? colCount : 1);
    for (int c=0;c<colCount;++c) colWidths[c] = equal;

    painter.setFont(titleFont);
    painter.drawText(QRectF(margin, currentY, pageWidth, 28), Qt::AlignLeft | Qt::AlignVCenter, "Gestion De Proejt");
    currentY += 38;

    painter.setFont(headerFont);
    int headerHeight = 40;
    painter.fillRect(margin, currentY, pageWidth, headerHeight, QColor(230, 230, 230));
    int x = margin;
    for (int col = 0; col < colCount; ++col) {
        QString headerText = table->horizontalHeaderItem(col) ? 
            table->horizontalHeaderItem(col)->text() : QString("Col %1").arg(col + 1);
    painter.fillRect(x, currentY, colWidths[col], headerHeight, QColor(230, 230, 230));
    painter.drawRect(x, currentY, colWidths[col], headerHeight);
    painter.drawText(QRectF(x + 6, currentY, colWidths[col] - 12, headerHeight), Qt::AlignLeft | Qt::AlignVCenter, headerText);
        x += colWidths[col];
    }
    currentY += headerHeight;

    painter.setFont(cellFont);
    int rowHeight = 34;
    int rowCount = table->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        if (currentY + rowHeight > pdfWriter.height() - margin) {
            pdfWriter.newPage();
            currentY = margin;
        }
        int x2 = margin;
        for (int col = 0; col < colCount; ++col) {
            QString text = table->item(row, col) ? table->item(row, col)->text() : QString();
    painter.drawRect(x2, currentY, colWidths[col], rowHeight);
    painter.drawText(QRectF(x2 + 6, currentY, colWidths[col] - 12, rowHeight), Qt::AlignLeft | Qt::AlignVCenter, text);
            x2 += colWidths[col];
        }
        currentY += rowHeight;
    }

    // Suppression de la section Statistiques pour un PDF plus simple

    painter.end();
    qDebug() << "PDF exporté avec succès:" << filePath;
    return true;
}
QString Projet::s_lastError;

const QString& Projet::lastError() { return s_lastError; }
