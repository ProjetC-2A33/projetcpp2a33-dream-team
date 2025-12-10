#include "pagesponsor.h"
#include "ui_pagesponsor.h"
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
#include <QSqlQueryModel>
#include <QDesktopServices>

pagesponsor::pagesponsor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pagesponsor),
      TCP_Server(new QTcpServer(this)),
      networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    // Combos
    ui->comboBox_type->addItems({"Financier", "En nature", "Co-branding"});
    ui->comboBox_statut->addItems({"Actif", "Inactif"});


    // Connexions des boutons
    connect(ui->pushButton_ajouter,   &QPushButton::clicked,
            this, &pagesponsor::ajouterSponsor);
    connect(ui->pushButton_modifier,  &QPushButton::clicked,
            this, &pagesponsor::modifierSponsor);
    connect(ui->pushButton_supprimer, &QPushButton::clicked,
            this, &pagesponsor::supprimerSponsor);
    connect(ui->pushButton_pdf,       &QPushButton::clicked,
            this, &pagesponsor::exporterPdf);

    // Tri + recherche
    connect(ui->comboBox_tri, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &pagesponsor::appliquerTri);
    connect(ui->lineEdit_chercher, &QLineEdit::textChanged,
            this, &pagesponsor::appliquerRecherche);

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
            this, &pagesponsor::afficherStatistiquesCirculaire);

    chargerSponsors();
    TCP_Server = new QTcpServer();
    if(TCP_Server->listen(QHostAddress::Any, 8888)) {
           connect(TCP_Server, &QTcpServer::newConnection, this, &pagesponsor::newConnection);
           QMessageBox::information(this,"Qt","Server Started on port 8888");
       } else {
           QMessageBox::critical(this,"Qt","Failed to start server on port 8888");
       }
    connect(ui->pushButton_envoyerSMS, &QPushButton::clicked,
               this, &pagesponsor::on_pushButton_envoyerSMS_clicked);

       // Connect network manager for SMS replies
       connect(networkManager, &QNetworkAccessManager::finished,
               this, &pagesponsor::onSMSReply);



}

pagesponsor::~pagesponsor()
{

    delete ui;
}

// =================== Contrôle de saisie ===================

bool pagesponsor::controleSaisie()
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



    // ---------- Dates ----------
    if (ui->dateEdit_fin->date() < ui->dateEdit_debut->date()) {
        QMessageBox::warning(this, "Dates invalides",
                             "La date de fin ne peut pas être avant la date de début.");
        return false;
    }

    return true;
}

// =================== Charger sponsors ===================

void pagesponsor::chargerSponsors()
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

void pagesponsor::ajouterSponsor()
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
        chargerSponsors();           // Refresh the table
        afficherStatistiquesCirculaire(); // Refresh the pie chart
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

// =================== Modifier ===================

void pagesponsor::modifierSponsor()
{
    int row = ui->tableWidget_sponsors->currentRow();
    if (row < 0 || row >= sponsors.size()) {
        QMessageBox::warning(this, "Sélection", "Sélectionnez un sponsor à modifier.");
        return;
    }

    if (!controleSaisie())
        return;

    // Get the original sponsor before modification for comparison
    Sponsor originalSponsor = sponsors[row];

    Sponsor modifiedSponsor = sponsors[row]; // copy with id
    modifiedSponsor.setNom(ui->lineEdit_nom->text());
    modifiedSponsor.setSecteur(ui->lineEdit_secteur->text());
    modifiedSponsor.setBudget(ui->lineEdit_budget->text().toDouble());
    modifiedSponsor.setContrat(ui->lineEdit_contrat->text());
    modifiedSponsor.setTypePartenariat(ui->comboBox_type->currentText());
    modifiedSponsor.setStatut(ui->comboBox_statut->currentText());
    modifiedSponsor.setEmail(ui->lineEdit_email->text());
    modifiedSponsor.setTelephone(ui->lineEdit_tel->text());
    modifiedSponsor.setDateDebutContrat(ui->dateEdit_debut->date().toString("yyyy-MM-dd"));
    modifiedSponsor.setDateFinContrat(ui->dateEdit_fin->date().toString("yyyy-MM-dd"));

    if (modifiedSponsor.modifier()) {
        QMessageBox::information(this, "Modification", "Sponsor modifié avec succès.");
        chargerSponsors();           // Refresh the table
        afficherStatistiquesCirculaire(); // Refresh the pie chart

        // AUTOMATIC SMS SENDING ON MODIFICATION
        QString phoneNumber = ui->lineEdit_tel->text().trimmed(); // Use current phone from UI
        QString sponsorName = ui->lineEdit_nom->text().trimmed();

        // Validate phone number
        if (isValidPhoneNumber(phoneNumber)) {

            // Check if there are significant changes worth notifying
            bool significantChange = false;
            QStringList changes;

            if (originalSponsor.getNom() != modifiedSponsor.getNom())
                changes << "Nom: " + originalSponsor.getNom() + " → " + modifiedSponsor.getNom();
            if (originalSponsor.getBudget() != modifiedSponsor.getBudget())
                changes << "Budget: " + QString::number(originalSponsor.getBudget()) + " → " + QString::number(modifiedSponsor.getBudget());
            if (originalSponsor.getTypePartenariat() != modifiedSponsor.getTypePartenariat())
                changes << "Type: " + originalSponsor.getTypePartenariat() + " → " + modifiedSponsor.getTypePartenariat();
            if (originalSponsor.getStatut() != modifiedSponsor.getStatut())
                changes << "Statut: " + originalSponsor.getStatut() + " → " + modifiedSponsor.getStatut();

            significantChange = !changes.isEmpty();

            if (significantChange) {
                // Create SMS message
                QString smsMessage = QString("Cher %1,\n\nVos informations de sponsor ont été mises à jour:\n")
                                     .arg(sponsorName.isEmpty() ? "partenaire" : sponsorName);

                for (const QString &change : changes) {
                    smsMessage += "• " + change + "\n";
                }

                smsMessage += "\nMerci pour votre partenariat continu!\n\nCordialement,\nVotre équipe de gestion";

                // Send SMS (asynchronous - won't block UI)
                sendAutomaticSMS(phoneNumber, smsMessage);
            } else {
                qDebug() << "Aucun changement significatif - SMS non envoyé";
            }
        } else {
            qDebug() << "Numéro de téléphone invalide pour l'envoi SMS:" << phoneNumber;
        }

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}
bool pagesponsor::isValidPhoneNumber(const QString &phone)
{
    if (phone.isEmpty()) return false;

    // Remove spaces and dashes
    QString cleanPhone = phone;
    cleanPhone = cleanPhone.remove(" ").remove("-").remove(".").remove("(").remove(")");

    // Check if it's a Tunisian number
    if (cleanPhone.startsWith("+216")) {
        return cleanPhone.length() == 13; // +216 + 8 digits
    } else if (cleanPhone.startsWith("216")) {
        return cleanPhone.length() == 11; // 216 + 8 digits
    } else if (cleanPhone.startsWith("00216")) {
        return cleanPhone.length() == 14; // 00216 + 8 digits
    } else if (cleanPhone.startsWith("2")) {
        return cleanPhone.length() == 8; // 8-digit Tunisian number starting with 2
    } else if (cleanPhone.length() == 8) {
        return true; // Any 8-digit number
    }

    return false;
}
void pagesponsor::sendAutomaticSMS(const QString &phoneNumber, const QString &message)
{
    // Format phone number for SMS API
    QString formattedPhone = phoneNumber;
    formattedPhone = formattedPhone.remove(" ").remove("-");

    if (!formattedPhone.startsWith("+")) {
        if (formattedPhone.startsWith("2")) {
            formattedPhone = "+216" + formattedPhone;
        } else if (formattedPhone.length() == 8) {
            formattedPhone = "+216" + formattedPhone;
        } else if (formattedPhone.startsWith("216")) {
            formattedPhone = "+" + formattedPhone;
        } else if (formattedPhone.startsWith("00216")) {
            formattedPhone = "+" + formattedPhone.mid(2);
        }
    }

    // Log the SMS being sent
    qDebug() << "Envoi SMS automatique à:" << formattedPhone;
    qDebug() << "Message:" << message;

    // Use your existing envoyerSMS function
    envoyerSMS(formattedPhone, message);
}

// =================== Supprimer ===================

void pagesponsor::supprimerSponsor()
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
        chargerSponsors();           // Refresh the table
        afficherStatistiquesCirculaire(); // Refresh the pie chart
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}

// =================== Tri / Recherche ===================

void pagesponsor::appliquerTri()
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
void pagesponsor::afficherSponsors()
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
void pagesponsor::appliquerRecherche(const QString &texte)
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
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
void pagesponsor::exporterPdf()
{
    qDebug() << "Export PDF des sponsors...";

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter les sponsors en PDF",
        QDir::homePath() + "/liste_sponsors.pdf",
        "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty())
        return;

    // 1) Récupérer les données depuis la base
    QSqlQuery query;
    query.prepare("SELECT ID_SPONSOR, NOM, SECTEUR, BUDGET, CONTRAT_ASSOCIE, "
                  "TYPE_PARTENARIAT, STATUT, EMAIL, TELEPHONE, "
                  "TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT, "
                  "TO_CHAR(DATE_FIN, 'DD/MM/YYYY') AS DATE_FIN "
                  "FROM SPONSOR ORDER BY ID_SPONSOR");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur Base de Données",
                              "Erreur SQL: " + query.lastError().text());
        return;
    }

    QList<QStringList> allRows;
    while (query.next()) {
        QStringList rowData;
        for (int i = 0; i < 11; ++i)
            rowData << query.value(i).toString();
        allRows << rowData;
    }

    int rowCount = allRows.size();
    if (rowCount == 0) {
        QMessageBox::warning(this, "Export PDF",
                             "Aucun sponsor trouvé dans la base de données.");
        return;
    }

    // 2) Configurer l’imprimante
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);

    // Au lieu de setPageMargins, on calcule la zone imprimable nous-mêmes
    QPageLayout layout = printer.pageLayout();
    QRect fullRect = layout.fullRectPixels(printer.resolution()); // toute la page en px

    int margin = 80; // marge en px (gauche, haut, droite, bas)
    QRect pageRect = fullRect.adjusted(margin, margin, -margin, -margin);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur PDF", "Impossible de créer le fichier PDF.");
        return;
    }
    painter.setRenderHint(QPainter::Antialiasing);

    // 3) Titre en haut, centré horizontalement, mais près du bord haut
    QFont titleFont("Arial", 18, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(pageRect.adjusted(0, 0, 0, -pageRect.height()/2),
                     Qt::AlignTop | Qt::AlignHCenter,
                     "LISTE DES SPONSORS");

    // Date en haut-droite
    QFont dateFont("Arial", 10);
    painter.setFont(dateFont);
    QString currentDate = QDate::currentDate().toString("dd/MM/yyyy");
    painter.drawText(pageRect.adjusted(0, 10, -10, 0),
                     Qt::AlignTop | Qt::AlignRight,
                     "Exporté le: " + currentDate);

    // position Y pour le tableau (juste sous le titre)
    int currentY = pageRect.top() + 60;

    // 4) Colonnes
    QStringList headers = {"ID", "Nom", "Secteur", "Budget", "Contrat",
                           "Type Part.", "Statut", "Email", "Téléphone",
                           "Date Début", "Date Fin"};

    // poids relatifs (pour répartir toute la largeur dispo)
    QVector<int> colWeights = {6, 10, 8, 8, 14, 10, 7, 20, 8, 9, 9};
    int weightSum = 0;
    for (int w : colWeights) weightSum += w;

    int startX = pageRect.left();              // **tableau collé à gauche**
    int availableWidth = pageRect.width();     // utilise toute la largeur
    QVector<int> columnWidths;
    columnWidths.reserve(headers.size());
    for (int w : colWeights) {
        int colWidth = availableWidth * w / weightSum;
        columnWidths << colWidth;
    }

    // 5) En-têtes
    QFont headerFont("Arial", 12, QFont::Bold);     // plus grand = plus lisible
    painter.setFont(headerFont);
    int headerHeight = painter.fontMetrics().height() + 10;

    int x = startX;
    painter.setBrush(QColor(240, 240, 240));
    painter.setPen(Qt::NoPen);
    painter.drawRect(startX, currentY, availableWidth, headerHeight);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    for (int i = 0; i < headers.size(); ++i) {
        int w = columnWidths[i];
        painter.drawRect(x, currentY, w, headerHeight);
        QRect headerRect(x + 2, currentY, w - 4, headerHeight);
        painter.drawText(headerRect, Qt::AlignCenter, headers[i]);
        x += w;
    }
    currentY += headerHeight;

    // 6) Lignes de données
    QFont dataFont("Arial", 11);
    painter.setFont(dataFont);
    int rowHeight = painter.fontMetrics().height() + 8;

    int footerSpace = 40;
    int availableHeight = pageRect.bottom() - currentY - footerSpace;
    int rowsPerPage = availableHeight / rowHeight;
    if (rowsPerPage < 1) rowsPerPage = 1;

    int currentPage = 1;
    int rowsOnCurrentPage = 0;

    for (int row = 0; row < allRows.size(); ++row) {

        // nouvelle page si on a plus de place
        if (rowsOnCurrentPage >= rowsPerPage) {
            // pied de page de la page courante
            painter.setPen(Qt::black);
            painter.drawText(pageRect.adjusted(0, 0, 0, -10),
                             Qt::AlignBottom | Qt::AlignLeft,
                             QString("Page %1 - Total: %2 sponsors")
                                 .arg(currentPage).arg(rowCount));

            printer.newPage();
            currentPage++;
            rowsOnCurrentPage = 0;

            // recalculer les rects pour la nouvelle page
            layout = printer.pageLayout();
            fullRect = layout.fullRectPixels(printer.resolution());
            pageRect = fullRect.adjusted(margin, margin, -margin, -margin);

            currentY = pageRect.top() + 20;

            // redessiner les en-têtes en haut-gauche
            painter.setFont(headerFont);
            x = pageRect.left();
            startX = pageRect.left();
            availableWidth = pageRect.width();

            painter.setBrush(QColor(240, 240, 240));
            painter.setPen(Qt::NoPen);
            painter.drawRect(startX, currentY, availableWidth, headerHeight);
            painter.setPen(Qt::black);
            painter.setBrush(Qt::NoBrush);

            // mêmes proportions de colonnes
            x = startX;
            for (int i = 0; i < headers.size(); ++i) {
                int w = columnWidths[i];
                painter.drawRect(x, currentY, w, headerHeight);
                QRect headerRect(x + 2, currentY, w - 4, headerHeight);
                painter.drawText(headerRect, Qt::AlignCenter, headers[i]);
                x += w;
            }

            currentY += headerHeight;
            painter.setFont(dataFont);
        }

        // alternance de couleur de lignes
        if (row % 2 == 0)
            painter.setBrush(QColor(252, 252, 252));
        else
            painter.setBrush(Qt::white);

        painter.setPen(Qt::NoPen);
        painter.drawRect(startX, currentY, availableWidth, rowHeight);

        painter.setPen(QColor(180, 180, 180));
        painter.setBrush(Qt::NoBrush);

        x = startX;
        const QStringList &rowData = allRows[row];
        for (int col = 0; col < headers.size(); ++col) {
            int w = columnWidths[col];
            QString cellText = (col < rowData.size()) ? rowData[col] : "";

            // on coupe un peu mais pas trop
            if (cellText.length() > 40)
                cellText = cellText.left(38) + "...";

            QRect textRect(x + 4, currentY, w - 8, rowHeight);
            Qt::Alignment align = Qt::AlignLeft | Qt::AlignVCenter;
            if (col == 0 || col == 3) // ID, Budget
                align = Qt::AlignRight | Qt::AlignVCenter;

            painter.drawRect(x, currentY, w, rowHeight);
            painter.drawText(textRect, align, cellText);

            x += w;
        }

        currentY += rowHeight;
        rowsOnCurrentPage++;
    }

    // pied de page de la dernière page
    painter.setPen(Qt::black);
    painter.drawText(pageRect.adjusted(0, 0, 0, -10),
                     Qt::AlignBottom | Qt::AlignLeft,
                     QString("Page %1 - Total: %2 sponsors")
                         .arg(currentPage).arg(rowCount));

    painter.end();

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

    QMessageBox::information(this, "Export PDF Réussi",
                             QString("Export PDF réussi !\n"
                                     "%1 sponsors exportés vers:\n%2")
                                 .arg(rowCount)
                                 .arg(fileName));

    qDebug() << "Export PDF terminé :" << fileName;
}
void pagesponsor::afficherStatistiquesCirculaire()
{
    // Récupérer les statistiques depuis la base de données
    QMap<QString, int> statistiques = Sponsor::getStatistiquesPartenariat();

    // Vider le layout existant
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

    // Calculer le total
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

    // Créer la série de données pour le camembert
    QPieSeries *series = new QPieSeries();

    QVector<QColor> colors = {
        QColor("#FF6384"), QColor("#36A2EB"), QColor("#FFCE56"),
        QColor("#4BC0C0"), QColor("#9966FF"), QColor("#FF9F40")
    };

    int colorIndex = 0;

    for (auto it = statistiques.begin(); it != statistiques.end(); ++it) {
        QString type = it.key();
        int count = it.value();
        double pourcentage = (count * 100.0) / total;

        QPieSlice *slice = series->append(
            QString("%1\n%2 sponsors (%3%)")
                .arg(type)
                .arg(count)
                .arg(QString::number(pourcentage, 'f', 1)),
            count
            );

        slice->setColor(colors[colorIndex % colors.size()]);
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);

        colorIndex++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des sponsors par type de partenariat");
    chart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    layout->addWidget(chartView);

    QLabel *totalLabel = new QLabel(QString("Total : %1 sponsors").arg(total));
    totalLabel->setAlignment(Qt::AlignCenter);
    totalLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    layout->addWidget(totalLabel);
}
void pagesponsor::newConnection()
{
 while (TCP_Server->hasPendingConnections())
{
     Add_New_Client_Connection(TCP_Server->nextPendingConnection());
 }
}

void pagesponsor::Read_Data_From_Socket()
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

void pagesponsor::Add_New_Client_Connection(QTcpSocket *socket)
{
    Client_Connection_List.append(socket);
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read_Data_From_Socket()));

    ui->comboBox_Client_list->addItem(QString::number(socket->socketDescriptor()));
     QString Client = "Client : " + QString::number(socket->socketDescriptor()) + " Connected With The Server.";
    ui->textEdit_Client_Messages->append(Client);
}


void pagesponsor::on_pushButton_Send_Message_Type_clicked()
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
void pagesponsor::envoyerSMS(const QString& telephone, const QString& message)
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
void pagesponsor::onSMSReply(QNetworkReply *reply)
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

void pagesponsor::on_pushButton_envoyerSMS_clicked()
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

void pagesponsor::on_Openhercules_clicked()
{
QString herculesPath = "C:/Users/LENOVO/Downloads/hercules_3-2-8.exe";

if(QFile::exists(herculesPath)) {
    QProcess *process = new QProcess(this);
    process->start(herculesPath);
} else {
    QMessageBox::warning(this, "File Not Found",
                       "Hercules executable not found at:\n" + herculesPath);
}
}
#include "smartstudio.h"

void pagesponsor::on_pushButton_62_clicked()
{
    smartstudio *Lc;
    hide();
    Lc = new smartstudio(this);
    Lc->show();
}



void pagesponsor::on_moteur_clicked()
{

}


void pagesponsor::spinMotor(int milliseconds) {

}


void pagesponsor::stopMotor() {
   ;
}
