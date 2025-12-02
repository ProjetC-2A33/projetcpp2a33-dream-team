#include "smartstudio.h"
#include "ui_smartstudio.h"
#include "employe.h"
#include "employeepiewidget.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QTableWidget>
#include <QAbstractItemModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSignalBlocker>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPageLayout>
#include <QDateTime>
#include <QDate>
#include <QDir>
#include <memory>

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

void smartstudio::fillFieldsFromTableRow(int row)
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

smartstudio::smartstudio(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::smartstudio)
{
    ui->setupUi(this);
    {
        // Afficher la table employé au démarrage
        std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
        fillTableWidgetFromModel(ui->tableView, model.get());
    }

    {
        // Préparer les options de tri sur le salaire
        const QSignalBlocker blocker(ui->comboBox_10);
        ui->comboBox_10->clear();
        ui->comboBox_10->addItem(tr("Trier par salaire"));
        ui->comboBox_10->addItem(tr("Salaire croissant"));
        ui->comboBox_10->addItem(tr("Salaire décroissant"));
    }
    
    // Connecter le signal cellClicked du QTableWidget pour remplir les champs
    connect(ui->tableView, &QTableWidget::cellClicked, this, [this](int row, int column) {
        Q_UNUSED(column);
        fillFieldsFromTableRow(row);
    });

    m_employeePieWidget = new EmployeePieWidget(ui->stackedWidget->widget(3));
    // Camembert : zone vide sous le tableau, à gauche des boutons
    m_employeePieWidget->setGeometry(480, 420, 260, 200);
    updateEmployeeTypeChart();

    // Calendrier simple dans une zone vide au-dessus du bouton quitter
    m_employeeCalendar = new QCalendarWidget(ui->stackedWidget->widget(3));
    // x, y, largeur, hauteur (widget plus petit et discret)
    m_employeeCalendar->setGeometry(840, 440, 220, 130);
    m_employeeCalendar->setSelectedDate(QDate::currentDate());
}

void smartstudio::updateEmployeeTypeChart()
{
    if (!m_employeePieWidget)
        return;

    std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
    if (!model) {
        m_employeePieWidget->setData({});
        return;
    }

    if (model->rowCount() == 0) {
        m_employeePieWidget->setData({});
        return;
    }

    // Dans ton tableau, la colonne 5 (index 5) correspond à "Type d'employé"
    const int typeIndex = 5;
    if (typeIndex < 0 || typeIndex >= model->columnCount()) {
        m_employeePieWidget->setData({});
        return;
    }

    QMap<QString, int> counters;
    for (int r = 0; r < model->rowCount(); ++r) {
        const QString type = model->data(model->index(r, typeIndex)).toString();
        counters[type] += 1;
    }

    QVector<QPair<QString, int>> data;
    data.reserve(counters.size());
    for (auto it = counters.cbegin(); it != counters.cend(); ++it) {
        data.append(qMakePair(it.key(), it.value()));
    }

    m_employeePieWidget->setData(data);
}

void smartstudio::on_pushButton_ajouter_clicked()
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
    if (nom.length() > 50) { // Vérification pour éviter d'autres erreurs similaires
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
        std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
        fillTableWidgetFromModel(ui->tableView, model.get());
        updateEmployeeTypeChart();

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

void smartstudio::on_pushButton_supprimer_clicked()
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
            std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
            fillTableWidgetFromModel(ui->tableView, model.get());
            // Vider les champs après suppression
            ui->lineEdit_id->clear();
            ui->lineEdit_nom->clear();
            ui->lineEdit_tel->clear();
            ui->lineEdit_salaire->clear();
            ui->lineEdit_mail->clear();
            ui->radioButton_autre->setChecked(true);
            updateEmployeeTypeChart();
        } else {
            QMessageBox::critical(this, tr("Erreur"), 
                tr("Échec de suppression.\nVérifiez que l'ID existe et qu'il n'y a pas de contraintes de clé étrangère."));
        }
    }
}

void smartstudio::on_pushButton_modifier_clicked()
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
    if (nom.length() > 50) { // Vérification pour éviter d'autres erreurs similaires
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
        std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
        fillTableWidgetFromModel(ui->tableView, model.get());
        updateEmployeeTypeChart();
    } else {
        QMessageBox::critical(this, tr("Erreur"), 
            tr("Échec de modification.\nVérifiez que l'ID existe et que les données sont valides."));
    }
}

smartstudio::~smartstudio()
{
    delete ui;
}

void smartstudio::on_pushButton_82_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void smartstudio::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void smartstudio::on_pushButton_84_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void smartstudio::on_pushButton_99_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void smartstudio::on_pushButton_85_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void smartstudio::on_pushButton_98_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void smartstudio::on_pushButton_86_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    updateEmployeeTypeChart();
}

void smartstudio::on_pushButton_113_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void smartstudio::on_pushButton_101_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void smartstudio::on_pushButton_62_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void smartstudio::on_pushButton_80_clicked()
{
    const QString email = ui->l2_25->text().trimmed();
    const QString password = ui->lineEdit_46->text();

    const QString validEmail = QStringLiteral("boujmilmohamed3@gmail.com");
    const QString validPassword = QStringLiteral("12345678");

    if (email == validEmail && password == validPassword) {
        ui->stackedWidget->setCurrentIndex(1); // passer au menu
    } else {
        QMessageBox::warning(this,
                             tr("Authentification"),
                             tr("E-mail ou mot de passe incorrect."));
    }
}

void smartstudio::on_pushButton_81_clicked()
{
    ui->l2_25->clear();      // champ E-mail
    ui->lineEdit_46->clear(); // champ Mot de passe
}

void smartstudio::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return; // Index invalide
    }
    
    // Utiliser l'index pour récupérer la ligne et remplir les champs
    fillFieldsFromTableRow(index.row());
}

void smartstudio::on_comboBox_10_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }

    if (index == 0) {
        std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
        fillTableWidgetFromModel(ui->tableView, model.get());
        return;
    }

    const bool ascending = (index == 1);
    std::unique_ptr<QSqlQueryModel> model(Etmp.trierParSalaire(ascending));
    if (!model) {
        QMessageBox::warning(this, tr("Tri impossible"), tr("Le tri par salaire n'a pas pu être effectué."));
        return;
    }

    fillTableWidgetFromModel(ui->tableView, model.get());
}

void smartstudio::on_l2_59_editingFinished()
{
    const QString text = ui->l2_59->text().trimmed();
    if (text.isEmpty()) {
        std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
        fillTableWidgetFromModel(ui->tableView, model.get());
        return;
    }

    bool ok = false;
    const int id = text.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("ID invalide"), tr("Veuillez entrer un ID numérique."));
        ui->l2_59->setFocus();
        return;
    }

    std::unique_ptr<QSqlQueryModel> model(Etmp.rechercherParId(id));
    if (!model) {
        QMessageBox::warning(this, tr("Recherche impossible"), tr("La recherche par ID a échoué."));
        return;
    }

    if (model->rowCount() == 0) {
        QMessageBox::information(this, tr("Aucun résultat"), tr("Aucun employé trouvé avec l'ID %1.").arg(id));
    }

    fillTableWidgetFromModel(ui->tableView, model.get());
}

void smartstudio::on_pushButton_114_clicked()
{
    if (ui->tableView->rowCount() == 0) {
        QMessageBox::information(this, tr("Export PDF"), tr("Aucun employé à exporter."));
        return;
    }

    const QString suggestedName =
        QDir::homePath() + QString("/Employes_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Exporter les employés en PDF"),
        suggestedName,
        tr("Fichiers PDF (*.pdf)"));

    if (fileName.isEmpty()) {
        return;
    }
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    QString html = "<h1 style='text-align:center;'>Liste des employés</h1>";
    html += QString("<p>%1</p>").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'><thead><tr>";

    for (int c = 0; c < ui->tableView->columnCount(); ++c) {
        QString header;
        if (auto *headerItem = ui->tableView->horizontalHeaderItem(c)) {
            header = headerItem->text();
        } else {
            header = ui->tableView->model()->headerData(c, Qt::Horizontal).toString();
        }
        html += QString("<th>%1</th>").arg(header.toHtmlEscaped());
    }
    html += "</tr></thead><tbody>";

    for (int r = 0; r < ui->tableView->rowCount(); ++r) {
        html += "<tr>";
        for (int c = 0; c < ui->tableView->columnCount(); ++c) {
            QTableWidgetItem *item = ui->tableView->item(r, c);
            const QString cellText = item ? item->text() : QString();
            html += QString("<td>%1</td>").arg(cellText.toHtmlEscaped());
        }
        html += "</tr>";
    }
    html += "</tbody></table>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);

    document.print(&printer);

    QMessageBox::information(this, tr("Export PDF"), tr("Export terminé : %1").arg(QDir::toNativeSeparators(fileName)));
}

void smartstudio::on_pushButton_2_clicked()
{
    {
        const QSignalBlocker blocker(ui->comboBox_10);
        ui->comboBox_10->setCurrentIndex(0);
    }
    ui->l2_59->clear();

    std::unique_ptr<QSqlQueryModel> model(Etmp.afficher());
    fillTableWidgetFromModel(ui->tableView, model.get());
}
