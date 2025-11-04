#include "marque.h"
#include "ui_marque.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QModelIndex>
#include <QAbstractItemView>
#include <QDate>
#include <QStringList>
#include <QMetaType>
#include <QMenu>
#include <QAction>

marque::marque(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::marque)
    , modeEdition(false)
{
    ui->setupUi(this);
    
    // Configuration de la table selon la structure NOUR.MARQUE
    ui->tableWidget_2->setColumnCount(5);
    QStringList headers;
    headers << "Nom Marque" << "Activité" << "Durée" << "Budget" << "Catégorie";
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Connexions manuelles pour les boutons
    connect(ui->pushButton_60, SIGNAL(clicked()), this, SLOT(on_pushButton_60_clicked()));
    connect(ui->pushButton_63, SIGNAL(clicked()), this, SLOT(on_pushButton_60_clicked()));
    connect(ui->pushButton_59, SIGNAL(clicked()), this, SLOT(on_pushButton_59_clicked()));
    
    // Menu contextuel pour modifier/supprimer
    connect(ui->tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
    
    // Initialiser les boutons en mode ajout
    mettreAJourBoutons();
    
    // Afficher les données au démarrage
    afficherMarques();
}

marque::~marque()
{
    delete ui;
}

// Fonction pour ajouter ou modifier une marque
void marque::on_pushButton_60_clicked()
{
    if (modeEdition) {
        // Mode modification
        modifierMarque();
    } else {
        // Mode ajout
        QString nom = ui->lineEdit_43->text();
        QString activite = ui->l2_13->text();
        QDate duree = ui->dateEdit->date();
        int budget = ui->spinBox->value();
        QString categorie = ui->l2_14->text();
        
        // Validation des champs
        if (nom.isEmpty() || activite.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires!");
            return;
        }
        
        QSqlQuery query;
        // Insertion selon la structure NOUR.MARQUE
        // COLUMN1 correspond à l'activité
        query.prepare("INSERT INTO NOUR.MARQUE (NOM_MARQUE, COLUMN1, DUREE, BUDGET, CATEGORIE) VALUES (:nom, :activite, :duree, :budget, :categorie)");
        query.bindValue(":nom", nom);
        query.bindValue(":activite", activite);
        query.bindValue(":duree", duree);
        query.bindValue(":budget", budget);
        query.bindValue(":categorie", categorie);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Marque ajoutée avec succès!");
            // Réinitialiser les champs
            ui->lineEdit_43->clear();
            ui->l2_13->clear();
            ui->dateEdit->setDate(QDate::currentDate());
            ui->spinBox->setValue(0);
            ui->l2_14->clear();
            // Rafraîchir l'affichage
            afficherMarques();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + query.lastError().text());
        }
    }
}

// Fonction pour gérer le bouton Annuler/Supprimer
void marque::on_pushButton_59_clicked()
{
    if (modeEdition) {
        // Mode édition : Supprimer
        supprimerMarque();
    } else {
        // Mode ajout : Annuler (réinitialiser les champs)
        ui->lineEdit_43->clear();
        ui->l2_13->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->spinBox->setValue(0);
        ui->l2_14->clear();
        // Désélectionner la ligne dans le tableau
        ui->tableWidget_2->clearSelection();
        modeEdition = false;
        mettreAJourBoutons();
    }
}

// Fonction pour ajouter un projet
void marque::on_pushButton_63_clicked()
{
    // Cette fonction sera implémentée selon la structure de la table PROJET
    QMessageBox::information(this, "Info", "Fonctionnalité à implémenter");
}

// Fonction pour afficher les marques
void marque::afficherMarques()
{
    QSqlQuery query;
    
    // Requête selon la structure réelle de la table NOUR.MARQUE
    // Colonnes: NOM_MARQUE (PK), BUDGET, CATEGORIE, COLUMN1 (activité), DUREE
    query.prepare("SELECT NOM_MARQUE, COLUMN1, DUREE, BUDGET, CATEGORIE FROM NOUR.MARQUE ORDER BY NOM_MARQUE");
    
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les marques: " + query.lastError().text());
        return;
    }
    
    ui->tableWidget_2->setRowCount(0);
    int row = 0;
    
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        // Colonne 0: NOM_MARQUE (clé primaire)
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        // Colonne 1: COLUMN1 (activité)
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        // Colonne 2: DUREE (date)
        QVariant dateVar = query.value(2);
        QString dateStr;
        if (dateVar.userType() == QMetaType::QDate || dateVar.userType() == QMetaType::QDateTime) {
            QDate date = dateVar.toDate();
            if (date.isValid()) {
                dateStr = date.toString(Qt::ISODate);
            }
        } else {
            dateStr = dateVar.toString();
        }
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(dateStr));
        // Colonne 3: BUDGET
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        // Colonne 4: CATEGORIE
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        row++;
    }
    
    // Ajuster la largeur des colonnes
    ui->tableWidget_2->resizeColumnsToContents();
}

// Fonction pour afficher les projets
void marque::afficherProjets()
{
    // À implémenter selon la structure de la table PROJET
    QMessageBox::information(this, "Info", "Fonctionnalité à implémenter");
}

// Fonction pour modifier une marque
void marque::modifierMarque()
{
    int row = ui->tableWidget_2->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une marque à modifier!");
        return;
    }
    
    // Récupérer le nom de la marque (clé primaire) depuis la première colonne
    QTableWidgetItem *nomItem = ui->tableWidget_2->item(row, 0);
    if (!nomItem || nomItem->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération du nom de la marque!");
        return;
    }
    
    QString nomAncien = nomItem->text(); // Nom actuel (clé primaire)
    QString nom = ui->lineEdit_43->text().trimmed();
    QString activite = ui->l2_13->text().trimmed();
    QDate duree = ui->dateEdit->date();
    int budget = ui->spinBox->value();
    QString categorie = ui->l2_14->text().trimmed();
    
    if (nom.isEmpty() || activite.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires!");
        return;
    }
    
    QSqlQuery query;
    // Mise à jour selon la structure NOUR.MARQUE
    // Si le nom change, on doit d'abord vérifier s'il existe déjà
    if (nom != nomAncien) {
        // Vérifier si le nouveau nom existe déjà
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM NOUR.MARQUE WHERE NOM_MARQUE = :nom");
        checkQuery.bindValue(":nom", nom);
        if (checkQuery.exec() && checkQuery.next()) {
            if (checkQuery.value(0).toInt() > 0) {
                QMessageBox::warning(this, "Erreur", "Une marque avec ce nom existe déjà!");
                return;
            }
        }
    }
    
    // Mettre à jour la marque
    query.prepare("UPDATE NOUR.MARQUE SET NOM_MARQUE=:nom, COLUMN1=:activite, DUREE=:duree, BUDGET=:budget, CATEGORIE=:categorie WHERE NOM_MARQUE=:nomAncien");
    query.bindValue(":nomAncien", nomAncien);
    query.bindValue(":nom", nom);
    query.bindValue(":activite", activite);
    query.bindValue(":duree", duree);
    query.bindValue(":budget", budget);
    query.bindValue(":categorie", categorie);
    
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Marque modifiée avec succès!");
        ui->lineEdit_43->clear();
        ui->l2_13->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->spinBox->setValue(0);
        ui->l2_14->clear();
        // Désélectionner la ligne
        ui->tableWidget_2->clearSelection();
        modeEdition = false;
        mettreAJourBoutons();
        afficherMarques();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification: " + query.lastError().text());
    }
}

// Fonction pour supprimer une marque
void marque::supprimerMarque()
{
    int row = ui->tableWidget_2->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une marque à supprimer!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette marque?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // Récupérer le nom de la marque (clé primaire) depuis la première colonne
        QTableWidgetItem *nomItem = ui->tableWidget_2->item(row, 0);
        if (!nomItem) {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération du nom de la marque!");
            return;
        }
        QString nom = nomItem->text();
        
        // Vérifier si la marque est utilisée dans COLLABORER
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM NOUR.COLLABORER WHERE NOM_MARQUE = :nom");
        checkQuery.bindValue(":nom", nom);
        if (checkQuery.exec() && checkQuery.next()) {
            if (checkQuery.value(0).toInt() > 0) {
                QMessageBox::warning(this, "Erreur", "Impossible de supprimer cette marque car elle est utilisée dans des collaborations!");
                return;
            }
        }
        
        QSqlQuery query;
        // Suppression selon la structure NOUR.MARQUE (NOM_MARQUE est la clé primaire)
        query.prepare("DELETE FROM NOUR.MARQUE WHERE NOM_MARQUE=:nom");
        query.bindValue(":nom", nom);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Marque supprimée avec succès!");
            // Réinitialiser les champs
            ui->lineEdit_43->clear();
            ui->l2_13->clear();
            ui->dateEdit->setDate(QDate::currentDate());
            ui->spinBox->setValue(0);
            ui->l2_14->clear();
            // Désélectionner la ligne
            ui->tableWidget_2->clearSelection();
            modeEdition = false;
            mettreAJourBoutons();
            afficherMarques();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + query.lastError().text());
        }
    }
}

// Fonction pour charger les données dans les champs lors de la sélection
void marque::on_tableWidget_2_clicked(const QModelIndex &index)
{
    int row = index.row();
    if (row >= 0 && row < ui->tableWidget_2->rowCount()) {
        // Colonne 0: NOM_MARQUE
        QTableWidgetItem *item0 = ui->tableWidget_2->item(row, 0);
        // Colonne 1: COLUMN1 (activité)
        QTableWidgetItem *item1 = ui->tableWidget_2->item(row, 1);
        // Colonne 2: DUREE
        QTableWidgetItem *item2 = ui->tableWidget_2->item(row, 2);
        // Colonne 3: BUDGET
        QTableWidgetItem *item3 = ui->tableWidget_2->item(row, 3);
        // Colonne 4: CATEGORIE
        QTableWidgetItem *item4 = ui->tableWidget_2->item(row, 4);
        
        if (item0) ui->lineEdit_43->setText(item0->text()); // Nom marque
        if (item1) ui->l2_13->setText(item1->text()); // Activité
        if (item2) {
            QDate date = QDate::fromString(item2->text(), Qt::ISODate);
            if (date.isValid()) {
                ui->dateEdit->setDate(date);
            }
        }
        if (item3) {
            bool ok;
            int value = item3->text().toInt(&ok);
            if (ok) {
                ui->spinBox->setValue(value);
            }
        }
        if (item4) ui->l2_14->setText(item4->text()); // Catégorie
        
        // Passer en mode édition
        modeEdition = true;
        mettreAJourBoutons();
    }
}

// Fonction appelée quand la sélection change
void marque::on_tableWidget_2_itemSelectionChanged()
{
    if (ui->tableWidget_2->selectedItems().isEmpty()) {
        // Aucune ligne sélectionnée, retour en mode ajout
        modeEdition = false;
        mettreAJourBoutons();
    }
}

// Fonction pour mettre à jour les textes des boutons
void marque::mettreAJourBoutons()
{
    if (modeEdition) {
        // Mode édition : Ajouter devient Modifier, Annuler devient Supprimer
        ui->pushButton_60->setText("Modifier");
        ui->pushButton_59->setText("Supprimer");
        // Mettre à jour aussi pushButton_63 et pushButton_64 si ils existent
        if (ui->pushButton_63) ui->pushButton_63->setText("Modifier");
        if (ui->pushButton_64) ui->pushButton_64->setText("Supprimer");
    } else {
        // Mode ajout : Modifier devient Ajouter, Supprimer devient Annuler
        ui->pushButton_60->setText("Ajouter");
        ui->pushButton_59->setText("Annuler");
        // Mettre à jour aussi pushButton_63 et pushButton_64 si ils existent
        if (ui->pushButton_63) ui->pushButton_63->setText("Ajouter");
        if (ui->pushButton_64) ui->pushButton_64->setText("Annuler");
    }
}

// Fonction pour afficher le menu contextuel
void marque::showContextMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget_2->itemAt(pos);
    if (item) {
        QMenu menu(this);
        QAction *modifierAction = menu.addAction("Modifier");
        QAction *supprimerAction = menu.addAction("Supprimer");
        QAction *selectedAction = menu.exec(ui->tableWidget_2->viewport()->mapToGlobal(pos));
        if (selectedAction == modifierAction) {
            modifierMarque();
        } else if (selectedAction == supprimerAction) {
            supprimerMarque();
        }
    }
}
