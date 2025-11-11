#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    ui->label_platform->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event); // Call parent class implementation

    // Create object
    createurcrud crc;
    ui->tableView->setModel(crc.afficher());
}

// ============================================================
// VALIDATION HELPERS
// ============================================================

bool MainWindow::isValidName(const QString &s)
{
    static const QRegularExpression re("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]+$");
    return !s.isEmpty() && re.match(s).hasMatch();
}

bool MainWindow::isValidEmail(const QString &s)
{
    static const QRegularExpression re(
        R"(^[\w\.\-]{1,64}@[\w\-]{1,255}(\.[a-zA-Z]{2,})+$)");
    return re.match(s).hasMatch();
}

bool MainWindow::isValidPhone(const QString &s)
{
    static const QRegularExpression re("^[0-9]{8}$");
    return re.match(s).hasMatch();
}

bool MainWindow::isValidNumber(const QString &s)
{
    static const QRegularExpression re("^[0-9]+$");
    return !s.isEmpty() && re.match(s).hasMatch();
}

// ============================================================
// ADD CREATOR (Button 60)
// ============================================================

void MainWindow::on_pushButton_60_clicked()
{
    QString idc       = ui->lineEdit_idcreateur->text().trimmed();
    QString nom       = ui->lineEdut_nom->text().trimmed();
    QString prenom    = ui->lineEdit_prenom->text().trimmed();
    QString email     = ui->LineEdit_email->text().trimmed();
    QString numtel    = ui->LineEdit_numtel->text().trimmed();
    QString nbStr     = ui->LineEdit_nbabonnes->text().trimmed();
    QString platforme = ui->label_platform->text();

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
    ui->label_platform->setText("Plateforme");
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_6->setChecked(false);

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

void MainWindow::on_checkBox_2_clicked() { ui->label_platform->setText("Instagram"); }
void MainWindow::on_checkBox_clicked()   { ui->label_platform->setText("Tiktok"); }
void MainWindow::on_checkBox_3_clicked() { ui->label_platform->setText("Youtube"); }
void MainWindow::on_checkBox_6_clicked() { ui->label_platform->setText("Autres"); }

// ============================================================
// SELECT ROW → FILL FORM
// ============================================================

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) return;

    QString idc       = model->data(model->index(row, 0)).toString();
    QString nom       = model->data(model->index(row, 1)).toString();
    QString prenom    = model->data(model->index(row, 2)).toString();
    QString email     = model->data(model->index(row, 3)).toString();
    QString numtel    = model->data(model->index(row, 5)).toString();
    int nbabonnes     = model->data(model->index(row, 6)).toInt();

    ui->lineEdit_idcreateur->setText(idc);
    ui->lineEdut_nom->setText(nom);
    ui->lineEdit_prenom->setText(prenom);
    ui->LineEdit_email->setText(email);
    ui->LineEdit_numtel->setText(numtel);
    ui->LineEdit_nbabonnes->setText(QString::number(nbabonnes));

    // Sync checkboxes
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_6->setChecked(false);
}

// ============================================================
// EDIT CREATOR (Button 59)
// ============================================================

void MainWindow::on_pushButton_59_clicked()
{
    QString idc       = ui->lineEdit_idcreateur->text().trimmed();
    QString nom       = ui->lineEdut_nom->text().trimmed();
    QString prenom    = ui->lineEdit_prenom->text().trimmed();
    QString email     = ui->LineEdit_email->text().trimmed();
    QString numtel    = ui->LineEdit_numtel->text().trimmed();
    QString nbStr     = ui->LineEdit_nbabonnes->text().trimmed();
    QString platforme = ui->label_platform->text();

    if (idc.isEmpty())
    {
        QMessageBox::warning(this, tr("Sélection requise"),
                             tr("Veuillez sélectionner un créateur dans le tableau."));
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        numtel.isEmpty() || nbStr.isEmpty() || platforme.isEmpty())
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

// ============================================================
// DELETE ON DOUBLE-CLICK
// ============================================================

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
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

void MainWindow::clearForm()
{
    ui->lineEdit_idcreateur->clear();
    ui->lineEdut_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->LineEdit_email->clear();
    ui->LineEdit_numtel->clear();
    ui->LineEdit_nbabonnes->clear();
    ui->label_platform->setText("Plateforme");
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_6->setChecked(false);
}
