#ifndef SMARTSTUDIO_H
#define SMARTSTUDIO_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "materielle.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class smartstudio;
}
QT_END_NAMESPACE

class smartstudio : public QMainWindow
{
    Q_OBJECT

public:
    smartstudio(QWidget *parent = nullptr);
    ~smartstudio();

private slots:




    // CRUD operations for materiel
    void on_pushButton_74_clicked(); // Ajouter
    void on_pushButton_73_clicked(); // Annuler
    void on_tableWidget_4_itemSelectionChanged(); // Selection changed
    void on_pushButton_75_clicked(); // Modifier (if exists)
    void on_pushButton_76_clicked(); // Supprimer (if exists)
    void refreshTable(); // Refresh table data
    void on_pushButton_79_clicked();
    // Search and Sort functionality
    void on_l2_29_textChanged(const QString &text); // Search filter
    void on_comboBox_8_currentIndexChanged(int index); // Sort by column

private:
    Ui::smartstudio *ui;
    Materielle materielle; // Instance de la classe Materielle pour les opérations CRUD
    void loadTableData(); // Load data from database
    void clearInputs(); // Clear input fields
    Materielle::MaterielData getDataFromUI(); // Get data from UI fields
};




#endif // SMARTSTUDIO_H
