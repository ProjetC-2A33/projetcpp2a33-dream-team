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
#include <QRegularExpression>
#include <QLineEdit>

marque::marque(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::marque)
    , modeEdition(false)
{
    ui->setupUi(this);
    
    ui->tableWidget_2->setColumnCount(5);
    QStringList headers;
    headers << "Nom Marque" << "Activité" << "Durée" << "Budget" << "Catégorie";
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(ui->pushButton_60, SIGNAL(clicked()), this, SLOT(on_pushButton_60_clicked()));
    connect(ui->pushButton_63, SIGNAL(clicked()), this, SLOT(on_pushButton_60_clicked()));
    connect(ui->pushButton_59, SIGNAL(clicked()), this, SLOT(on_pushButton_59_clicked()));
    
    connect(ui->tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
    mettreAJourBoutons();
    afficherMarques();
}

marque::~marque()
{
    delete ui;
}
void marque::on_pushButton_60_clicked()
{
    if (modeEdition) {
        modifierMarque();
    } else {
        // Contrôle de saisie commun
        QString validationError;
        if (!validateInputs(validationError)) {
            QMessageBox::warning(this, "Erreur", validationError);
            return;
        }

        const QString nom = ui->lineEdit_43->text().trimmed();
        const QString activite = ui->l2_13->text().trimmed();
        const QDate   duree = ui->dateEdit->date();
        const int     budget = ui->spinBox->value();
        const QString categorie = ui->l2_14->text().trimmed();

        // Unicité du nom (clé primaire)
        {
            QSqlQuery check;
            check.prepare("SELECT COUNT(*) FROM NOUR.MARQUE WHERE NOM_MARQUE = :nom");
            check.bindValue(":nom", nom);
            if (check.exec() && check.next() && check.value(0).toInt() > 0) {
                QMessageBox::warning(this, "Erreur", "Ce nom de marque existe déjà.");
                return;
            }
        }

        QSqlQuery query;
        query.prepare("INSERT INTO NOUR.MARQUE (NOM_MARQUE, COLUMN1, DUREE, BUDGET, CATEGORIE) VALUES (:nom, :activite, :duree, :budget, :categorie)");
        query.bindValue(":nom", nom);
        query.bindValue(":activite", activite);
        query.bindValue(":duree", duree);
        query.bindValue(":budget", budget);
        query.bindValue(":categorie", categorie);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Marque ajoutée avec succès!");
            ui->lineEdit_43->clear();
            ui->l2_13->clear();
            ui->dateEdit->setDate(QDate::currentDate());
            ui->spinBox->setValue(0);
            ui->l2_14->clear();
            afficherMarques();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + query.lastError().text());
        }
    }
}

void marque::on_pushButton_59_clicked()
{
    if (modeEdition) {
        supprimerMarque();
    } else {
        ui->lineEdit_43->clear();
        ui->l2_13->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->spinBox->setValue(0);
        ui->l2_14->clear();
        ui->tableWidget_2->clearSelection();
        modeEdition = false;
        mettreAJourBoutons();
    }
}

void marque::on_pushButton_63_clicked()
{
    QMessageBox::information(this, "Info", "Fonctionnalité à implémenter");
}

void marque::afficherMarques()
{
    QSqlQuery query;
    
    query.prepare("SELECT NOM_MARQUE, COLUMN1, DUREE, BUDGET, CATEGORIE FROM NOUR.MARQUE ORDER BY NOM_MARQUE");
    
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les marques: " + query.lastError().text());
        return;
    }
    
    ui->tableWidget_2->setRowCount(0);
    int row = 0;
    
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
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
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        row++;
    }
    
    ui->tableWidget_2->resizeColumnsToContents();
}

void marque::afficherProjets()
{
    QMessageBox::information(this, "Info", "Fonctionnalité à implémenter");
}

void marque::modifierMarque()
{
    int row = ui->tableWidget_2->currentRow();
    if (row < 0) {
        // tenter de retrouver par le nom saisi
        const QString nomChamp = ui->lineEdit_43->text().trimmed();
        if (!nomChamp.isEmpty()) {
            row = findRowByNom(nomChamp);
        }
        if (row < 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une marque à modifier!");
            return;
        }
    }
    
    QTableWidgetItem *nomItem = ui->tableWidget_2->item(row, 0);
    if (!nomItem || nomItem->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération du nom de la marque!");
        return;
    }
    
    QString nomAncien = nomItem->text(); // Nom actuel (clé primaire)
    QString validationError;
    if (!validateInputs(validationError)) {
        QMessageBox::warning(this, "Erreur", validationError);
        return;
    }
    const QString nom = ui->lineEdit_43->text().trimmed();
    const QString activite = ui->l2_13->text().trimmed();
    const QDate   duree = ui->dateEdit->date();
    const int     budget = ui->spinBox->value();
    const QString categorie = ui->l2_14->text().trimmed();
    
    QSqlQuery query;
    if (nom != nomAncien) {
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
        ui->tableWidget_2->clearSelection();
        modeEdition = false;
        mettreAJourBoutons();
        afficherMarques();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification: " + query.lastError().text());
    }
}

void marque::supprimerMarque()
{
    int row = ui->tableWidget_2->currentRow();
    if (row < 0) {
        // tenter de retrouver par le nom saisi
        const QString nomChamp = ui->lineEdit_43->text().trimmed();
        if (!nomChamp.isEmpty()) {
            row = findRowByNom(nomChamp);
        }
        if (row < 0) {
            QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une marque à supprimer!");
            return;
        }
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette marque?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        QTableWidgetItem *nomItem = ui->tableWidget_2->item(row, 0);
        if (!nomItem) {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération du nom de la marque!");
            return;
        }
        QString nom = nomItem->text();
        
        QSqlQuery query;
        query.prepare("DELETE FROM NOUR.MARQUE WHERE NOM_MARQUE=:nom");
        query.bindValue(":nom", nom);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Marque supprimée avec succès!");
            ui->lineEdit_43->clear();
            ui->l2_13->clear();
            ui->dateEdit->setDate(QDate::currentDate());
            ui->spinBox->setValue(0);
            ui->l2_14->clear();
            ui->tableWidget_2->clearSelection();
            modeEdition = false;
            mettreAJourBoutons();
            afficherMarques();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + query.lastError().text());
        }
    }
}

void marque::on_tableWidget_2_clicked(const QModelIndex &index)
{
    int row = index.row();
    if (row >= 0 && row < ui->tableWidget_2->rowCount()) {
        QTableWidgetItem *item0 = ui->tableWidget_2->item(row, 0);
        QTableWidgetItem *item1 = ui->tableWidget_2->item(row, 1);
        QTableWidgetItem *item2 = ui->tableWidget_2->item(row, 2);
        QTableWidgetItem *item3 = ui->tableWidget_2->item(row, 3);
        QTableWidgetItem *item4 = ui->tableWidget_2->item(row, 4);
        
        if (item0) ui->lineEdit_43->setText(item0->text());
        if (item1) ui->l2_13->setText(item1->text());
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
        if (item4) ui->l2_14->setText(item4->text());
        
        modeEdition = true;
        mettreAJourBoutons();
    }
}

void marque::on_tableWidget_2_itemSelectionChanged()
{
    if (ui->tableWidget_2->selectedItems().isEmpty()) {
        modeEdition = false;
        mettreAJourBoutons();
    }
}

void marque::mettreAJourBoutons()
{
    if (modeEdition) {
        ui->pushButton_60->setText("Modifier");
        ui->pushButton_59->setText("Supprimer");
        if (ui->pushButton_63) ui->pushButton_63->setText("Modifier");
        if (ui->pushButton_64) ui->pushButton_64->setText("Supprimer");
    } else {
        ui->pushButton_60->setText("Ajouter");
        ui->pushButton_59->setText("Annuler");
        if (ui->pushButton_63) ui->pushButton_63->setText("Ajouter");
        if (ui->pushButton_64) ui->pushButton_64->setText("Annuler");
    }
}

void marque::showContextMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget_2->itemAt(pos);
    if (item) {
        // S'assurer que la ligne sous le curseur est sélectionnée
        int r = item->row();
        if (r >= 0 && r < ui->tableWidget_2->rowCount()) {
            ui->tableWidget_2->setCurrentCell(r, 0);
            ui->tableWidget_2->selectRow(r);
            modeEdition = true;
            mettreAJourBoutons();
        }
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

// Validation des champs communs
bool marque::validateInputs(QString &errorMessage) const
{
    const QString nom = ui->lineEdit_43->text().trimmed();
    const QString activite = ui->l2_13->text().trimmed();
    const QDate   date = ui->dateEdit->date();
    const int     budget = ui->spinBox->value();
    const QString categorie = ui->l2_14->text().trimmed();

    QStringList errors;
    // Règles:
    // - nom: alphanum + espace + - _ ' , 1..20
    // - activité/catégorie: lettres + espaces 1..20
    // - date: valide et entre 1900-01-01 et 2100-12-31
    // - budget: 0..1_000_000
    QRegularExpression nameRe("^[A-Za-z0-9À-ÖØ-öø-ÿ _\'-]{1,20}$");
    QRegularExpression alphaRe("^[A-Za-zÀ-ÖØ-öø-ÿ ]{1,20}$");

    bool okNom = nameRe.match(nom).hasMatch();
    bool okAct = alphaRe.match(activite).hasMatch();
    bool okCat = alphaRe.match(categorie).hasMatch();
    bool okDate = date.isValid() && date >= QDate(1900,1,1) && date <= QDate(2100,12,31);
    bool okBudget = (budget >= 0 && budget <= 1000000);

    if (!okNom) errors << "Nom de la marque invalide (1-20, alphanum/espaces/-/_/\')";
    if (!okAct) errors << "Activité invalide (1-20, lettres/espaces)";
    if (!okCat) errors << "Catégorie invalide (1-20, lettres/espaces)";
    if (!okDate) errors << "Date hors plage (1900-2100) ou invalide";
    if (!okBudget) errors << "Budget doit être entre 0 et 1 000 000";

    // Marquage visuel des champs
    markFieldValidity(ui->lineEdit_43, okNom);
    markFieldValidity(ui->l2_13, okAct);
    markFieldValidity(ui->l2_14, okCat);

    if (!errors.isEmpty()) {
        errorMessage = errors.join("\n");
        return false;
    }
    return true;
}

int marque::findRowByNom(const QString &nom) const
{
    for (int r = 0; r < ui->tableWidget_2->rowCount(); ++r) {
        QTableWidgetItem *it = ui->tableWidget_2->item(r, 0);
        if (it && it->text().compare(nom, Qt::CaseInsensitive) == 0) {
            return r;
        }
    }
    return -1;
}

void marque::markFieldValidity(QWidget *w, bool ok) const
{
    if (!w) return;
    if (ok) {
        w->setStyleSheet("");
    } else {
        w->setStyleSheet("border: 2px solid #d9534f; border-radius: 4px;");
    }
}
