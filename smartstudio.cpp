#include "smartstudio.h"
#include "ui_smartstudio.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include <QTimer>
#include <QVariant>

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
    connect(ui->comboBox_8, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &smartstudio::on_comboBox_8_currentIndexChanged);
    
    // Load data after UI is fully initialized (delay to ensure database connection is ready)
    QTimer::singleShot(200, this, [this]() {
        loadTableData();
    });
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
    ui->stackedWidget->setCurrentIndex(1);
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
    ui->l2_26->clear();
    ui->l2_27->clear();
    ui->l2_28->clear();
    ui->comboBox_7->setCurrentIndex(0);
    ui->dateTimeEdit_15->setDate(QDate::currentDate());
    
    // Clear checkboxes
    ui->checkBox_13->setChecked(false);
    ui->checkBox_14->setChecked(false);
    ui->checkBox_15->setChecked(false);
    ui->checkBox_16->setChecked(false);
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

// Search functionality - filter table based on search text
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

// Sort functionality - sort table by selected column
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

