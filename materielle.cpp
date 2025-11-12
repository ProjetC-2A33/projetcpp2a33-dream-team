#include "materielle.h"
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>
#include <QVariant>

Materielle::Materielle()
{
    currentTableName = "";
}

QString Materielle::findTableName()
{
    QStringList possibleNames = {"ATELIER", "MATERIEL", "MATERIELS", "atelier", "materiel", "MATERIEL_TABLE"};
    
    for (const QString &tableName : possibleNames) {
        QSqlQuery testQuery;
        testQuery.prepare("SELECT COUNT(*) FROM " + tableName);
        if (testQuery.exec()) {
            testQuery.finish();
            qDebug() << "Table found: " << tableName;
            currentTableName = tableName;
            return tableName;
        }
        testQuery.finish();
    }
    
    // Try to query user tables to find matching table
    QSqlQuery metaQuery;
    if (metaQuery.exec("SELECT TABLE_NAME FROM USER_TABLES WHERE UPPER(TABLE_NAME) LIKE '%MATERIEL%' OR UPPER(TABLE_NAME) LIKE '%ATELIER%'")) {
        if (metaQuery.next()) {
            QString foundTable = metaQuery.value(0).toString();
            metaQuery.finish();
            qDebug() << "Table found via metadata: " << foundTable;
            currentTableName = foundTable;
            return foundTable;
        }
        metaQuery.finish();
    }
    
    return ""; // Not found
}

QString Materielle::getTableName()
{
    if (currentTableName.isEmpty()) {
        currentTableName = findTableName();
    }
    return currentTableName;
}

bool Materielle::idExists(const QString &idMateriel)
{
    QString tableName = getTableName();
    if (tableName.isEmpty()) {
        return false;
    }
    
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT IDMATERIEL FROM " + tableName + " WHERE IDMATERIEL = :id");
    checkQuery.bindValue(":id", idMateriel);
    if (checkQuery.exec()) {
        bool exists = checkQuery.next();
        checkQuery.finish();
        return exists;
    }
    checkQuery.finish();
    return false;
}

Materielle::MaterielData Materielle::getMaterielById(const QString &idMateriel)
{
    MaterielData data;
    QString tableName = getTableName();
    if (tableName.isEmpty()) {
        return data;
    }
    
    QSqlQuery query;
    query.prepare("SELECT IDMATERIEL, ETAT_MATERIEL, LACALISATION, DATE_ACHAT, NB_MATERIEL, TYPE_MATERIEL, SUIVI FROM " + tableName + " WHERE IDMATERIEL = :id");
    query.bindValue(":id", idMateriel);
    
    if (query.exec() && query.next()) {
        data.idMateriel = query.value(0).toString();
        data.etatMateriel = query.value(1).toString();
        data.localisation = query.value(2).toString();
        
        if (query.value(3).type() == QVariant::Date) {
            data.dateAchat = query.value(3).toDate();
        } else {
            data.dateAchat = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
        }
        
        data.nbMateriel = query.value(4).toInt();
        data.typeMateriel = query.value(5).toString();
        data.suivi = query.value(6).toString();
    }
    query.finish();
    
    return data;
}

// CREATE - Ajouter un matériel
bool Materielle::ajouter(const MaterielData &data, QWidget *parent)
{
    // Validation
    if (data.idMateriel.isEmpty() || data.etatMateriel.isEmpty() || 
        data.localisation.isEmpty() || data.typeMateriel.isEmpty()) {
        if (parent) {
            QMessageBox::warning(parent, "Attention", "Veuillez remplir tous les champs obligatoires!");
        }
        return false;
    }
    
    QString tableName = getTableName();
    if (tableName.isEmpty()) {
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Impossible de trouver la table dans la base de données.\nVeuillez vérifier le nom de la table.");
        }
        return false;
    }
    
    // Check if ID already exists
    if (idExists(data.idMateriel)) {
        if (parent) {
            QMessageBox::warning(parent, "Attention", "Cet ID matériel existe déjà!");
        }
        return false;
    }
    
    // Insert into database
    QSqlQuery query;
    query.prepare("INSERT INTO " + tableName + " (IDMATERIEL, ETAT_MATERIEL, LACALISATION, DATE_ACHAT, NB_MATERIEL, TYPE_MATERIEL, SUIVI) "
                  "VALUES (:id, :etat, :localisation, TO_DATE(:date, 'YYYY-MM-DD'), :nb, :type, :suivi)");
    query.bindValue(":id", data.idMateriel);
    query.bindValue(":etat", data.etatMateriel);
    query.bindValue(":localisation", data.localisation);
    query.bindValue(":date", data.dateAchat.toString("yyyy-MM-dd"));
    query.bindValue(":nb", data.nbMateriel);
    query.bindValue(":type", data.typeMateriel);
    query.bindValue(":suivi", data.suivi.isEmpty() ? "En cours" : data.suivi);
    
    if (query.exec()) {
        query.finish();
        if (parent) {
            QMessageBox::information(parent, "Succès", "Matériel ajouté avec succès!");
        }
        return true;
    } else {
        QString errorMsg = query.lastError().text();
        query.finish();
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Erreur lors de l'ajout: " + errorMsg);
        }
        qDebug() << "SQL Error:" << errorMsg;
        qDebug() << "Last Query:" << query.lastQuery();
        return false;
    }
}

// UPDATE - Modifier un matériel
bool Materielle::modifier(const QString &oldId, const MaterielData &data, QWidget *parent)
{
    // Validation
    if (data.idMateriel.isEmpty() || data.etatMateriel.isEmpty() || 
        data.localisation.isEmpty() || data.typeMateriel.isEmpty()) {
        if (parent) {
            QMessageBox::warning(parent, "Attention", "Veuillez remplir tous les champs obligatoires!");
        }
        return false;
    }
    
    QString tableName = getTableName();
    if (tableName.isEmpty()) {
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Impossible de trouver la table dans la base de données.\nVeuillez vérifier le nom de la table.");
        }
        return false;
    }
    
    // Check if new ID conflicts with existing ID (if ID changed)
    if (data.idMateriel != oldId && idExists(data.idMateriel)) {
        if (parent) {
            QMessageBox::warning(parent, "Attention", "Cet ID matériel existe déjà!");
        }
        return false;
    }
    
    // Update database
    QSqlQuery query;
    query.prepare("UPDATE " + tableName + " SET IDMATERIEL = :id, ETAT_MATERIEL = :etat, LACALISATION = :localisation, "
                  "DATE_ACHAT = TO_DATE(:date, 'YYYY-MM-DD'), NB_MATERIEL = :nb, TYPE_MATERIEL = :type, SUIVI = :suivi "
                  "WHERE IDMATERIEL = :oldId");
    query.bindValue(":id", data.idMateriel);
    query.bindValue(":etat", data.etatMateriel);
    query.bindValue(":localisation", data.localisation);
    query.bindValue(":date", data.dateAchat.toString("yyyy-MM-dd"));
    query.bindValue(":nb", data.nbMateriel);
    query.bindValue(":type", data.typeMateriel);
    query.bindValue(":suivi", data.suivi.isEmpty() ? "En cours" : data.suivi);
    query.bindValue(":oldId", oldId);
    
    if (query.exec()) {
        query.finish();
        if (parent) {
            QMessageBox::information(parent, "Succès", "Matériel modifié avec succès!");
        }
        return true;
    } else {
        QString errorMsg = query.lastError().text();
        query.finish();
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Erreur lors de la modification: " + errorMsg);
        }
        qDebug() << "SQL Error:" << errorMsg;
        return false;
    }
}

// DELETE - Supprimer un matériel
bool Materielle::supprimer(const QString &idMateriel, QWidget *parent)
{
    if (idMateriel.isEmpty()) {
        if (parent) {
            QMessageBox::warning(parent, "Attention", "Veuillez sélectionner un matériel à supprimer!");
        }
        return false;
    }
    
    QString tableName = getTableName();
    if (tableName.isEmpty()) {
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Impossible de trouver la table dans la base de données.\nVeuillez vérifier le nom de la table.");
        }
        return false;
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM " + tableName + " WHERE IDMATERIEL = :id");
    query.bindValue(":id", idMateriel);
    
    if (query.exec()) {
        query.finish();
        if (parent) {
            QMessageBox::information(parent, "Succès", "Matériel supprimé avec succès!");
        }
        return true;
    } else {
        QString errorMsg = query.lastError().text();
        query.finish();
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Erreur lors de la suppression: " + errorMsg);
        }
        qDebug() << "SQL Error:" << errorMsg;
        return false;
    }
}

// READ - Lire tous les matériels et les afficher dans le tableau
bool Materielle::lire(QTableWidget *tableWidget, QWidget *parent)
{
    if (!tableWidget) {
        return false;
    }
    
    QString tableName = getTableName();
    if (tableName.isEmpty()) {
        if (parent) {
            QMessageBox::critical(parent, "Erreur", 
                "Impossible de trouver la table dans la base de données.\n\n"
                "Vérifiez que:\n"
                "- La table existe (nom possible: ATELIER, MATERIEL, etc.)\n"
                "- Vous avez les permissions nécessaires\n"
                "- La connexion à la base de données est active\n\n"
                "Veuillez vérifier le nom exact de la table dans votre base de données Oracle.");
        }
        return false;
    }
    
    QSqlQuery query;
    query.prepare("SELECT IDMATERIEL, ETAT_MATERIEL, LACALISATION, DATE_ACHAT, NB_MATERIEL, TYPE_MATERIEL, SUIVI FROM " + tableName + " ORDER BY IDMATERIEL");
    
    if (!query.exec()) {
        if (parent) {
            QMessageBox::critical(parent, "Erreur", "Erreur lors du chargement des données: " + query.lastError().text() + 
                                  "\n\nTable utilisée: " + tableName +
                                  "\n\nVérifiez que:\n- Les colonnes existent dans la table\n- La connexion à la base de données est active");
        }
        qDebug() << "SQL Error:" << query.lastError().text();
        qDebug() << "Last Query:" << query.lastQuery();
        query.finish();
        return false;
    }

    // Set table columns
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Etat" << "Localisation" << "Date Achat" << "Nombre" << "Type" << "Suivi");
    
    // Enable sorting on table headers
    tableWidget->setSortingEnabled(true);
    
    // Clear existing rows
    tableWidget->setRowCount(0);
    
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        
        // IDMATERIEL
        if (!query.value(0).isNull()) {
            tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        } else {
            tableWidget->setItem(row, 0, new QTableWidgetItem(""));
        }
        
        // ETAT_MATERIEL
        if (!query.value(1).isNull()) {
            tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        } else {
            tableWidget->setItem(row, 1, new QTableWidgetItem(""));
        }
        
        // LACALISATION
        if (!query.value(2).isNull()) {
            tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        } else {
            tableWidget->setItem(row, 2, new QTableWidgetItem(""));
        }
        
        // DATE_ACHAT - Handle both DATE and string formats
        QString dateStr = "";
        if (!query.value(3).isNull()) {
            if (query.value(3).type() == QVariant::Date) {
                QDate date = query.value(3).toDate();
                if (date.isValid()) {
                    dateStr = date.toString("yyyy-MM-dd");
                }
            } else {
                dateStr = query.value(3).toString();
            }
        }
        tableWidget->setItem(row, 3, new QTableWidgetItem(dateStr));
        
        // NB_MATERIEL
        if (!query.value(4).isNull()) {
            tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        } else {
            tableWidget->setItem(row, 4, new QTableWidgetItem(""));
        }
        
        // TYPE_MATERIEL
        if (!query.value(5).isNull()) {
            tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        } else {
            tableWidget->setItem(row, 5, new QTableWidgetItem(""));
        }
        
        // SUIVI
        if (!query.value(6).isNull()) {
            tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        } else {
            tableWidget->setItem(row, 6, new QTableWidgetItem(""));
        }
        
        row++;
    }
    
    tableWidget->resizeColumnsToContents();
    query.finish();
    qDebug() << "Loaded" << row << "rows from table" << tableName;
    
    return true;
}

