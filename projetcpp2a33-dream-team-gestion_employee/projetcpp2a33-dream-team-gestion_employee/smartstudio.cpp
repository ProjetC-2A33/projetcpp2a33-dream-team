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
    ,TCP_Server(new QTcpServer(this)),
    networkManager(new QNetworkAccessManager(this))
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
    ui->comboBox_type->addItems({"Financier", "En nature", "Co-branding"});
    ui->comboBox_statut->addItems({"Actif", "Inactif"});


    // Connexions des boutons
    connect(ui->pushButton_ajouter,   &QPushButton::clicked,
            this, &smartstudio::ajouterSponsor);
    connect(ui->pushButton_modifier,  &QPushButton::clicked,
            this, &smartstudio::modifierSponsor);
    connect(ui->pushButton_supprimer, &QPushButton::clicked,
            this, &smartstudio::supprimerSponsor);
    connect(ui->pushButton_pdf,       &QPushButton::clicked,
            this, &smartstudio::exporterPdf);

    // Tri + recherche
    connect(ui->comboBox_tri, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &smartstudio::appliquerTri);
    connect(ui->lineEdit_chercher, &QLineEdit::textChanged,
            this, &smartstudio::appliquerRecherche);

    // Lorsqu'on clique une ligne, remplir le formulaire
    connect(ui->tableWidget_sponsors, &QTableWidget::cellClicked,
            this, [=](int row, int){
                if (row >= 0 && row < sponsors.size()) {
                    const Sponsor &s = sponsors[row];
                    ui->lineEdit_nom->setText(s.getNom());
                    ui->lineEdit_secteur->setText(s.getSecteur());
                    ui->lineEdit_budget->setText(QString::number(s.getBudget()));
                    ui->lineEdit_contrat->setText(s.getContrat());
                    ui->comboBox_type->setCurrentText(s.getTypePartenariat());
                    ui->comboBox_statut->setCurrentText(s.getStatut());
                    ui->lineEdit_email->setText(s.getEmail());
                    ui->lineEdit_tel->setText(s.getTelephone());
                    ui->dateEdit_debut->setDate(QDate::fromString(s.getDateDebutContrat(), "yyyy-MM-dd"));
                    ui->dateEdit_fin->setDate(QDate::fromString(s.getDateFinContrat(), "yyyy-MM-dd"));
                }
            });

    connect(ui->stat, &QPushButton::clicked,
            this, &smartstudio::afficherStatistiquesCirculaire);

    chargerSponsors();
    TCP_Server = new QTcpServer();
    if(TCP_Server->listen(QHostAddress::Any, 8888)) {
           connect(TCP_Server, &QTcpServer::newConnection, this, &smartstudio::newConnection);
           QMessageBox::information(this,"Qt","Server Started on port 8888");
       } else {
           QMessageBox::critical(this,"Qt","Failed to start server on port 8888");
       }
    connect(ui->pushButton_envoyerSMS, &QPushButton::clicked,
               this, &smartstudio::on_pushButton_envoyerSMS_clicked);

       // Connect network manager for SMS replies
       connect(networkManager, &QNetworkAccessManager::finished,
               this, &smartstudio::onSMSReply);
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

void smartstudio::on_pushButton_83_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}
#include "smartstudio.h"
#include "ui_smartstudio.h"
#include "sponsor.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QDate>
#include <QRegularExpression>
#include <QListView>
#include <QStringListModel>
#include <QChartView>
#include <QPieSeries>
#include <QChart>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QDate>
#include <QRegularExpression>
#include <QListView>
#include <QStringListModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QInputDialog>
#include <QBuffer>
#include <QMessageBox>
bool smartstudio::controleSaisie()
{
    QString nom     = ui->lineEdit_nom->text().trimmed();
    QString secteur = ui->lineEdit_secteur->text().trimmed();
    QString budgetS = ui->lineEdit_budget->text().trimmed();
    QString contrat = ui->lineEdit_contrat->text().trimmed();
    QString email   = ui->lineEdit_email->text().trimmed();
    QString tel     = ui->lineEdit_tel->text().trimmed();

    if (nom.isEmpty() || secteur.isEmpty() || budgetS.isEmpty() ||
        contrat.isEmpty() || email.isEmpty() || tel.isEmpty()) {
        QMessageBox::warning(this, "Champs vides",
                             "Veuillez remplir tous les champs obligatoires.");
        return false;
    }

    bool ok = false;
    double budget = budgetS.toDouble(&ok);
    if (!ok || budget <= 0) {
        QMessageBox::warning(this, "Budget incorrect",
                             "Le budget doit être un nombre positif.");
        return false;
    }

    // ---------- Email ----------
    QRegularExpression mailRegex(R"(^[\w\.]+@[\w\.]+\.[a-zA-Z]{2,4}$)");
    if (!mailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Email invalide",
                             "Veuillez saisir un email valide.");
        return false;
    }

    // ---------- Téléphone ----------
    QRegularExpression telRegex(R"(^\d{8}$)");
    if (!telRegex.match(tel).hasMatch()) {
        QMessageBox::warning(this, "Téléphone invalide",
                             "Le numéro doit contenir  8  chiffres.");
        return false;
    }

    // ---------- Dates ----------
    if (ui->dateEdit_fin->date() < ui->dateEdit_debut->date()) {
        QMessageBox::warning(this, "Dates invalides",
                             "La date de fin ne peut pas être avant la date de début.");
        return false;
    }

    return true;
}

// =================== Charger sponsors ===================

void smartstudio::chargerSponsors()
{
    sponsors = Sponsor::afficher("", "");

    ui->tableWidget_sponsors->setRowCount(0);
    ui->tableWidget_sponsors->setColumnCount(10);

    QStringList titres = {"Nom", "Secteur", "Budget", "Contrat", "Type", "Statut",
                          "Email", "Téléphone", "Date début", "Date fin"};
    ui->tableWidget_sponsors->setHorizontalHeaderLabels(titres);

    for (int i = 0; i < sponsors.size(); ++i) {
        ui->tableWidget_sponsors->insertRow(i);
        ui->tableWidget_sponsors->setItem(i, 0, new QTableWidgetItem(sponsors[i].getNom()));
        ui->tableWidget_sponsors->setItem(i, 1, new QTableWidgetItem(sponsors[i].getSecteur()));
        ui->tableWidget_sponsors->setItem(i, 2, new QTableWidgetItem(QString::number(sponsors[i].getBudget())));
        ui->tableWidget_sponsors->setItem(i, 3, new QTableWidgetItem(sponsors[i].getContrat()));
        ui->tableWidget_sponsors->setItem(i, 4, new QTableWidgetItem(sponsors[i].getTypePartenariat()));
        ui->tableWidget_sponsors->setItem(i, 5, new QTableWidgetItem(sponsors[i].getStatut()));
        ui->tableWidget_sponsors->setItem(i, 6, new QTableWidgetItem(sponsors[i].getEmail()));
        ui->tableWidget_sponsors->setItem(i, 7, new QTableWidgetItem(sponsors[i].getTelephone()));
        ui->tableWidget_sponsors->setItem(i, 8, new QTableWidgetItem(sponsors[i].getDateDebutContrat()));
        ui->tableWidget_sponsors->setItem(i, 9, new QTableWidgetItem(sponsors[i].getDateFinContrat()));
    }

    ui->tableWidget_sponsors->resizeColumnsToContents();
}

// =================== Ajouter ===================

void smartstudio::ajouterSponsor()
{
    if (!controleSaisie())
        return;

    Sponsor s(0,
              ui->lineEdit_nom->text(),
              ui->lineEdit_secteur->text(),
              ui->lineEdit_budget->text().toDouble(),
              ui->lineEdit_contrat->text(),
              ui->comboBox_type->currentText(),
              ui->comboBox_statut->currentText(),
              ui->lineEdit_email->text(),
              ui->lineEdit_tel->text(),
              ui->dateEdit_debut->date().toString("yyyy-MM-dd"),
              ui->dateEdit_fin->date().toString("yyyy-MM-dd"));

    if (s.ajouter()) {
        QMessageBox::information(this, "Ajout", "Sponsor ajouté avec succès.");
        chargerSponsors();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

// =================== Modifier ===================

void smartstudio::modifierSponsor()
{
    int row = ui->tableWidget_sponsors->currentRow();
    if (row < 0 || row >= sponsors.size()) {
        QMessageBox::warning(this, "Sélection", "Sélectionnez un sponsor à modifier.");
        return;
    }

    if (!controleSaisie())
        return;

    Sponsor s = sponsors[row]; // copie avec id
    s.setNom(ui->lineEdit_nom->text());
    s.setSecteur(ui->lineEdit_secteur->text());
    s.setBudget(ui->lineEdit_budget->text().toDouble());
    s.setContrat(ui->lineEdit_contrat->text());
    s.setTypePartenariat(ui->comboBox_type->currentText());
    s.setStatut(ui->comboBox_statut->currentText());
    s.setEmail(ui->lineEdit_email->text());
    s.setTelephone(ui->lineEdit_tel->text());
    s.setDateDebutContrat(ui->dateEdit_debut->date().toString("yyyy-MM-dd"));
    s.setDateFinContrat(ui->dateEdit_fin->date().toString("yyyy-MM-dd"));

    if (s.modifier()) {
        QMessageBox::information(this, "Modification", "Sponsor modifié avec succès.");
        chargerSponsors();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

// =================== Supprimer ===================

void smartstudio::supprimerSponsor()
{
    int row = ui->tableWidget_sponsors->currentRow();
    if (row < 0 || row >= sponsors.size()) {
        QMessageBox::warning(this, "Sélection", "Sélectionnez un sponsor à supprimer.");
        return;
    }

    if (QMessageBox::question(this, "Suppression",
                              "Voulez-vous vraiment supprimer ce sponsor ?")
        != QMessageBox::Yes)
        return;

    Sponsor s = sponsors[row];

    if (s.supprimer()) {
        QMessageBox::information(this, "Suppression", "Sponsor supprimé.");
        chargerSponsors();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}

// =================== Tri / Recherche ===================

void smartstudio::appliquerTri()
{
    QString critere = ui->comboBox_tri->currentText();


    QString baseCritere = critere;
    baseCritere = baseCritere.replace(" (▲)", "").replace(" (▼)", "");

    if (baseCritere == "Nom") {
        static bool nomAscending = true;

        if (nomAscending) {
            sponsors = Sponsor::trierParNom(true);
            ui->comboBox_tri->setItemText(ui->comboBox_tri->currentIndex(), "Nom (▲)");
            nomAscending = false;
        } else {
            sponsors = Sponsor::trierParNom(false);
            ui->comboBox_tri->setItemText(ui->comboBox_tri->currentIndex(), "Nom (▼)");
            nomAscending = true;
        }
    }
    else if (baseCritere == "Budget") {
        static bool budgetAscending = true;

        if (budgetAscending) {
            sponsors = Sponsor::trierParBudget(true);
            ui->comboBox_tri->setItemText(ui->comboBox_tri->currentIndex(), "Budget (▲)");
            budgetAscending = false;
        } else {
            sponsors = Sponsor::trierParBudget(false);
            ui->comboBox_tri->setItemText(ui->comboBox_tri->currentIndex(), "Budget (▼)");
            budgetAscending = true;
        }
    }
    else if (baseCritere == "Aucun tri") {
        ui->comboBox_tri->setItemText(1, "Nom");
        ui->comboBox_tri->setItemText(2, "Budget");

        chargerSponsors();
        return;
    }
    else {

        return;
    }

    afficherSponsors();
}
void smartstudio::afficherSponsors()
{
    ui->tableWidget_sponsors->clear();
    ui->tableWidget_sponsors->setRowCount(0);

    QStringList headers;
    headers << "ID" << "Nom" << "Secteur" << "Budget" << "Contrat"
            << "Type" << "Statut" << "Email" << "Téléphone"
            << "Date Début" << "Date Fin";
    ui->tableWidget_sponsors->setColumnCount(headers.size());
    ui->tableWidget_sponsors->setHorizontalHeaderLabels(headers);

    int row = 0;
    for (const Sponsor &s : sponsors) {
        ui->tableWidget_sponsors->insertRow(row);

        ui->tableWidget_sponsors->setItem(row, 0, new QTableWidgetItem(QString::number(s.getIdSponsor())));
        ui->tableWidget_sponsors->setItem(row, 1, new QTableWidgetItem(s.getNom()));
        ui->tableWidget_sponsors->setItem(row, 2, new QTableWidgetItem(s.getSecteur()));
        ui->tableWidget_sponsors->setItem(row, 3, new QTableWidgetItem(QString::number(s.getBudget())));
        ui->tableWidget_sponsors->setItem(row, 4, new QTableWidgetItem(s.getContrat()));
        ui->tableWidget_sponsors->setItem(row, 5, new QTableWidgetItem(s.getTypePartenariat()));
        ui->tableWidget_sponsors->setItem(row, 6, new QTableWidgetItem(s.getStatut()));
        ui->tableWidget_sponsors->setItem(row, 7, new QTableWidgetItem(s.getEmail()));
        ui->tableWidget_sponsors->setItem(row, 8, new QTableWidgetItem(s.getTelephone()));
        ui->tableWidget_sponsors->setItem(row, 9, new QTableWidgetItem(s.getDateDebutContrat()));
        ui->tableWidget_sponsors->setItem(row, 10, new QTableWidgetItem(s.getDateFinContrat()));

        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = ui->tableWidget_sponsors->item(row, col);
            if (item) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }

        row++;
    }


    ui->tableWidget_sponsors->resizeColumnsToContents();
}
void smartstudio::appliquerRecherche(const QString &texte)
{
    QString currentTri = ui->comboBox_tri->currentText();
    QString baseCritere = currentTri.replace(" (▲)", "").replace(" (▼)", "");

    if (baseCritere == "Nom") {
        bool isAscending = !currentTri.contains("(▼)");
        sponsors = Sponsor::trierParNom(isAscending);
    }
    else if (baseCritere == "Budget") {
        bool isAscending = !currentTri.contains("(▼)");
        sponsors = Sponsor::trierParBudget(isAscending);
    }
    else {
        sponsors = Sponsor::afficher();
    }

    if (!texte.isEmpty()) {
        QList<Sponsor> filtered;
        QString searchText = texte.toLower();
        for (const Sponsor &s : sponsors) {
            if (s.getNom().toLower().contains(searchText) ||
                s.getSecteur().toLower().contains(searchText) ||
                s.getTelephone().contains(searchText)) {
                filtered.append(s);
            }
        }
        sponsors = filtered;
    }

    afficherSponsors();
}
void smartstudio::exporterPdf()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter les sponsors en PDF",
        "sponsors.pdf",
        "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty())
        return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur PDF",
                              "Impossible de créer le fichier PDF.");
        return;
    }

    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);
    QSizeF tableSize = ui->tableWidget_sponsors->size();

    double xScale = pageRect.width()  / tableSize.width();
    double yScale = pageRect.height() / tableSize.height();
    double scale  = qMin(xScale, yScale);
    scale *= 0.80;

    double tableDrawWidth  = tableSize.width()  * scale;
    double tableDrawHeight = tableSize.height() * scale;

    double offsetX = pageRect.x() + (pageRect.width()  - tableDrawWidth) / 2.0;
    double offsetY = pageRect.y() + 40;


    painter.translate(offsetX, offsetY);
    painter.scale(scale, scale);

    ui->tableWidget_sponsors->render(&painter);

    painter.end();

    QMessageBox::information(this, "Export PDF",
                             "Le tableau des sponsors a été exporté en PDF.");
}


void smartstudio::afficherStatistiquesCirculaire()
{
    QMap<QString, int> statistiques = Sponsor::getStatistiquesPartenariat();


    QLayout *layout = ui->liststat->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    } else {
        layout = new QVBoxLayout(ui->liststat);
    }


    int total = 0;
    for (int count : statistiques) {
        total += count;
    }

    if (total == 0) {
        QLabel *label = new QLabel("Aucun sponsor dans la base de données");
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);
        return;
    }

    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

    QVector<QColor> colors = {
        QColor("#FF6384"), QColor("#36A2EB"), QColor("#FFCE56"),
        QColor("#4BC0C0"), QColor("#9966FF"), QColor("#FF9F40")
    };

    int colorIndex = 0;

    for (auto it = statistiques.begin(); it != statistiques.end(); ++it) {
        QString type = it.key();
        int count = it.value();
        double pourcentage = (count * 100.0) / total;

        QtCharts::QPieSlice *slice = series->append(
            QString("%1\n%2 sponsors (%3%)")
                .arg(type)
                .arg(count)
                .arg(QString::number(pourcentage, 'f', 1)),
            count
            );

        slice->setColor(colors[colorIndex % colors.size()]);
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);
        slice->setLabelPosition(QtCharts::QPieSlice::LabelOutside);
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);

        colorIndex++;
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des sponsors par type de partenariat");
    chart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QtCharts::QChart::AllAnimations);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    layout->addWidget(chartView);

    QLabel *totalLabel = new QLabel(QString("Total : %1 sponsors").arg(total));
    totalLabel->setAlignment(Qt::AlignCenter);
    totalLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    layout->addWidget(totalLabel);
}
void smartstudio::newConnection()
{
 while (TCP_Server->hasPendingConnections())
{
     Add_New_Client_Connection(TCP_Server->nextPendingConnection());
 }
}

void smartstudio::Read_Data_From_Socket()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qDebug() << "Error: Could not cast sender to QTcpSocket";
        return;
    }

    if (socket->bytesAvailable() > 0) {
        QByteArray Message_From_Server = socket->readAll();
        if (Message_From_Server.isEmpty()) {
            qDebug() << "Error: Empty message received from client";
            return;
        }

        QString Message = "Client : " + QString::number(socket->socketDescriptor()) + " :: " + QString(Message_From_Server);
        ui->textEdit_Client_Messages->append(Message);
    } else {
        qDebug() << "No data available to read from socket";
    }

    if (socket->error() != QAbstractSocket::UnknownSocketError) {
        qDebug() << "Socket error: " << socket->errorString();
    }
}

void smartstudio::Add_New_Client_Connection(QTcpSocket *socket)
{
    Client_Connection_List.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read_Data_From_Socket()));

    ui->comboBox_Client_list->addItem(QString::number(socket->socketDescriptor()));
     QString Client = "Client : " + QString::number(socket->socketDescriptor()) + " Connected With The Server.";
    ui->textEdit_Client_Messages->append(Client);
}


void smartstudio::on_pushButton_Send_Message_Type_clicked()
{
    QString Message_For_Client = ui->lineEdit_Message_For_Client->text();
    QString Receiver = ui->comboBox_Client_list->currentText();
    if (ui->comboBox_Send_Message_Type->currentText() == "All")
    {
foreach(QTcpSocket *socket, Client_Connection_List)
{
    socket->write(Message_For_Client.toStdString().c_str());
}
    }
    else
         {
        foreach(QTcpSocket *socket, Client_Connection_List)
        {
            if (socket->socketDescriptor() == Receiver.toLongLong())
            {
                socket->write(Message_For_Client.toStdString().c_str());
            }
        }

    }
}
void smartstudio::envoyerSMS(const QString& telephone, const QString& message)
{

    QString formattedPhone = telephone;


    formattedPhone = formattedPhone.remove(" ").remove("-");


    if (!formattedPhone.startsWith("+")) {
        if (formattedPhone.startsWith("2")) {
            formattedPhone = "+216" + formattedPhone; // +2162xxxxxxx
        } else if (formattedPhone.length() == 8) {
            formattedPhone = "+216" + formattedPhone; // +216xxxxxxx
        }
    }


    QUrl url(infobipBaseUrl + "/sms/2/text/advanced");


    QJsonObject destinationObject;
    destinationObject["to"] = formattedPhone;

    QJsonArray destinationsArray;
    destinationsArray.append(destinationObject);

    QJsonObject messageObject;

    messageObject["from"] = "InfoSMS"; // Use this for free trial
    messageObject["destinations"] = destinationsArray; // Must be "destinations" array
    messageObject["text"] = message;

    QJsonArray messagesArray;
    messagesArray.append(messageObject);

    QJsonObject jsonPayload;
    jsonPayload["messages"] = messagesArray;

    QJsonDocument jsonDoc(jsonPayload);
    QByteArray postData = jsonDoc.toJson();

    qDebug() << "Sending JSON:" << jsonDoc.toJson(QJsonDocument::Indented);


    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("App " + infobipApiKey).toUtf8());

    networkManager->post(request, postData);

    QMessageBox::information(this, "SMS", "Envoi du SMS en cours...");
}
void smartstudio::onSMSReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains("messages")) {
            QJsonArray messages = jsonObj["messages"].toArray();
            if (!messages.isEmpty()) {
                QJsonObject firstMessage = messages[0].toObject();
                QJsonObject statusObj = firstMessage["status"].toObject();
                QString status = statusObj["name"].toString();
                QString messageId = firstMessage["messageId"].toString();

                QMessageBox::information(this, "SMS Réussi",
                                        QString("SMS envoyé avec succès!\nStatus: %1\nID: %2")
                                        .arg(status).arg(messageId));
            } else {
                QMessageBox::information(this, "SMS Réussi", "SMS envoyé avec succès!");
            }
        } else {
            QMessageBox::information(this, "SMS Réussi", "SMS envoyé avec succès!");
        }
    } else {
        QString errorDetails = reply->errorString();
        QByteArray response = reply->readAll();

        if (!response.isEmpty()) {
            QJsonDocument errorDoc = QJsonDocument::fromJson(response);
            if (!errorDoc.isNull()) {
                QJsonObject errorObj = errorDoc.object();
                if (errorObj.contains("requestError")) {
                    QJsonObject serviceException = errorObj["requestError"].toObject()["serviceException"].toObject();
                    errorDetails += "\n" + serviceException["text"].toString();
                }
            }
        }

        QMessageBox::critical(this, "Erreur SMS",
                             QString("Erreur lors de l'envoi du SMS: %1")
                             .arg(errorDetails));
    }

    reply->deleteLater();
}

void smartstudio::on_pushButton_envoyerSMS_clicked()
{
    QString telephone = ui->lineEdit_tel->text().trimmed();
    QString nom = ui->lineEdit_nom->text().trimmed();

    if (telephone.isEmpty()) {
        QMessageBox::warning(this, "SMS", "Veuillez sélectionner un sponsor avec un numéro de téléphone.");
        return;
    }

    if (telephone.length() < 8) {
        QMessageBox::warning(this, "SMS", "Numéro de téléphone invalide.");
        return;
    }

    QString defaultMessage = QString("Cher %1, merci pour votre partenariat! Votre soutien est précieux pour notre organisation.")
                            .arg(nom.isEmpty() ? "partenaire" : nom);

    bool ok;
    QString customMessage = QInputDialog::getMultiLineText(this,
                                                          "Message SMS",
                                                          "Entrez votre message:",
                                                          defaultMessage,
                                                          &ok);

    if (ok && !customMessage.isEmpty()) {
        if (customMessage.length() > 160) {
            QMessageBox::warning(this, "SMS", "Le message est trop long (max 160 caractères).");
            return;
        }
        envoyerSMS(telephone, customMessage);
    }
}

#include <QProcess>

void smartstudio::on_Openhercules_clicked()
{
QString herculesPath = "C:/Users/MSI/Downloads/hercules_3-2-8.exe";

if(QFile::exists(herculesPath)) {
    QProcess *process = new QProcess(this);
    process->start(herculesPath);
} else {
    QMessageBox::warning(this, "File Not Found",
                       "Hercules executable not found at:\n" + herculesPath);
}
}

void smartstudio::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
