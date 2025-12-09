#include "pagecreateur.h"
#include "ui_pagecreateur.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QDir>
#include <QAbstractItemModel>
#include <QUrl>
#include <QQmlContext>
#include <QQmlError>
#include <QQuickWidget>
#include <QQuickItem>
#include <QVariantList>
#include <QVariantMap>
#include <QVariant>
#include <QMetaObject>

pagecreateur::pagecreateur(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pagecreateur)
{
    ui->setupUi(this);
    if (ui->pushButton_stat)
    {
        connect(ui->pushButton_stat, &QPushButton::clicked,
                this, &pagecreateur::on_pushButton_stat_clicked);
    }

    ui->comboBox_2->clear();
    ui->comboBox_2->addItem(tr("Tri par défaut"));
    ui->comboBox_2->addItem(tr("Abonnés ↑"));
    ui->comboBox_2->addItem(tr("Abonnés ↓"));
    ui->comboBox_2->setCurrentIndex(0);

    // --- Statistics QML setup ---
    if (ui->quickWidgetstat)
    {
        ui->quickWidgetstat->setResizeMode(QQuickWidget::SizeRootObjectToView);
        ui->quickWidgetstat->rootContext()->setContextProperty("platformStatsModel", QVariantList());
        ui->quickWidgetstat->setSource(QUrl(QStringLiteral("qrc:/qml/PieChartStat.qml")));
    }

    // --- TableView configuration ---
    //ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // --- Real-time input validators (instant feedback) ---
    QRegularExpression rxName("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]*$");  // letters + spaces
    ui->lineEdut_nom->setValidator(new QRegularExpressionValidator(rxName, this));
    ui->lineEdit_prenom->setValidator(new QRegularExpressionValidator(rxName, this));

    QRegularExpression rxEmail(R"(^[\w\.\-]*@?[\w\-]*\.?[a-zA-Z]*$)");
    ui->LineEdit_email->setValidator(new QRegularExpressionValidator(rxEmail, this));

    ui->LineEdit_numtel->setValidator(new QIntValidator(10000000, 99999999, this));  // 8 digits
    ui->LineEdit_nbabonnes->setValidator(new QIntValidator(0, 999999999, this));    // positive int

    resetPlatformSelection();
}

pagecreateur::~pagecreateur()
{
    delete ui;
}

void pagecreateur::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event); // Call parent class implementation

    // Create object
    createurcrud crc;
    ui->tableView->setModel(crc.afficher());
    ui->comboBox_2->setCurrentIndex(0);
}
// ---------------------- STATISTIQUES ----------------------
void pagecreateur::on_pushButton_stat_clicked()
{
    createurcrud cr;
    const auto stats = cr.statistiquesParPlatforme();

    QVariantList seriesData;
    seriesData.reserve(stats.size());
    for (const auto &entry : stats)
    {
        QVariantMap row;
        row.insert(QStringLiteral("label"), entry.first);
        row.insert(QStringLiteral("value"), entry.second);
        seriesData << row;
    }

    if (!ui->quickWidgetstat)
    {
        QMessageBox::warning(this, tr("Widget manquant"),
                             tr("Le graphique n'est pas disponible dans l'interface."));
        return;
    }

    if (ui->quickWidgetstat->status() == QQuickWidget::Error)
    {
        QStringList errors;
        const auto qmlErrors = ui->quickWidgetstat->errors();
        for (const QQmlError &err : qmlErrors)
            errors << err.toString();

        QMessageBox::critical(
            this, tr("Erreur QML"),
            tr("Impossible d'afficher le graphique (erreur QML).\n%1")
                .arg(errors.join("\n")));
        return;
    }

    QQmlContext *context = ui->quickWidgetstat->rootContext();
    context->setContextProperty(QStringLiteral("platformStatsModel"), seriesData);

    if (QObject *root = ui->quickWidgetstat->rootObject())
    {
        root->setProperty("platformStatsModel", seriesData);
        QMetaObject::invokeMethod(root, "refreshSeries",
                                  Q_ARG(QVariant, QVariant(seriesData)));
    }
    else
    {
        QMessageBox::warning(this, tr("Graphique indisponible"),
                             tr("Le composant QML n'est pas encore prêt."));
    }
}
// ---------------------- TRI DANS LE COMBOBOX ----------------------
void pagecreateur::on_comboBox_2_currentIndexChanged(int index)
{
    createurcrud sorter;
    QSqlQueryModel* model = nullptr;

    if (index == 1)
        model = sorter.afficherTrieParAbonnes(true);
    else if (index == 2)
        model = sorter.afficherTrieParAbonnes(false);
    else
        model = sorter.afficher();

    ui->tableView->setModel(model);
}
// ---------------------- RECHERCHE PAR ID ----------------------
void pagecreateur::on_l2_15_textChanged(const QString &text)
{
    QString trimmed = text.trimmed();
    if (trimmed.isEmpty())
    {
        on_comboBox_2_currentIndexChanged(ui->comboBox_2->currentIndex());
        return;
    }

    createurcrud searcher;
    ui->tableView->setModel(searcher.rechercherParId(trimmed));
}

// ============================================================
// ---------------------- VALIDATION DES CHAMPS ----------------------
// ============================================================

bool pagecreateur::isValidName(const QString &s)
{
    static const QRegularExpression re("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]+$");
    return !s.isEmpty() && re.match(s).hasMatch();
}

bool pagecreateur::isValidEmail(const QString &s)
{
    static const QRegularExpression re(
        R"(^[\w\.\-]{1,64}@[\w\-]{1,255}(\.[a-zA-Z]{2,})+$)");
    return re.match(s).hasMatch();
}

bool pagecreateur::isValidPhone(const QString &s)
{
    static const QRegularExpression re("^[0-9]{8}$");
    return re.match(s).hasMatch();
}

bool pagecreateur::isValidNumber(const QString &s)
{
    static const QRegularExpression re("^[0-9]+$");
    return !s.isEmpty() && re.match(s).hasMatch();
}

// ============================================================
// ADD CREATOR (Button 60)
// ============================================================

void pagecreateur::on_pushButton_60_clicked()
{
    QString idc       = ui->lineEdit_idcreateur->text().trimmed();
    QString nom       = ui->lineEdut_nom->text().trimmed();
    QString prenom    = ui->lineEdit_prenom->text().trimmed();
    QString email     = ui->LineEdit_email->text().trimmed();
    QString numtel    = ui->LineEdit_numtel->text().trimmed();
    QString nbStr     = ui->LineEdit_nbabonnes->text().trimmed();
    QString platforme = m_selectedPlatform;

    // --- Check required fields ---
    if (idc.isEmpty() || nom.isEmpty() || prenom.isEmpty() ||
        email.isEmpty() || numtel.isEmpty() || nbStr.isEmpty() ||
        platforme.isEmpty() || platforme == "Plateforme")
    {
        QMessageBox::warning(this, tr("Champ manquant"),
                             tr("Tous les champs sont obligatoires."));
        return;
    }

    // --- Name validation ---
    if (!isValidName(nom))
    {
        QMessageBox::warning(this, tr("Nom invalide"),
                             tr("Le <b>nom</b> ne doit contenir que des lettres et espaces."));
        return;
    }
    if (!isValidName(prenom))
    {
        QMessageBox::warning(this, tr("Prénom invalide"),
                             tr("Le <b>prénom</b> ne doit contenir que des lettres et espaces."));
        return;
    }

    // --- Email ---
    if (!isValidEmail(email))
    {
        QMessageBox::warning(this, tr("E-mail invalide"),
                             tr("Veuillez entrer une adresse e-mail valide."));
        return;
    }

    // --- Phone (8 digits) ---
    if (!isValidPhone(numtel))
    {
        QMessageBox::warning(this, tr("Téléphone invalide"),
                             tr("Le numéro doit contenir <b>exactement 8 chiffres</b>."));
        return;
    }

    // --- Subscribers ---
    if (!isValidNumber(nbStr))
    {
        QMessageBox::warning(this, tr("Abonnés invalide"),
                             tr("Le nombre d’abonnés doit être un nombre positif."));
        return;
    }
    int nbabonnes = nbStr.toInt();

    // --- Clear form ---
    ui->lineEdit_idcreateur->clear();
    ui->lineEdut_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->LineEdit_email->clear();
    ui->LineEdit_numtel->clear();
    ui->LineEdit_nbabonnes->clear();
    resetPlatformSelection();

    // --- Insert into DB ---
    createurcrud crc(idc, nom, prenom, email,platforme, numtel, nbabonnes);
    if (crc.Ajouter())
    {
        ui->tableView->setModel(crc.afficher());
        QMessageBox::information(this, tr("Succès"),
                                 tr("Créateur ajouté avec succès."), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Échec de l’ajout. Vérifiez les données."), QMessageBox::Cancel);
    }
}

// ============================================================
// PLATFORM CHECKBOXES
// ============================================================

void pagecreateur::on_checkBox_2_clicked() { updatePlatformSelection("Instagram", ui->checkBox_2); }
void pagecreateur::on_checkBox_clicked()   { updatePlatformSelection("Tiktok", ui->checkBox); }
void pagecreateur::on_checkBox_3_clicked() { updatePlatformSelection("Youtube", ui->checkBox_3); }
void pagecreateur::on_checkBox_6_clicked() { updatePlatformSelection("Autres", ui->checkBox_6); }

// ============================================================
// SELECT ROW → FILL FORM
// ============================================================

void pagecreateur::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) return;

    QString idc       = model->data(model->index(row, 0)).toString();
    QString nom       = model->data(model->index(row, 1)).toString();
    QString prenom    = model->data(model->index(row, 2)).toString();
    QString email     = model->data(model->index(row, 3)).toString();
    QString platform  = model->data(model->index(row, 4)).toString();
    QString numtel    = model->data(model->index(row, 5)).toString();
    int nbabonnes     = model->data(model->index(row, 6)).toInt();

    ui->lineEdit_idcreateur->setText(idc);
    ui->lineEdut_nom->setText(nom);
    ui->lineEdit_prenom->setText(prenom);
    ui->LineEdit_email->setText(email);
    ui->LineEdit_numtel->setText(numtel);
    ui->LineEdit_nbabonnes->setText(QString::number(nbabonnes));

    if (platform.compare("Instagram", Qt::CaseInsensitive) == 0)
        updatePlatformSelection("Instagram", ui->checkBox_2);
    else if (platform.compare("Tiktok", Qt::CaseInsensitive) == 0)
        updatePlatformSelection("Tiktok", ui->checkBox);
    else if (platform.compare("Youtube", Qt::CaseInsensitive) == 0)
        updatePlatformSelection("Youtube", ui->checkBox_3);
    else if (platform.compare("Autres", Qt::CaseInsensitive) == 0)
        updatePlatformSelection("Autres", ui->checkBox_6);
    else
        resetPlatformSelection();
}

// ============================================================
// EDIT CREATOR (Button 59)
// ============================================================

void pagecreateur::on_pushButton_59_clicked()
{
    QString idc       = ui->lineEdit_idcreateur->text().trimmed();
    QString nom       = ui->lineEdut_nom->text().trimmed();
    QString prenom    = ui->lineEdit_prenom->text().trimmed();
    QString email     = ui->LineEdit_email->text().trimmed();
    QString numtel    = ui->LineEdit_numtel->text().trimmed();
    QString nbStr     = ui->LineEdit_nbabonnes->text().trimmed();
    QString platforme = m_selectedPlatform;

    if (idc.isEmpty())
    {
        QMessageBox::warning(this, tr("Sélection requise"),
                             tr("Veuillez sélectionner un créateur dans le tableau."));
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        numtel.isEmpty() || nbStr.isEmpty() || platforme.isEmpty() || platforme == "Plateforme")
    {
        QMessageBox::warning(this, tr("Champ manquant"),
                             tr("Tous les champs sont obligatoires."));
        return;
    }

    if (!isValidName(nom) || !isValidName(prenom))
    {
        QMessageBox::warning(this, tr("Nom/Prénom invalide"),
                             tr("Lettres et espaces uniquement."));
        return;
    }
    if (!isValidEmail(email))
    {
        QMessageBox::warning(this, tr("E-mail invalide"),
                             tr("Format e-mail incorrect."));
        return;
    }
    if (!isValidPhone(numtel))
    {
        QMessageBox::warning(this, tr("Téléphone invalide"),
                             tr("8 chiffres exactement."));
        return;
    }
    if (!isValidNumber(nbStr))
    {
        QMessageBox::warning(this, tr("Abonnés invalide"),
                             tr("Chiffres uniquement."));
        return;
    }
    int nbabonnes = nbStr.toInt();

    createurcrud crc;
    crc.setidc(idc);
    crc.setnom(nom);
    crc.setprenom(prenom);
    crc.setemail(email);
    crc.setplatforme(platforme);
    crc.setnumtel(numtel);
    crc.setnbabonnes(nbabonnes);

    if (crc.modifier())
    {
        ui->tableView->setModel(crc.afficher());
        clearForm();
        QMessageBox::information(this, tr("Succès"),
                                 tr("Créateur modifié avec succès."), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Échec de la modification."), QMessageBox::Cancel);
    }
}
// ---------------------- exportation pdf ----------------------
void pagecreateur::on_pushButton_61_clicked()
{
    QAbstractItemModel* model = ui->tableView->model();
    if (!model)
    {
        QMessageBox::warning(this, tr("Table vide"),
                             tr("Aucune donnée à exporter."));
        return;
    }

    QString suggested = QDir::homePath() + "/createurs.pdf";
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Exporter en PDF"), suggested, tr("Fichiers PDF (*.pdf)"));

    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setPageOrientation(QPageLayout::Portrait);
    pdf.setResolution(300);

    QTextDocument doc;
    QString html = "<h2 align='center'>Liste des créateurs</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'>";
    html += "<tr style='background-color:#e0e0e0;'>";

    int columnCount = model->columnCount();
    for (int col = 0; col < columnCount; ++col)
    {
        const QString header = model->headerData(col, Qt::Horizontal).toString().toHtmlEscaped();
        html += "<th>" + header + "</th>";
    }
    html += "</tr>";

    const int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        html += "<tr>";
        for (int col = 0; col < columnCount; ++col)
        {
            QString cell = model->data(model->index(row, col)).toString().toHtmlEscaped();
            if (cell.isEmpty())
                cell = "&nbsp;";
            html += "<td>" + cell + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";
    doc.setHtml(html);
    doc.print(&pdf);

    QMessageBox::information(this, tr("Export terminé"),
                             tr("Le tableau a été exporté dans :\n%1").arg(fileName));
}

// ============================================================
// DELETE ON DOUBLE-CLICK
// ============================================================

void pagecreateur::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) return;

    QString idc = model->data(model->index(row, 0)).toString();
    if (idc.isEmpty())
    {
        QMessageBox::warning(this, tr("Erreur"), tr("ID introuvable."), QMessageBox::Ok);
        return;
    }

    auto reply = QMessageBox::question(
        this, tr("Confirmer la suppression"),
        tr("Supprimer le créateur <b>ID: %1</b> ?<br>Cette action est <u>irréversible</u>.").arg(idc),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        createurcrud crc;
        if (crc.Supprime(idc))
        {
            ui->tableView->setModel(crc.afficher());
            clearForm();
            QMessageBox::information(this, tr("Supprimé"),
                                     tr("Créateur supprimé avec succès."), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(this, tr("Échec"),
                                  tr("Impossible de supprimer."), QMessageBox::Cancel);
        }
    }
}

// ============================================================
// HELPER: CLEAR ALL FIELDS
// ============================================================

void pagecreateur::clearForm()
{
    ui->lineEdit_idcreateur->clear();
    ui->lineEdut_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->LineEdit_email->clear();
    ui->LineEdit_numtel->clear();
    ui->LineEdit_nbabonnes->clear();
    resetPlatformSelection();
}

void pagecreateur::resetPlatformSelection()
{
    m_selectedPlatform = QStringLiteral("Plateforme");
    if (!ui)
        return;

    if (ui->checkBox)
        ui->checkBox->setChecked(false);
    if (ui->checkBox_2)
        ui->checkBox_2->setChecked(false);
    if (ui->checkBox_3)
        ui->checkBox_3->setChecked(false);
    if (ui->checkBox_6)
        ui->checkBox_6->setChecked(false);
}

void pagecreateur::updatePlatformSelection(const QString &platform, QCheckBox *trigger)
{
    if (!ui)
        return;

    m_selectedPlatform = platform;

    if (ui->checkBox_2)
        ui->checkBox_2->setChecked(trigger == ui->checkBox_2);
    if (ui->checkBox)
        ui->checkBox->setChecked(trigger == ui->checkBox);
    if (ui->checkBox_3)
        ui->checkBox_3->setChecked(trigger == ui->checkBox_3);
    if (ui->checkBox_6)
        ui->checkBox_6->setChecked(trigger == ui->checkBox_6);
}

void pagecreateur::on_pb_qrcode_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Générer un QR Code - Sélectionnez un créateur"));
    dialog->resize(900, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QTableView *tableView = new QTableView(dialog);
    createurcrud crc;
    tableView->setModel(crc.afficher());

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    QLabel *qrLabel = new QLabel(dialog);
    qrLabel->setAlignment(Qt::AlignCenter);
    qrLabel->setMinimumSize(300, 300);
    qrLabel->setStyleSheet("border: 2px solid gray; background: white;");
    qrLabel->setText(tr("<h3>Cliquez sur une ligne pour générer le QR Code</h3>"));

    layout->addWidget(tableView);
    layout->addWidget(qrLabel);

    // Connexion : clic → QR code
    connect(tableView, &QTableView::clicked, this, [=](const QModelIndex &index) {
        QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(tableView->model());
        if (!model) return;

        QString idc = model->data(model->index(index.row(), 0)).toString();
        if (idc.isEmpty()) {
            qrLabel->setText(tr("ID non trouvé"));
            return;
        }

        QrCode qr = QrCode::encodeText(idc.toUtf8().constData(), QrCode::Ecc::MEDIUM);
        QImage img = qrToImage(qr, 10, 4);

        QPixmap pixmap = QPixmap::fromImage(img);

        qrLabel->setText("");   // IMPORTANT → remove text
        qrLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    dialog->exec();
    delete dialog;
}


QImage pagecreateur::qrToImage(const QrCode &qr, int scale, int border)
{
    if (border < 0) border = 4;
    int size = qr.getSize();
    QImage image((size + border * 2) * scale, (size + border * 2) * scale, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qr.getModule(x, y)) {
                painter.drawRect((x + border) * scale, (y + border) * scale, scale, scale);
            }
        }
    }
    return image;
}
// ---------------------- calendrier ----------------------
void pagecreateur::on_pb_calendar_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Calendrier des événements"));
    dialog->resize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    QCalendarWidget *calendar = new QCalendarWidget(dialog);
    calendar->setGridVisible(true);
    calendar->setMinimumDate(QDate(2020, 1, 1));
    calendar->setMaximumDate(QDate(2030, 12, 31));

    // Label removed (no date, no ID)
    QLabel *dateLabel = new QLabel("", dialog);
    dateLabel->setAlignment(Qt::AlignCenter);

    QTextEdit *eventEdit = new QTextEdit(dialog);
    eventEdit->setPlaceholderText(tr("Entrez la description de l'événement ici..."));
    eventEdit->setMaximumHeight(120);

    QPushButton *saveButton = new QPushButton(tr("Sauvegarder l'événement"), dialog);
    saveButton->setStyleSheet("QPushButton { padding: 10px; font-weight: bold; }");

    QLabel *eventDisplay = new QLabel(dialog);
    eventDisplay->setWordWrap(true);
    eventDisplay->setStyleSheet("QLabel { background: #f0f8ff; padding: 15px; border: 1px solid #ccc; border-radius: 8px; }");

    // Form layout
    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->addWidget(new QLabel(tr("<h3>Ajouter un événement</h3>")));
    formLayout->addWidget(eventEdit);
    formLayout->addWidget(saveButton);

    mainLayout->addWidget(calendar);
    mainLayout->addWidget(dateLabel); // stays empty
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(new QLabel(tr("<h3>Événements :</h3>")));
    mainLayout->addWidget(eventDisplay);
    mainLayout->addStretch();

    // Load events for a selected date
    auto loadEventsForDate = [=](const QDate &date) {
        QString dateStr = date.toString("yyyy-MM-dd");

        // No header text, leave dateLabel empty
        dateLabel->setText("");

        QSqlQuery query;
        query.prepare("SELECT description FROM evenements WHERE date_event = ?");
        query.addBindValue(dateStr);
        query.exec();

        QStringList events;
        while (query.next()) {
            events << "• " + query.value(0).toString();
        }

        if (events.isEmpty()) {
            eventDisplay->setText("<i>Aucun événement prévu ce jour.</i>");
        } else {
            eventDisplay->setText(events.join("<br>"));
        }
    };

    // On date change
    connect(calendar, &QCalendarWidget::selectionChanged, this, [=]() {
        QDate selected = calendar->selectedDate();
        loadEventsForDate(selected);
        eventEdit->clear();
    });

    // Save event
    connect(saveButton, &QPushButton::clicked, this, [=]() {
        QDate selected = calendar->selectedDate();
        QString desc = eventEdit->toPlainText().trimmed();

        if (desc.isEmpty()) {
            QMessageBox::warning(dialog, tr("Champ vide"), tr("Veuillez entrer une description."));
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO evenements (date_event, description) VALUES (?, ?)");
        query.addBindValue(selected.toString("yyyy-MM-dd"));
        query.addBindValue(desc);

        if (query.exec()) {
            QMessageBox::information(dialog, tr("Succès"), tr("Événement sauvegardé !"));
            eventEdit->clear();
            loadEventsForDate(selected);
        } else {
            QMessageBox::critical(dialog, tr("Erreur"), tr("Échec de sauvegarde."));
        }
    });

    // Load current date events on start
    loadEventsForDate(QDate::currentDate());

    dialog->exec();
    delete dialog;
}

#include "smartstudio.h"
void pagecreateur::on_pushButton_113_clicked()
{
    smartstudio *sp;
    hide();
    sp = new smartstudio(this);
    sp->show();
}

