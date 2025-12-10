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
#include <QGroupBox>
#include <QTableWidget>
#include <QVariant>
#include <QList>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
// Define the static member variable (MUST BE AT TOP LEVEL)
QString Projet::s_lastError = "";

// Constructeur par défaut
Projet::Projet() : m_budget(0.0) {
}

// Constructeur avec paramètres
Projet::Projet(const QString& id, const QString& email, const QDate& deadline,
               double budget, const QString& typeProjet)
    : m_id(id), m_email(email), m_deadline(deadline), m_budget(budget),
      m_typeProjet(typeProjet) {
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
bool Projet::add(const Projet& p) {/*
    if (!isValid(p)) {
        return false;
    }

    QSqlDatabase& db = Connection::createInstance().getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Erreur: Base de données non connectée";
        s_lastError = "Base de données non connectée";
        return false;
    }

    QSqlQuery query(db);
    // CORRECTED: Changed from LAYTH.PROJETS to ISSAM.PROJETS
    query.prepare("INSERT INTO ISSAM.PROJETS (ID_P, E_MAIL, DEADLINE, BUDGET, TYPE_PROJET) "
        "VALUES (:id, :email, :deadline, :budget, :typeProjet)");

    query.bindValue(":id", p.m_id);
    query.bindValue(":email", p.m_email);
    query.bindValue(":deadline", p.m_deadline);
    query.bindValue(":budget", p.m_budget);
    query.bindValue(":typeProjet", p.m_typeProjet);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout:" << query.lastError().text();
        s_lastError = query.lastError().text();
        return false;
    }

    qDebug() << "Projet ajouté avec succès: ID_P =" << p.m_id;
    s_lastError.clear();
    return true;*/
}

// Modifier un projet
bool Projet::update(const Projet& p) {
    /*
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
    // CORRECTED: Changed from LAYTH.PROJETS to ISSAM.PROJETS
    query.prepare("UPDATE ISSAM.PROJETS SET "
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
    return true;*/
}

bool Projet::updateById(const QString& oldId, const Projet& p) {
    /*
    if (!isValid(p)) {
        s_lastError = "Validation invalide";
        return false;
    }

    QSqlDatabase& db = Connection::createInstance().getDatabase();
    if (!db.isOpen()) {
        s_lastError = "DB fermée";
        return false;
    }

    QSqlQuery query(db);
    // CORRECTED: Changed from LAYTH.PROJETS to ISSAM.PROJETS
    query.prepare("UPDATE ISSAM.PROJETS SET "
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

    if (!query.exec()) {
        s_lastError = query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        s_lastError = QString("Aucun projet trouvé avec ID_P = %1").arg(oldId);
        return false;
    }

    s_lastError.clear();
    return true;*/
}

// Supprimer un projet par ID
bool Projet::removeById(const QString& id) {
    /*
    if (id.isEmpty()) {
        s_lastError = "ID_P vide";
        return false;
    }

    QSqlDatabase& db = Connection::createInstance().getDatabase();
    if (!db.isOpen()) {
        s_lastError = "Base de données non connectée";
        return false;
    }

    QSqlQuery query(db);
    // CORRECTED: Changed from LAYTH.PROJETS to ISSAM.PROJETS
    query.prepare("DELETE FROM ISSAM.PROJETS WHERE ID_P = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        s_lastError = query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        s_lastError = QString("Aucun projet trouvé avec ID_P = %1").arg(id);
        return false;
    }

    s_lastError.clear();
    return true;*/
}

// Récupérer tous les projets (optionnel: filtre par ID ou email)
QVector<Projet> Projet::fetchAll(const QString& filter) {
    /*
    QVector<Projet> projets;
    s_lastError.clear();

    QSqlDatabase& db = Connection::createInstance().getDatabase();
    if (!db.isOpen()) {
        s_lastError = "Base de données non connectée";
        return projets;
    }

    QSqlQuery query(db);
    // CORRECTED: Changed from LAYTH.PROJETS to ISSAM.PROJETS
    QString sql = "SELECT ID_P, E_MAIL, DEADLINE, BUDGET, TYPE_PROJET FROM ISSAM.PROJETS";

    if (!filter.isEmpty()) {
        QString where = " WHERE ID_P LIKE :f OR E_MAIL LIKE :f OR TYPE_PROJET LIKE :f OR TO_CHAR(BUDGET) LIKE :f OR TO_CHAR(DEADLINE, 'YYYY-MM-DD') LIKE :f";
        bool okNum = false;
        filter.toDouble(&okNum);
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
        bool okNum = false;
        double num = filter.toDouble(&okNum);
        QDate date = QDate::fromString(filter, "yyyy-MM-dd");

        if (okNum) {
            query.bindValue(":bExact", num);
        }
        if (date.isValid()) {
            query.bindValue(":dExact", date);
        }
    }

    if (!query.exec()) {
        s_lastError = query.lastError().text();
        return projets;
    }

    while (query.next()) {
        Projet p(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toDate(),
            query.value(3).toDouble(),
            query.value(4).toString()
        );
        projets.append(p);
    }

    return projets;*/
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
    // Limiter à 50 caractères pour correspondre à votre validation
    if (result.length() > 50) {
        result = result.left(50);
    }

    return result;
}

// Exporter le tableau en PDF
bool Projet::exportTableToPdf(QTableWidget* table, const QString& filePath) {
   /* if (!table) {
        s_lastError = "TableWidget est null";
        return false;
    }

    if (filePath.isEmpty()) {
        s_lastError = "Chemin de fichier vide";
        return false;
    }

    // Check if table has data
    if (table->rowCount() == 0) {
        s_lastError = "Le tableau est vide - aucune donnée à exporter";
        return false;
    }

    try {
        QPdfWriter pdfWriter(filePath);
        // Set A4 page size
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        // Set landscape orientation for better table display
        pdfWriter.setPageOrientation(QPageLayout::Landscape);
        pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
        pdfWriter.setTitle("Liste des Projets");
        pdfWriter.setCreator("Gestion de Projets Application");

        QPainter painter(&pdfWriter);
        if (!painter.isActive()) {
            s_lastError = "Impossible d'initialiser le painter pour PDF";
            return false;
        }

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);

        // Font definitions
        QFont titleFont("Arial", 18, QFont::Bold);
        QFont headerFont("Arial", 10, QFont::Bold);
        QFont dataFont("Arial", 9);
        QFont footerFont("Arial", 8);

        // Page dimensions
        int pageWidth = pdfWriter.width();
        int pageHeight = pdfWriter.height();
        int margin = 40;
        int usableWidth = pageWidth - 2 * margin;

        int currentY = margin;

        // Column widths (adjust based on your actual data)
        int colCount = table->columnCount();
        QVector<int> colWidths(colCount);

        // Calculate column widths based on content
        int totalTextWidth = 0;
        QFontMetrics fm(dataFont);

        for (int col = 0; col < colCount; ++col) {
            int maxWidth = fm.width(table->horizontalHeaderItem(col) ?
                                   table->horizontalHeaderItem(col)->text() :
                                   QString("Colonne %1").arg(col+1)) + 20;

            for (int row = 0; row < table->rowCount(); ++row) {
                QTableWidgetItem* item = table->item(row, col);
                if (item) {
                    int textWidth = fm.width(item->text()) + 20;
                    if (textWidth > maxWidth) maxWidth = textWidth;
                }
            }

            // Limit maximum column width
            if (maxWidth > 250) maxWidth = 250;
            if (maxWidth < 80) maxWidth = 80;

            colWidths[col] = maxWidth;
            totalTextWidth += maxWidth;
        }

        // Adjust column widths to fit page
        if (totalTextWidth > usableWidth) {
            double ratio = (double)usableWidth / totalTextWidth;
            for (int col = 0; col < colCount; ++col) {
                colWidths[col] = static_cast<int>(colWidths[col] * ratio);
            }
        }

        // Draw title
        painter.setFont(titleFont);
        painter.setPen(Qt::darkBlue);
        painter.drawText(QRectF(margin, currentY, usableWidth, 30),
                        Qt::AlignCenter, "📋 LISTE DES PROJETS");
        currentY += 40;

        // Draw date and time
        painter.setFont(QFont("Arial", 9));
        painter.setPen(Qt::darkGray);
        QString dateInfo = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
        painter.drawText(QRectF(margin, currentY, usableWidth - 100, 20),
                        Qt::AlignLeft, "Date d'exportation: " + dateInfo);
        painter.drawText(QRectF(margin + usableWidth - 100, currentY, 100, 20),
                        Qt::AlignRight, QString("Total: %1 projets").arg(table->rowCount()));
        currentY += 30;

        // Draw table header
        painter.setFont(headerFont);
        painter.setPen(Qt::white);
        painter.setBrush(QBrush(QColor(70, 130, 180))); // Steel blue color

        int headerHeight = 35;

        // Draw header background
        painter.drawRect(margin, currentY, usableWidth, headerHeight);

        // Draw header text
        int xPos = margin;
        for (int col = 0; col < colCount; ++col) {
            QString headerText = table->horizontalHeaderItem(col) ?
                                table->horizontalHeaderItem(col)->text() :
                                QString("Colonne %1").arg(col+1);

            painter.drawText(QRectF(xPos + 5, currentY + 5, colWidths[col] - 10, headerHeight - 10),
                            Qt::AlignCenter | Qt::TextWordWrap, headerText);

            // Draw vertical separator
            if (col < colCount - 1) {
                painter.drawLine(xPos + colWidths[col], currentY,
                               xPos + colWidths[col], currentY + headerHeight);
            }

            xPos += colWidths[col];
        }

        currentY += headerHeight;

        // Draw table data
        painter.setFont(dataFont);
        int rowHeight = 35;

        for (int row = 0; row < table->rowCount(); ++row) {
            // Check for page break
            if (currentY + rowHeight > pageHeight - margin - 50) {
                pdfWriter.newPage();
                currentY = margin;

                // Redraw header on new page
                painter.setFont(headerFont);
                painter.setPen(Qt::white);
                painter.setBrush(QBrush(QColor(70, 130, 180)));
                painter.drawRect(margin, currentY, usableWidth, headerHeight);

                xPos = margin;
                for (int col = 0; col < colCount; ++col) {
                    QString headerText = table->horizontalHeaderItem(col) ?
                                        table->horizontalHeaderItem(col)->text() :
                                        QString("Colonne %1").arg(col+1);

                    painter.drawText(QRectF(xPos + 5, currentY + 5, colWidths[col] - 10, headerHeight - 10),
                                    Qt::AlignCenter | Qt::TextWordWrap, headerText);

                    if (col < colCount - 1) {
                        painter.drawLine(xPos + colWidths[col], currentY,
                                       xPos + colWidths[col], currentY + headerHeight);
                    }

                    xPos += colWidths[col];
                }

                currentY += headerHeight;
                painter.setFont(dataFont);
            }

            // Alternate row colors for readability
            painter.setPen(Qt::black);
            if (row % 2 == 0) {
                painter.setBrush(QBrush(QColor(245, 245, 245))); // Light gray
            } else {
                painter.setBrush(QBrush(Qt::white));
            }

            // Draw row background
            painter.drawRect(margin, currentY, usableWidth, rowHeight);

            // Draw cell data
            xPos = margin;
            for (int col = 0; col < colCount; ++col) {
                QString cellText = "";
                QTableWidgetItem* item = table->item(row, col);
                if (item) {
                    cellText = item->text();
                }

                // Special formatting for specific columns
                if (col == 3) { // Budget column
                    bool ok;
                    double budget = cellText.toDouble(&ok);
                    if (ok) {
                        painter.setPen(Qt::darkGreen);
                        cellText = QString("%1 DT").arg(budget, 0, 'f', 2);
                    }
                } else if (col == 2) { // Date column
                    QDate date = QDate::fromString(cellText, "yyyy-MM-dd");
                    if (date.isValid()) {
                        painter.setPen(Qt::darkBlue);
                        cellText = date.toString("dd/MM/yyyy");
                    }
                } else {
                    painter.setPen(Qt::black);
                }

                painter.drawText(QRectF(xPos + 5, currentY + 5, colWidths[col] - 10, rowHeight - 10),
                                Qt::AlignCenter | Qt::TextWordWrap, cellText);

                // Draw vertical separator
                if (col < colCount - 1) {
                    painter.drawLine(xPos + colWidths[col], currentY,
                                   xPos + colWidths[col], currentY + rowHeight);
                }

                xPos += colWidths[col];
            }

            // Reset pen color
            painter.setPen(Qt::black);
            currentY += rowHeight;
        }

        // Draw outer border
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(margin, margin + 70, usableWidth, currentY - margin - 70);

        // Add summary section
        currentY += 30;
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.setPen(Qt::darkBlue);

        // Calculate total budget
        double totalBudget = 0.0;
        int budgetColumnIndex = -1;

        // Find budget column
        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem* header = table->horizontalHeaderItem(col);
            if (header && header->text().toUpper().contains("BUDGET")) {
                budgetColumnIndex = col;
                break;
            }
        }

        // Calculate total if budget column exists
        if (budgetColumnIndex != -1) {
            for (int row = 0; row < table->rowCount(); ++row) {
                QTableWidgetItem* item = table->item(row, budgetColumnIndex);
                if (item) {
                    bool ok;
                    double value = item->text().toDouble(&ok);
                    if (ok) {
                        totalBudget += value;
                    }
                }
            }

            painter.drawText(QRectF(margin, currentY, usableWidth, 25),
                            Qt::AlignLeft,
                            QString("Budget total: %1 DT").arg(totalBudget, 0, 'f', 2));
            currentY += 20;
        }

        // Add footer
        painter.setFont(footerFont);
        painter.setPen(Qt::gray);
        QString footerText = QString("Document généré par Gestion de Projets Application - Page 1");
        painter.drawText(QRectF(margin, pageHeight - 30, usableWidth, 20),
                        Qt::AlignCenter, footerText);

        painter.end();

        s_lastError.clear();
        qDebug() << "✅ PDF exporté avec succès vers:" << filePath;
        qDebug() << "   Nombre de projets exportés:" << table->rowCount();
        qDebug() << "   Taille de page:" << pageWidth << "x" << pageHeight;
        qDebug() << "   Largeur utilisable:" << usableWidth;

        return true;

    } catch (const std::exception& e) {
        s_lastError = QString("Erreur lors de la création du PDF: %1").arg(e.what());
        qDebug() << "❌ Erreur PDF:" << s_lastError;
        return false;
    } catch (...) {
        s_lastError = "Erreur inconnue lors de la création du PDF";
        qDebug() << "❌ Erreur PDF inconnue";
        return false;
    }*/
}
