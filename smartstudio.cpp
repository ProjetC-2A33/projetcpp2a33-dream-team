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




smartstudio::smartstudio(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::smartstudio)
{
    ui->setupUi(this);
    
    // Connect the selection changed signal manually if needed
    connect(ui->tableWidget_4, &QTableWidget::itemSelectionChanged, 
            this, &smartstudio::on_tableWidget_4_itemSelectionChanged);
    
    // Connect search and sort signals
    connect(ui->l2_29, &QLineEdit::textChanged, 
            this, &smartstudio::on_l2_29_textChanged);
    
    // Load data after UI is fully initialized (delay to ensure database connection is ready)
    QTimer::singleShot(200, this, [this]() {
        loadTableData();
    });
}

smartstudio::~smartstudio()
{
    delete ui;
}



// Load data from database into tableWidget_4
void smartstudio::loadTableData()
{
    materielle.lire(ui->tableWidget_4, this);
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
void smartstudio::on_pushButton_79_clicked()
{
    // 1) Vérifier s'il y a des données à exporter
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
    titleFormat.setFontFamily("Arial");

    QTextBlockFormat centerFormat;
    centerFormat.setAlignment(Qt::AlignCenter);
    cursor.setBlockFormat(centerFormat);
    cursor.insertText("Liste du matériel", titleFormat);
    cursor.insertBlock();

    // --- Date ---
    QTextCharFormat dateFormat;
    dateFormat.setFontPointSize(11);
    dateFormat.setFontFamily("Arial");

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
    headerFormat.setFontFamily("Arial");

    // Style pour le corps
    QTextCharFormat bodyFormat;
    bodyFormat.setFontPointSize(10);
    bodyFormat.setFontFamily("Arial");

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
                             "Le PDF a été généré avec succès.");
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
