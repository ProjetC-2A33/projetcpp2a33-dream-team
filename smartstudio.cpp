#include "smartstudio.h"
#include "ui_smartstudio.h"
#include "employe.h"
#include "employeepiewidget.h"
#include "arduino.h"
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
    ui->comboBox_type->addItems({"Financier", "En nature", "Co-branding"});
    ui->comboBox_statut->addItems({"Actif", "Inactif"});







       // Connect the selection changed signal manually if needed
       connect(ui->tableWidget_4, &QTableWidget::itemSelectionChanged,
               this, &smartstudio::on_tableWidget_4_itemSelectionChanged);
       connect(ui->tableWidget_4, &QTableView::clicked, this, &smartstudio::on_tab_commqr_clicked);
       // Connect search and sort signals
       connect(ui->l2_29, &QLineEdit::textChanged,
               this, &smartstudio::on_l2_29_textChanged);


       //    Load data after UI is fully initialized (delay to ensure database connection is ready)
       //QTimer::singleShot(200, this, [this]() {
         //  loadTableData();
      // });

      /*chartTypesView = new QChartView(ui->widget_chart_types);
       chartTypesView->setRenderHint(QPainter::Antialiasing);
       chartTypesView->setGeometry(0, 0, ui->widget_chart_types->width(), ui->widget_chart_types->height());

       chartLocView = new QChartView(ui->widget_chart_loc);
       chartLocView->setRenderHint(QPainter::Antialiasing);
       chartLocView->setGeometry(0, 0, ui->widget_chart_loc->width(), ui->widget_chart_loc->height());
       */

    // ==================== INITIALISER DÉTECTEUR DE FLAMME ====================
    // Active la surveillance incendie - met l'état matériel "EN DANGER" si flamme détectée
    setupFlameDetector();
    
    // ==================== INITIALISER CAPTEUR DE TEMPÉRATURE ====================
    setupTemperatureSensor();
}

// ==================== CONFIGURATION DÉTECTEUR DE FLAMME ====================
void smartstudio::setupFlameDetector()
{
    flameDetector = new FlameDetector(this);
    
    // Connexion des signaux
    connect(flameDetector, &FlameDetector::flameDetected, this, [this](const QString &localisation) {
        qDebug() << "🔥🔥🔥 ALERTE INCENDIE REÇUE DANS SMARTSTUDIO!";
        
        // Mettre à jour la base de données
        flameDetector->updateMaterielEtatDanger(localisation);
        
        // Rafraîchir la table pour voir les changements
        refreshTable();
        
        // Afficher alerte
        QMessageBox::critical(this, "🔥 ALERTE INCENDIE 🔥",
            "FLAMME DÉTECTÉE!\n\n"
            "⚠️ Tous les matériels ont été mis\n"
            "en état: EN DANGER\n\n"
            "ÉVACUEZ IMMÉDIATEMENT!");
    });
    
    connect(flameDetector, &FlameDetector::flameCleared, this, [this](const QString &localisation) {
        qDebug() << "✅ Flamme disparue - Situation normale";
        Q_UNUSED(localisation);
    });
    
    connect(flameDetector, &FlameDetector::connectionStatusChanged, this, [](bool connected) {
        if (connected) {
            qDebug() << "🔥✅ Détecteur de flamme CONNECTÉ";
        } else {
            qDebug() << "🔥❌ Détecteur de flamme DÉCONNECTÉ";
        }
    });
    
    // Démarrer la détection automatiquement
    if (flameDetector->startDetection()) {
        qDebug() << "🔥 Surveillance incendie ACTIVE";
    } else {
        qDebug() << "⚠️ Détecteur de flamme non disponible (Arduino non connecté?)";
    }
}

// ==================== CONFIGURATION CAPTEUR DE TEMPÉRATURE ====================
void smartstudio::setupTemperatureSensor()
{
    temperatureSensor = new TemperatureSensor(this);
    
    // Créer les labels pour afficher la température sur la page matériel (page_5 / stackedWidget_2)
    // On les crée de manière programmatique
    tempLabel = new QLabel(ui->stackedWidget_2->widget(0));  // page_9 dans stackedWidget_2
    tempLabel->setGeometry(450, 460, 200, 40);
    tempLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Arial Black';"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   color: white;"
        "   background-color: #0A0A2A;"
        "   border: 2px solid #1E1E5A;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
    );
    tempLabel->setText("🌡️ Temp: -- °C");
    tempLabel->show();
    
    humLabel = new QLabel(ui->stackedWidget_2->widget(0));  // page_9 dans stackedWidget_2
    humLabel->setGeometry(660, 460, 200, 40);
    humLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Arial Black';"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   color: white;"
        "   background-color: #0A0A2A;"
        "   border: 2px solid #1E1E5A;"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
    );
    humLabel->setText("💧 Hum: -- %");
    humLabel->show();
    
    // Connexion des signaux de température
    connect(temperatureSensor, &TemperatureSensor::temperatureChanged, this, [this](float temp) {
        QString text = QString("🌡️ Temp: %1 °C").arg(temp, 0, 'f', 1);
        tempLabel->setText(text);
        
        // Changer couleur si température anormale
        if (temp > 35.0) {
            tempLabel->setStyleSheet(
                "QLabel { font-family: 'Arial Black'; font-size: 16px; font-weight: bold; "
                "color: white; background-color: #FF0000; border: 2px solid red; "
                "border-radius: 10px; padding: 5px; }"
            );
        } else if (temp < 10.0) {
            tempLabel->setStyleSheet(
                "QLabel { font-family: 'Arial Black'; font-size: 16px; font-weight: bold; "
                "color: white; background-color: #0066FF; border: 2px solid blue; "
                "border-radius: 10px; padding: 5px; }"
            );
        } else {
            tempLabel->setStyleSheet(
                "QLabel { font-family: 'Arial Black'; font-size: 16px; font-weight: bold; "
                "color: white; background-color: #0A0A2A; border: 2px solid #1E1E5A; "
                "border-radius: 10px; padding: 5px; }"
            );
        }
    });
    
    connect(temperatureSensor, &TemperatureSensor::humidityChanged, this, [this](float hum) {
        QString text = QString("💧 Hum: %1 %").arg(hum, 0, 'f', 1);
        humLabel->setText(text);
        
        // Changer couleur si humidité élevée
        if (hum > 80.0) {
            humLabel->setStyleSheet(
                "QLabel { font-family: 'Arial Black'; font-size: 16px; font-weight: bold; "
                "color: white; background-color: #FF6600; border: 2px solid orange; "
                "border-radius: 10px; padding: 5px; }"
            );
        } else {
            humLabel->setStyleSheet(
                "QLabel { font-family: 'Arial Black'; font-size: 16px; font-weight: bold; "
                "color: white; background-color: #0A0A2A; border: 2px solid #1E1E5A; "
                "border-radius: 10px; padding: 5px; }"
            );
        }
    });
    
    connect(temperatureSensor, &TemperatureSensor::alertTriggered, this, [this](const QString &alertType, float value) {
        QString message;
        if (alertType.contains("TEMPÉRATURE ÉLEVÉE")) {
            message = QString("⚠️ ALERTE: Température élevée!\n\nValeur: %1 °C\n\nVérifiez la climatisation du local.").arg(value, 0, 'f', 1);
            QMessageBox::warning(this, "🌡️ Alerte Température", message);
        } else if (alertType.contains("TEMPÉRATURE BASSE")) {
            message = QString("⚠️ ALERTE: Température basse!\n\nValeur: %1 °C\n\nVérifiez le chauffage du local.").arg(value, 0, 'f', 1);
            QMessageBox::warning(this, "🌡️ Alerte Température", message);
        } else if (alertType.contains("HUMIDITÉ")) {
            message = QString("⚠️ ALERTE: Humidité élevée!\n\nValeur: %1 %\n\nRisque pour le matériel électronique.").arg(value, 0, 'f', 1);
            QMessageBox::warning(this, "💧 Alerte Humidité", message);
        }
    });
    
    connect(temperatureSensor, &TemperatureSensor::connectionStatusChanged, this, [this](bool connected) {
        if (connected) {
            qDebug() << "🌡️✅ Capteur de température CONNECTÉ";
            tempLabel->setText("🌡️ Temp: Lecture...");
            humLabel->setText("💧 Hum: Lecture...");
        } else {
            qDebug() << "🌡️❌ Capteur de température DÉCONNECTÉ";
            tempLabel->setText("🌡️ Temp: N/A");
            humLabel->setText("💧 Hum: N/A");
        }
    });
    
    // Démarrer la lecture automatiquement
    if (temperatureSensor->startReading()) {
        qDebug() << "🌡️ Surveillance température ACTIVE";
    } else {
        qDebug() << "⚠️ Capteur de température non disponible (Arduino non connecté?)";
        tempLabel->setText("🌡️ Non connecté");
        humLabel->setText("💧 Non connecté");
    }
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
#include "pagecreateur.h"
void smartstudio::on_pushButton_82_clicked()
{
    pagecreateur *Cp;
    hide();
    Cp = new pagecreateur(this);
    Cp->show();
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
#include "mainwindow.h"
void smartstudio::on_pushButton_85_clicked()
{
    MainWindow *Lp;
    hide();
    Lp = new MainWindow(this);
    Lp->show();
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

    const QString validEmail = QStringLiteral("admin");
    const QString validPassword = QStringLiteral("admin");

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
#include "pagesponsor.h"
void smartstudio::on_pushButton_83_clicked()
{
    pagesponsor *dg = new pagesponsor(this);
    hide();
    dg->show();
}


void smartstudio::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
#include "smartstudio.h"
#include "ui_smartstudio.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include <QTimer>
#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QPixmap>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QPageSize>
#include <QPageLayout>
#include <QDate>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QValueAxis>







// Load data from database into tableWidget_4
void smartstudio::loadTableData()
{
    materielle.lire(ui->tableWidget_4, this);
    updateCharts();
}

// Get data from UI fields
Materielle::MaterielData smartstudio::getDataFromUI()
{
    Materielle::MaterielData data;

    data.idMateriel = ui->lineEdit_47->text();
    data.etatMateriel = ui->l2_27->text();
    data.localisation = ui->comboBox_7->currentText();
    data.dateAchat = ui->dateTimeEdit_15->date();
    data.nbMateriel = ui->l2_28->text().toInt();

    // Get type materiel from checkboxes
    QString typeMateriel = "";
    if (ui->checkBox_13->isChecked()) typeMateriel += "Camera ";
    if (ui->checkBox_14->isChecked()) typeMateriel += "Produit photo shoot ";
    if (ui->checkBox_15->isChecked()) typeMateriel += "Micro ";
    if (ui->checkBox_17->isChecked()) typeMateriel += "PC ";
    if (ui->checkBox_18->isChecked()) typeMateriel += "Tablette graphique ";
    data.typeMateriel = typeMateriel.trimmed();

    // Default SUIVI value if not provided
    data.suivi = "En cours";

    return data;
}

// Clear input fields
void smartstudio::clearInputs()
{
    ui->lineEdit_47->clear();
    ui->l2_27->clear();
    ui->l2_28->clear();
    ui->comboBox_7->setCurrentIndex(0);
    ui->dateTimeEdit_15->setDate(QDate::currentDate());

    // Clear checkboxes
    ui->checkBox_13->setChecked(false);
    ui->checkBox_14->setChecked(false);
    ui->checkBox_15->setChecked(false);
    ui->checkBox_17->setChecked(false);
    ui->checkBox_18->setChecked(false);
}

// Refresh table
void smartstudio::refreshTable()
{
    loadTableData();
}

// Ajouter (Add) button
void smartstudio::on_pushButton_74_clicked()
{
    Materielle::MaterielData data = getDataFromUI();

    // Validation
    if (data.idMateriel.isEmpty() || data.etatMateriel.isEmpty() ||
        data.localisation.isEmpty() || data.typeMateriel.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires!");
        return;
    }

    if (materielle.ajouter(data, this)) {
        clearInputs();
        refreshTable();
    }
}

// Annuler (Cancel) button
void smartstudio::on_pushButton_73_clicked()
{
    clearInputs();
}

// Selection changed - populate fields for modification
void smartstudio::on_tableWidget_4_itemSelectionChanged()
{
    int currentRow = ui->tableWidget_4->currentRow();
    if (currentRow >= 0 && ui->tableWidget_4->item(currentRow, 0)) {
        // Populate fields with selected row data
        ui->lineEdit_47->setText(ui->tableWidget_4->item(currentRow, 0)->text());
        if (ui->tableWidget_4->item(currentRow, 1))
            ui->l2_27->setText(ui->tableWidget_4->item(currentRow, 1)->text());

        // Set combo box
        if (ui->tableWidget_4->item(currentRow, 2)) {
            QString localisation = ui->tableWidget_4->item(currentRow, 2)->text();
            int index = ui->comboBox_7->findText(localisation);
            if (index >= 0) {
                ui->comboBox_7->setCurrentIndex(index);
            }
        }

        // Set date
        if (ui->tableWidget_4->item(currentRow, 3)) {
            QDate date = QDate::fromString(ui->tableWidget_4->item(currentRow, 3)->text(), "yyyy-MM-dd");
            if (date.isValid()) {
                ui->dateTimeEdit_15->setDate(date);
            }
        }

        if (ui->tableWidget_4->item(currentRow, 4))
            ui->l2_28->setText(ui->tableWidget_4->item(currentRow, 4)->text());

        // Set type materiel checkboxes
        if (ui->tableWidget_4->item(currentRow, 5)) {
            QString typeMateriel = ui->tableWidget_4->item(currentRow, 5)->text();
            ui->checkBox_13->setChecked(typeMateriel.contains("Camera"));
            ui->checkBox_14->setChecked(typeMateriel.contains("Produit photo shoot"));
            ui->checkBox_15->setChecked(typeMateriel.contains("Micro"));
            ui->checkBox_17->setChecked(typeMateriel.contains("PC"));
            ui->checkBox_18->setChecked(typeMateriel.contains("Tablette graphique"));
        }
    }
}

// Modifier (Update) button
void smartstudio::on_pushButton_75_clicked()
{
    int currentRow = ui->tableWidget_4->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un matériel à modifier!");
        return;
    }

    QString oldId = ui->tableWidget_4->item(currentRow, 0)->text();
    Materielle::MaterielData data = getDataFromUI();

    // Get SUIVI from table if available
    if (ui->tableWidget_4->item(currentRow, 6)) {
        QString suivi = ui->tableWidget_4->item(currentRow, 6)->text();
        if (!suivi.isEmpty()) {
            data.suivi = suivi;
        }
    }

    // Validation
    if (data.idMateriel.isEmpty() || data.etatMateriel.isEmpty() ||
        data.localisation.isEmpty() || data.typeMateriel.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires!");
        return;
    }

    if (materielle.modifier(oldId, data, this)) {
        clearInputs();
        refreshTable();
    }
}

// Supprimer (Delete) button
void smartstudio::on_pushButton_76_clicked()
{
    int currentRow = ui->tableWidget_4->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un matériel à supprimer!");
        return;
    }

    QString idMateriel = ui->tableWidget_4->item(currentRow, 0)->text();

    int ret = QMessageBox::question(this, "Confirmation",
                                    "Êtes-vous sûr de vouloir supprimer ce matériel (ID: " + idMateriel + ")?",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        if (materielle.supprimer(idMateriel, this)) {
            clearInputs();
            refreshTable();
        }
    }
}
//statistics
void smartstudio::updateCharts()
{
 /*   int camera = 0, micro = 0, pc = 0, tablette = 0;
    int onsite = 0, remote = 0, hybrid = 0;

    for (int row = 0; row < ui->tableWidget_4->rowCount(); ++row) {
        QTableWidgetItem *typeItem = ui->tableWidget_4->item(row, 5);
        QTableWidgetItem *nbItem = ui->tableWidget_4->item(row, 4);
        int nb = nbItem ? nbItem->text().toInt() : 0;
        if (typeItem) {
            QString t = typeItem->text();
            if (t.contains("camera", Qt::CaseInsensitive)) camera += nb;
            if (t.contains("micro", Qt::CaseInsensitive)) micro += nb;
            if (t.contains("pc", Qt::CaseInsensitive)) pc += nb;
            if (t.contains("tablette", Qt::CaseInsensitive)) tablette += nb;
        }
        QTableWidgetItem *locItem = ui->tableWidget_4->item(row, 2);
        if (locItem) {
            QString l = locItem->text();
            if (l.contains("On site", Qt::CaseInsensitive)) onsite += nb;
            else if (l.contains("Remote", Qt::CaseInsensitive)) remote += nb;
            else if (l.contains("Hybrid", Qt::CaseInsensitive)) hybrid += nb;
        }
    }

    auto *barSet = new QBarSet("Types");
    *barSet << camera << micro << pc << tablette;
    auto *series = new QBarSeries();
    series->append(barSet);

    auto *chartTypes = new QChart();
    chartTypes->addSeries(series);
    chartTypes->setTitle("Répartition par type");
    chartTypes->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Camera" << "Micro" << "PC" << "Tablette";
    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chartTypes->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setTitleText("Nombre");
    axisY->setLabelFormat("%d");
    chartTypes->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartTypesView->setChart(chartTypes);
    chartTypesView->setRubberBand(QChartView::RectangleRubberBand);

    auto *pieSeries = new QPieSeries();
    auto *sliceOnsite = pieSeries->append("On site", onsite);
    auto *sliceRemote = pieSeries->append("Remote", remote);
    auto *sliceHybrid = pieSeries->append("Hybrid", hybrid);
    pieSeries->setLabelsVisible(true);
    sliceOnsite->setLabel(QString("On site: %1").arg(onsite));
    sliceRemote->setLabel(QString("Remote: %1").arg(remote));
    sliceHybrid->setLabel(QString("Hybrid: %1").arg(hybrid));

    auto *chartLoc = new QChart();
    chartLoc->addSeries(pieSeries);
    chartLoc->setTitle("Répartition par localisation");
    chartLoc->setAnimationOptions(QChart::SeriesAnimations);

    chartLocView->setChart(chartLoc);
    chartLocView->setRubberBand(QChartView::RectangleRubberBand);
    */
}
void smartstudio::on_pushButton_79_clicked()
{
    /*<<<< 1) Vérifier s'il y a des données à exporter
    if (ui->tableWidget_4->rowCount() == 0) {
        QMessageBox::warning(this, "Attention",
                             "Il n'y a aucun matériel à exporter en PDF !");
        return;
    }

    // 2) Choix du fichier
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Enregistrer le PDF",
        "Materiel.pdf",
        "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    // 3) Préparer le document texte (mise en page)
    QTextDocument doc;
    QTextCursor cursor(&doc);

    // --- Titre ---
    QTextCharFormat titleFormat;
    titleFormat.setFontPointSize(18);
    titleFormat.setFontWeight(QFont::Bold);
    titleFormat.setFontFamilies(QStringList() << "Arial");

    QTextBlockFormat centerFormat;
    centerFormat.setAlignment(Qt::AlignCenter);
    cursor.setBlockFormat(centerFormat);
    cursor.insertText("Liste du matériel", titleFormat);
    cursor.insertBlock();

    // --- Date ---
    QTextCharFormat dateFormat;
    dateFormat.setFontPointSize(11);
    dateFormat.setFontFamilies(QStringList() << "Arial");

    QDate today = QDate::currentDate();
    cursor.insertText("Date : " + today.toString("dd/MM/yyyy"), dateFormat);
    cursor.insertBlock();
    cursor.insertBlock(); // ligne vide

    // Revenir à un alignement à gauche pour le tableau
    QTextBlockFormat leftFormat;
    leftFormat.setAlignment(Qt::AlignLeft);
    cursor.setBlockFormat(leftFormat);

    // 4) Création du tableau (nombre de lignes / colonnes)
    int rows = ui->tableWidget_4->rowCount() + 1; // +1 pour l'en-tête
    int cols = ui->tableWidget_4->columnCount();

    QTextTableFormat tableFormat;
    tableFormat.setBorder(0.8);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setCellPadding(6);
    tableFormat.setCellSpacing(0);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 90)); // 90% de la largeur

    QTextTable *table = cursor.insertTable(rows, cols, tableFormat);

    // Style pour l'en-tête
    QTextCharFormat headerFormat;
    headerFormat.setFontPointSize(11);
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontFamilies(QStringList() << "Arial");

    // Style pour le corps
    QTextCharFormat bodyFormat;
    bodyFormat.setFontPointSize(10);
    bodyFormat.setFontFamilies(QStringList() << "Arial");

    // 5) Remplir l'en-tête avec les titres de colonnes du QTableWidget
    for (int col = 0; col < cols; ++col) {
        QTextTableCell cell = table->cellAt(0, col);
        QTextCursor cellCursor = cell.firstCursorPosition();

        QString headerText;
        if (ui->tableWidget_4->horizontalHeaderItem(col)) {
            headerText = ui->tableWidget_4->horizontalHeaderItem(col)->text();
        }

        cellCursor.setCharFormat(headerFormat);
        cellCursor.insertText(headerText);
    }

    // 6) Remplir les lignes du tableau avec les données
    for (int row = 0; row < ui->tableWidget_4->rowCount(); ++row) {
        for (int col = 0; col < cols; ++col) {
            QTextTableCell cell = table->cellAt(row + 1, col);
            QTextCursor cellCursor = cell.firstCursorPosition();

            QString text;
            QTableWidgetItem *item = ui->tableWidget_4->item(row, col);
            if (item)
                text = item->text();

            cellCursor.setCharFormat(bodyFormat);
            cellCursor.insertText(text);
        }
    }

    // 7) Création du PDF
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Taille logique de page (A4 à 96 dpi approx.)
    const int dpi = 96;
    const qreal inch = 25.4;
    qreal pageWidthPx  = QPageSize(QPageSize::A4).size(QPageSize::Millimeter).width()
                        * dpi / inch;
    qreal pageHeightPx = QPageSize(QPageSize::A4).size(QPageSize::Millimeter).height()
                         * dpi / inch;

    doc.setPageSize(QSizeF(pageWidthPx, pageHeightPx));

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de créer le fichier PDF.");
        return;
    }

    // Mise à l'échelle pour utiliser toute la page
    QRectF pageRect(0, 0, pdfWriter.width(), pdfWriter.height());
    QRectF docRect(QPointF(0,0), doc.size());

    qreal sx = pageRect.width()  / docRect.width();
    qreal sy = pageRect.height() / docRect.height();
    qreal scale = qMin(sx, sy);          // garder les proportions

    painter.save();
    painter.translate(pageRect.left(), pageRect.top());
    painter.scale(scale, scale);
    doc.drawContents(&painter);
    painter.restore();

    painter.end();

    QMessageBox::information(this, "Succès",
                             "Le PDF a été généré avec succès.");*/
}


// Search functionality - FCT RECHERCHE
void smartstudio::on_l2_29_textChanged(const QString &text)
{
    QString searchText = text.trimmed();

    // If search is empty, show all rows
    if (searchText.isEmpty()) {
        for (int row = 0; row < ui->tableWidget_4->rowCount(); ++row) {
            ui->tableWidget_4->setRowHidden(row, false);
        }
        return;
    }

    // Filter table rows based on search text
    for (int row = 0; row < ui->tableWidget_4->rowCount(); ++row) {
        bool match = false;

        // Search in all columns
        for (int col = 0; col < ui->tableWidget_4->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget_4->item(row, col);
            if (item) {
                QString itemText = item->text();
                if (itemText.contains(searchText, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
        }

        // Show or hide row based on match
        ui->tableWidget_4->setRowHidden(row, !match);
    }
}

// FU - sort table by selected column
void smartstudio::on_comboBox_8_currentIndexChanged(int index)
{
    if (index < 0 || index >= ui->tableWidget_4->columnCount()) {
        return;
    }

    // Disable sorting temporarily to avoid recursion
    bool wasSortingEnabled = ui->tableWidget_4->isSortingEnabled();
    ui->tableWidget_4->setSortingEnabled(false);

    // Sort the table by the selected column (ascending)
    ui->tableWidget_4->sortItems(index, Qt::AscendingOrder);

    // Re-enable sorting if it was enabled
    ui->tableWidget_4->setSortingEnabled(wasSortingEnabled);
}
// Availability filter/search removed per request
void smartstudio::on_tab_commqr_clicked(const QModelIndex &index)
{
    QString id = ui->tableWidget_4->model()->data(index.sibling(index.row(), 0)).toString();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Attention", "ID matériel introuvable sur la ligne sélectionnée.");
        return;
    }

    Materielle::MaterielData d = materielle.getMaterielById(id);
    if (d.idMateriel.isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de récupérer les données du matériel."));
        return;
    }

    QString tout = d.etatMateriel;

   // QImage image = m_generator.generateQr(tout, 140, 2);
   // QPixmap pm = QPixmap::fromImage(image).scaled(ui->label_qr_preview_2->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
   // ui->label_qr_preview_2->setPixmap(pm);

    checkMaterialStateForAlert(d);
}

void smartstudio::checkMaterialStateForAlert(const Materielle::MaterielData &data)
{
    if (materialNeedsMaintenance(data)) {
        QMessageBox::warning(
            this,
            tr("Alerte de maintenance"),
            tr("Le matériel %1 (%2) nécessite une vérification/prévention.")
                .arg(data.idMateriel, data.etatMateriel));
        statusBar()->showMessage(
            tr("Maintenance requise pour %1").arg(data.idMateriel), 6000);
    } else {
        statusBar()->showMessage(
            tr("Matériel %1 en état normal").arg(data.idMateriel), 4000);
    }
}

bool smartstudio::materialNeedsMaintenance(const Materielle::MaterielData &data) const
{
    const QString state = data.etatMateriel.toLower();
    const QStringList indicators = {"maintenance", "réparation", "panne", "défaut", "casse", "cassé", "usé", "urgent"};

    for (const QString &indicator : indicators) {
        if (state.contains(indicator, Qt::CaseInsensitive)) {
            return true;
        }
    }

    return false;
}
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QClipboard>
#include <QApplication>

// Add these to your smartstudio.h header:
// #include <QPixmap>
// QPixmap generateQrCodeFromData(const QString &data, int size = 300);

void smartstudio::on_button_avail_generate_2_clicked()
{
    // Check if there are any rows in the table
    if (ui->tableWidget_4->rowCount() == 0) {
        QMessageBox::warning(this, tr("Aucune donnée"),
            tr("Le tableau est vide. Aucun QR code à générer."));
        return;
    }

    // DEBUG: Check selection state
    qDebug() << "Selection model has selection:" << ui->tableWidget_4->selectionModel()->hasSelection();
    qDebug() << "Selected rows count:" << ui->tableWidget_4->selectionModel()->selectedRows().count();
    qDebug() << "Selected items count:" << ui->tableWidget_4->selectedItems().count();

    // Get selected rows - FIXED APPROACH
    QList<int> selectedRows;

    // Method 1: Check for selected items (most reliable for QTableWidget)
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget_4->selectedItems();
    if (!selectedItems.isEmpty()) {
        // Get unique rows from selected items
        QSet<int> uniqueRows;
        for (QTableWidgetItem* item : selectedItems) {
            uniqueRows.insert(item->row());
        }
        selectedRows = uniqueRows.values();
        qDebug() << "Found" << selectedRows.size() << "unique rows from selected items";
    }

    // Method 2: If no items selected, check current row
    if (selectedRows.isEmpty()) {
        int currentRow = ui->tableWidget_4->currentRow();
        if (currentRow >= 0) {
            selectedRows.append(currentRow);
            qDebug() << "Using current row:" << currentRow;
        }
    }

    // Method 3: If still empty, check selection model's selected rows
    if (selectedRows.isEmpty()) {
        QModelIndexList selectedIndexes = ui->tableWidget_4->selectionModel()->selectedRows();
        for (const QModelIndex &index : selectedIndexes) {
            if (index.isValid()) {
                selectedRows.append(index.row());
            }
        }
        qDebug() << "Found" << selectedRows.size() << "rows from selection model";
    }

    // If still no selection, use all rows
    if (selectedRows.isEmpty()) {
        // Ask user if they want to generate QR codes for all rows
        int reply = QMessageBox::question(this, tr("Aucune sélection"),
            tr("Aucune ligne sélectionnée.\n\nVoulez-vous générer des QR codes pour TOUTES les lignes du tableau?"),
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int row = 0; row < ui->tableWidget_4->rowCount(); ++row) {
                selectedRows.append(row);
            }
            qDebug() << "Generating for all" << selectedRows.size() << "rows";
        } else {
            return; // User cancelled
        }
    }

    // Sort rows for better organization
    std::sort(selectedRows.begin(), selectedRows.end());

    qDebug() << "Final selected rows:" << selectedRows;

    // Ask for save directory
    QString saveDir = QFileDialog::getExistingDirectory(
        this,
        tr("Choisir un dossier pour enregistrer les QR codes"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
    );

    if (saveDir.isEmpty()) {
        return; // User cancelled
    }

    int successCount = 0;
    int errorCount = 0;

    // Generate QR code for each selected row
    for (int row : selectedRows) {
        // Gather data from the row
        QString qrData = generateQrDataFromRow(row);

        if (qrData.isEmpty()) {
            qDebug() << "Données vides pour la ligne" << row;
            errorCount++;
            continue;
        }

        // Generate QR code using Nayuki library
        QPixmap qrPixmap = generateQrCodeFromData(qrData, 300); // 300x300 pixels

        if (qrPixmap.isNull()) {
            qDebug() << "Échec de génération QR pour la ligne" << row;
            errorCount++;
            continue;
        }

        // Create filename from ID (column 0)
        QString id = "Unknown";
        QTableWidgetItem* idItem = ui->tableWidget_4->item(row, 0);
        if (idItem && !idItem->text().isEmpty()) {
            id = idItem->text().trimmed();
        } else {
            id = QString("Ligne_%1").arg(row + 1);
        }

        // Make filename safe
        QString safeId = id;
        safeId = safeId.replace(QRegularExpression("[<>:\"/\\\\|?*]"), "_");
        safeId = safeId.replace(QRegularExpression("\\s+"), "_");

        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        QString filename = QString("%1/QR_%2_%3.png")
                           .arg(saveDir)
                           .arg(safeId)
                           .arg(timestamp);

        // Save the image
        if (qrPixmap.save(filename, "PNG")) {
            successCount++;
            qDebug() << "QR code sauvegardé:" << filename;
        } else {
            qDebug() << "Échec de sauvegarde pour:" << filename;
            errorCount++;
        }
    }

    // Show result message
    if (successCount > 0) {
        QString message = tr("%1 QR code(s) généré(s) avec succès dans:\n%2")
                          .arg(successCount)
                          .arg(QDir::toNativeSeparators(saveDir));

        if (errorCount > 0) {
            message += tr("\n\n%1 échec(s) lors de la génération.").arg(errorCount);
        }

        if (QMessageBox::question(this, tr("Succès"),
            message + tr("\n\nVoulez-vous ouvrir le dossier?"),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(saveDir));
        }
    } else {
        QMessageBox::warning(this, tr("Échec"),
            tr("Aucun QR code n'a pu être généré.\n\nVérifiez que:\n1. Les données sont valides\n2. Le dossier de destination est accessible\n3. Le tableau contient des données"));
    }
}
// Helper function to extract data from a table row
QString smartstudio::generateQrDataFromRow(int row)
{
    if (row < 0 || row >= ui->tableWidget_4->rowCount()) {
        return QString();
    }

    QStringList data;
    for (int col = 0; col < ui->tableWidget_4->columnCount(); ++col) {
        QTableWidgetItem *item = ui->tableWidget_4->item(row, col);
        QString header = ui->tableWidget_4->horizontalHeaderItem(col) ?
                         ui->tableWidget_4->horizontalHeaderItem(col)->text() :
                         QString("Col%1").arg(col + 1);
        QString value = item ? item->text() : "";

        data << QString("%1: %2").arg(header, value);
    }

    // Add timestamp
    data << QString("Généré le: %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));

    return data.join("\n");
}

// Function to generate QR code image using Nayuki library
QPixmap smartstudio::generateQrCodeFromData(const QString &data, int size)
{
    try {
        // Convert QString to std::string for Nayuki library
        std::string text = data.toStdString();

        // Generate QR Code with medium error correction
        qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);

        // Get QR code size (modules count)
        int qrSize = qr.getSize();

        // Create QPixmap with white background
        QPixmap pixmap(size, size);
        pixmap.fill(Qt::white);

        // Create painter
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);

        // Calculate scale factor
        int border = 4; // Quiet zone border
        double scale = static_cast<double>(size - 2 * border) / qrSize;

        // Draw each module
        painter.setBrush(Qt::black);
        painter.setPen(Qt::NoPen);

        for (int y = 0; y < qrSize; y++) {
            for (int x = 0; x < qrSize; x++) {
                if (qr.getModule(x, y)) {
                    QRectF rect(
                        border + x * scale,
                        border + y * scale,
                        scale,
                        scale
                    );
                    painter.drawRect(rect);
                }
            }
        }

        // Add optional border
        painter.setPen(QPen(Qt::lightGray, 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0, 0, size - 1, size - 1);

        painter.end();
        return pixmap;

    } catch (const std::exception &e) {
        qDebug() << "Erreur génération QR:" << e.what();
        return QPixmap();
    }
}

// Alternative: Generate QR code and display in a label
QPixmap smartstudio::generateAndDisplayQrCode(const QString &data, QLabel *targetLabel, int size)
{
    QPixmap qrPixmap = generateQrCodeFromData(data, size);

    if (!qrPixmap.isNull() && targetLabel) {
        targetLabel->setPixmap(qrPixmap.scaled(
            targetLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
        targetLabel->setAlignment(Qt::AlignCenter);
    }

    return qrPixmap;
}

// For direct copying to clipboard
void smartstudio::copyQrCodeToClipboard(const QString &data)
{
    QPixmap qrPixmap = generateQrCodeFromData(data, 300);
    if (!qrPixmap.isNull()) {
        QApplication::clipboard()->setPixmap(qrPixmap);
        QMessageBox::information(this, tr("QR Code"),
            tr("QR Code copié dans le presse-papier."));
    }
}
