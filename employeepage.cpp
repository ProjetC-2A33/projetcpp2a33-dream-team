#include "employeepage.h"
#include "ui_employeepage.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QTableWidget>
#include <QAbstractItemModel>
#include <QCheckBox>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>
#include <QTableWidgetItem>
#include <QModelIndex>
#include <initializer_list>

// ============================================================================
// IMPLEMENTATION DE LA CLASSE EMPLOYE
// ============================================================================

namespace {
QString normalizeColumn(const QString &name)
{
    QString normalized = name;
    normalized.remove('_');
    normalized = normalized.toLower();
    return normalized;
}

struct ColumnMapping {
    QString id;
    QString nomPrenom;
    QString numTel;
    QString salaire;
    QString email;
    QString typeEmploye;
    bool isValid() const {
        return !id.isEmpty() && !nomPrenom.isEmpty() && !numTel.isEmpty() &&
               !salaire.isEmpty() && !email.isEmpty() && !typeEmploye.isEmpty();
    }
};

ColumnMapping resolveColumns()
{
    static ColumnMapping mapping;
    static bool resolved = false;

    if (resolved) {
        return mapping;
    }

    QSqlQuery query("SELECT * FROM EMPLOYE WHERE 1=0");
    QSqlRecord record = query.record();
    QStringList columns;
    for (int i = 0; i < record.count(); ++i) {
        columns << record.fieldName(i);
    }

    auto findColumn = [&](std::initializer_list<QString> keys) -> QString {
        for (const QString &key : keys) {
            const QString normalizedKey = normalizeColumn(key);
            for (const QString &column : columns) {
                if (normalizeColumn(column) == normalizedKey) {
                    return column;
                }
            }
        }
        return QString();
    };

    mapping.id = findColumn({"IDEMPLOYE", "ID_EMPLOYE", "ID"});
    mapping.nomPrenom = findColumn({"NOM_PRENOM", "NOM", "NOMEMPLOYE", "NOM_COMPLET"});
    mapping.numTel = findColumn({"NUM_TEL", "NUMTEL", "TELEPHONE", "TEL", "NOM_TEL"});
    mapping.salaire = findColumn({"SALAIRE", "SALAIRE_EMPLOYE", "SALARY"});
    mapping.email = findColumn({"E_MAIL", "EMAIL", "MAIL"});
    mapping.typeEmploye = findColumn({"TYPE_EMPLOYE", "TYPE", "POSTE"});

    resolved = true;

    if (!mapping.isValid()) {
        qDebug() << "❗ Impossible de résoudre toutes les colonnes de la table EMPLOYE."
                 << "Colonnes trouvées:" << columns;
    } else {
        qDebug() << "Colonnes EMPLOYE détectées:" << mapping.id << mapping.nomPrenom
                 << mapping.numTel << mapping.salaire << mapping.email << mapping.typeEmploye;
    }

    return mapping;
}
} // namespace anonyme

// Constructeurs
Employe::Employe() {
    id_employe = 0;
    num_tel = 0;
    salaire = 0.0;
}

Employe::Employe(int id, QString nom, int tel, float sal, QString mail, QString type) {
    id_employe = id;
    nom_prenom = nom;
    num_tel = tel;
    salaire = sal;
    e_mail = mail;
    type_employe = type;
}

// Getters
int Employe::getId() { return id_employe; }
QString Employe::getNomPrenom() { return nom_prenom; }
int Employe::getNumTel() { return num_tel; }
float Employe::getSalaire() { return salaire; }
QString Employe::getEmail() { return e_mail; }
QString Employe::getTypeEmploye() { return type_employe; }

// Ajouter
bool Employe::ajouter() {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Ajout employe impossible: colonnes manquantes.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("INSERT INTO EMPLOYE (%1, %2, %3, %4, %5, %6) "
                                 "VALUES (:id, :nom, :tel, :sal, :mail, :type)")
                      .arg(columns.id, columns.nomPrenom, columns.numTel,
                           columns.salaire, columns.email, columns.typeEmploye));
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom_prenom);
    query.bindValue(":tel", num_tel);
    query.bindValue(":sal", salaire);
    query.bindValue(":mail", e_mail);
    query.bindValue(":type", type_employe);
    const bool ok = query.exec();
    if (!ok) {
        qDebug() << "Ajout employe échoué:" << query.lastError().text();
    }
    return ok;
}

// Afficher
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom et prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("E-mail"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type d'employé"));
    return model;
}

// Supprimer
bool Employe::supprimer(int id) {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Suppression employe impossible: colonnes manquantes.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("DELETE FROM EMPLOYE WHERE %1 = :id").arg(columns.id));
    query.bindValue(":id", id);
    const bool ok = query.exec();
    if (!ok) {
        qDebug() << "Suppression employe échouée:" << query.lastError().text();
    }
    return ok;
}

// Modifier
bool Employe::modifier() {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Modification employe impossible: colonnes manquantes.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("UPDATE EMPLOYE SET %1=:nom, %2=:tel, %3=:sal, %4=:mail, %5=:type "
                                 "WHERE %6=:id")
                      .arg(columns.nomPrenom, columns.numTel, columns.salaire,
                           columns.email, columns.typeEmploye, columns.id));
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom_prenom);
    query.bindValue(":tel", num_tel);
    query.bindValue(":sal", salaire);
    query.bindValue(":mail", e_mail);
    query.bindValue(":type", type_employe);
    const bool ok = query.exec();
    if (!ok) {
        qDebug() << "Modification employe échouée:" << query.lastError().text();
    }
    return ok;
}

// ============================================================================
// IMPLEMENTATION DE LA CLASSE EMPLOYEEPAGE
// ============================================================================

// Helper to copy a QAbstractItemModel into a QTableWidget
static void fillTableWidgetFromModel(QTableWidget *tableWidget, QAbstractItemModel *model)
{
    if (!tableWidget || !model) return;

    tableWidget->clear();
    tableWidget->setRowCount(model->rowCount());
    tableWidget->setColumnCount(model->columnCount());

    // Set horizontal headers from model headerData if available
    QStringList headers;
    headers.reserve(model->columnCount());
    for (int c = 0; c < model->columnCount(); ++c) {
        headers << model->headerData(c, Qt::Horizontal).toString();
    }
    tableWidget->setHorizontalHeaderLabels(headers);

    for (int r = 0; r < model->rowCount(); ++r) {
        for (int c = 0; c < model->columnCount(); ++c) {
            const QVariant data = model->data(model->index(r, c), Qt::DisplayRole);
            QTableWidgetItem *item = new QTableWidgetItem(data.toString());
            tableWidget->setItem(r, c, item);
        }
    }
}

EmployeePage::EmployeePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EmployeePage)
{
    ui->setupUi(this);
    refreshTable();
    
    // Connecter le signal cellClicked du QTableWidget pour remplir les champs
    connect(ui->tableView, &QTableWidget::cellClicked, this, [this](int row, int column) {
        Q_UNUSED(column);
        fillFieldsFromTableRow(row);
    });
    
    // Connecter les checkboxes pour qu'ils fonctionnent comme des radio buttons
    connect(ui->radioButton_graphiste, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->radioButton_video->setChecked(false);
            ui->radioButton_influencer->setChecked(false);
            ui->radioButton_technicien->setChecked(false);
            ui->radioButton_autre->setChecked(false);
        }
    });
    connect(ui->radioButton_video, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->radioButton_graphiste->setChecked(false);
            ui->radioButton_influencer->setChecked(false);
            ui->radioButton_technicien->setChecked(false);
            ui->radioButton_autre->setChecked(false);
        }
    });
    connect(ui->radioButton_influencer, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->radioButton_graphiste->setChecked(false);
            ui->radioButton_video->setChecked(false);
            ui->radioButton_technicien->setChecked(false);
            ui->radioButton_autre->setChecked(false);
        }
    });
    connect(ui->radioButton_technicien, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->radioButton_graphiste->setChecked(false);
            ui->radioButton_video->setChecked(false);
            ui->radioButton_influencer->setChecked(false);
            ui->radioButton_autre->setChecked(false);
        }
    });
    connect(ui->radioButton_autre, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked) {
            ui->radioButton_graphiste->setChecked(false);
            ui->radioButton_video->setChecked(false);
            ui->radioButton_influencer->setChecked(false);
            ui->radioButton_technicien->setChecked(false);
        }
    });
}

EmployeePage::~EmployeePage()
{
    delete ui;
}

void EmployeePage::refreshTable()
{
    fillTableWidgetFromModel(ui->tableView, Etmp.afficher());
}

void EmployeePage::fillFieldsFromTableRow(int row)
{
    if (row < 0 || row >= ui->tableView->rowCount()) {
        return; // Ligne invalide
    }
    
    // Remplir les champs avec les données de la ligne
    QTableWidgetItem *itemId = ui->tableView->item(row, 0);
    QTableWidgetItem *itemNom = ui->tableView->item(row, 1);
    QTableWidgetItem *itemTel = ui->tableView->item(row, 2);
    QTableWidgetItem *itemSalaire = ui->tableView->item(row, 3);
    QTableWidgetItem *itemMail = ui->tableView->item(row, 4);
    QTableWidgetItem *itemType = ui->tableView->item(row, 5);
    
    if (itemId) {
        ui->lineEdit_id->setText(itemId->text());
    }
    if (itemNom) {
        ui->lineEdit_nom->setText(itemNom->text());
    }
    if (itemTel) {
        ui->lineEdit_tel->setText(itemTel->text());
    }
    if (itemSalaire) {
        ui->lineEdit_salaire->setText(itemSalaire->text());
    }
    if (itemMail) {
        ui->lineEdit_mail->setText(itemMail->text());
    }
    if (itemType) {
        QString type = itemType->text();
        // Définir le radio button correspondant
        if (type.contains("Graphiste", Qt::CaseInsensitive) || type.contains("Designer", Qt::CaseInsensitive)) {
            ui->radioButton_graphiste->setChecked(true);
        } else if (type.contains("Vidéo", Qt::CaseInsensitive) || type.contains("Video", Qt::CaseInsensitive)) {
            ui->radioButton_video->setChecked(true);
        } else if (type.contains("Influencer", Qt::CaseInsensitive)) {
            ui->radioButton_influencer->setChecked(true);
        } else if (type.contains("Technicien", Qt::CaseInsensitive)) {
            ui->radioButton_technicien->setChecked(true);
        } else {
            ui->radioButton_autre->setChecked(true);
        }
    }
}

void EmployeePage::on_pushButton_ajouter_clicked()
{
    if (ui->lineEdit_id->text().isEmpty() || ui->lineEdit_nom->text().isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquants"), tr("ID et Nom sont obligatoires."));
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    int tel = ui->lineEdit_tel->text().toInt();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString mail = ui->lineEdit_mail->text();
    
    // Validation de la longueur des champs selon les contraintes Oracle
    if (mail.length() > 20) {
        QMessageBox::warning(this, tr("Erreur de validation"), 
            tr("L'email ne peut pas dépasser 20 caractères.\nLongueur actuelle: %1 caractères").arg(mail.length()));
        return;
    }
    if (nom.length() > 50) {
        QMessageBox::warning(this, tr("Erreur de validation"), 
            tr("Le nom ne peut pas dépasser 50 caractères.\nLongueur actuelle: %1 caractères").arg(nom.length()));
        return;
    }

    QString type;
    if (ui->radioButton_graphiste->isChecked())
        type = "Graphiste / Designer";
    else if (ui->radioButton_video->isChecked())
        type = "Vidéo Shoot";
    else if (ui->radioButton_influencer->isChecked())
        type = "Influencer Collab";
    else if (ui->radioButton_technicien->isChecked())
        type = "Technicien Collab";
    else if (ui->radioButton_autre->isChecked())
        type = "Autre";
    else
        type = "Non défini";

    Employe E(id, nom, tel, salaire, mail, type);
    bool test = E.ajouter();

    if (test) {
        QMessageBox::information(this, tr("Ajout"), tr("Employé ajouté avec succès."));
        refreshTable();

        // vider les champs après ajout
        ui->lineEdit_id->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_tel->clear();
        ui->lineEdit_salaire->clear();
        ui->lineEdit_mail->clear();
        ui->radioButton_autre->setChecked(true);
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de l'ajout."));
    }
}

void EmployeePage::on_pushButton_supprimer_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquant"), tr("Veuillez entrer l'ID de l'employé à supprimer."));
        return;
    }

    int id = ui->lineEdit_id->text().toInt();

    auto reply = QMessageBox::question(this, tr("Confirmer"), 
        tr("Voulez-vous supprimer l'employé avec l'ID %1 ?").arg(id),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        bool test = Etmp.supprimer(id);
        if (test) {
            QMessageBox::information(this, tr("Suppression"), tr("Employé supprimé avec succès."));
            refreshTable();
            // Vider les champs après suppression
            ui->lineEdit_id->clear();
            ui->lineEdit_nom->clear();
            ui->lineEdit_tel->clear();
            ui->lineEdit_salaire->clear();
            ui->lineEdit_mail->clear();
            ui->radioButton_autre->setChecked(true);
        } else {
            QMessageBox::critical(this, tr("Erreur"), 
                tr("Échec de suppression.\nVérifiez que l'ID existe et qu'il n'y a pas de contraintes de clé étrangère."));
        }
    }
}

void EmployeePage::on_pushButton_modifier_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquant"), tr("Veuillez entrer l'ID de l'employé à modifier."));
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    int tel = ui->lineEdit_tel->text().toInt();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString mail = ui->lineEdit_mail->text();
    
    // Validation de la longueur des champs selon les contraintes Oracle
    if (mail.length() > 20) {
        QMessageBox::warning(this, tr("Erreur de validation"), 
            tr("L'email ne peut pas dépasser 20 caractères.\nLongueur actuelle: %1 caractères").arg(mail.length()));
        return;
    }
    if (nom.length() > 50) {
        QMessageBox::warning(this, tr("Erreur de validation"), 
            tr("Le nom ne peut pas dépasser 50 caractères.\nLongueur actuelle: %1 caractères").arg(nom.length()));
        return;
    }

    QString type;
    if (ui->radioButton_graphiste->isChecked())
        type = "Graphiste / Designer";
    else if (ui->radioButton_video->isChecked())
        type = "Vidéo Shoot";
    else if (ui->radioButton_influencer->isChecked())
        type = "Influencer Collab";
    else if (ui->radioButton_technicien->isChecked())
        type = "Technicien Collab";
    else if (ui->radioButton_autre->isChecked())
        type = "Autre";
    else
        type = "Non défini";

    // Vérifier que le nom n'est pas vide pour la modification
    if (nom.isEmpty()) {
        QMessageBox::warning(this, tr("Champs manquant"), tr("Le nom est obligatoire pour la modification."));
        return;
    }
    
    Employe E(id, nom, tel, salaire, mail, type);
    bool test = E.modifier();

    if (test) {
        QMessageBox::information(this, tr("Modification"), tr("Employé modifié avec succès."));
        refreshTable();
    } else {
        QMessageBox::critical(this, tr("Erreur"), 
            tr("Échec de modification.\nVérifiez que l'ID existe et que les données sont valides."));
    }
}

void EmployeePage::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return; // Index invalide
    }
    
    // Utiliser l'index pour récupérer la ligne et remplir les champs
    fillFieldsFromTableRow(index.row());
}

void EmployeePage::on_pushButton_113_clicked()
{
    emit backToMenu();
}

