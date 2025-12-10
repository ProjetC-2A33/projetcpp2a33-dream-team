#ifndef SMARTSTUDIO_H
#define SMARTSTUDIO_H
#include "employe.h"
#include "arduino.h"

#include <QMainWindow>
#include <QCalendarWidget>
#include "employeepiewidget.h"
#include <QMainWindow>
#include <QList>
#include "sponsor.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>


#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "materielle.h"
//#include <QrCodeGenerator.h>

#include <QtCharts/QChartView>



QT_BEGIN_NAMESPACE
namespace Ui {
class smartstudio;
}
QT_END_NAMESPACE

class smartstudio : public QMainWindow
{
    Q_OBJECT
private:
    Employe Etmp; // objet global pour manipuler les employés
    EmployeePieWidget *m_employeePieWidget;
    QCalendarWidget *m_employeeCalendar;
    void fillFieldsFromTableRow(int row); // Helper pour remplir les champs depuis une ligne du tableau
    void updateEmployeeTypeChart();

public:
    smartstudio(QWidget *parent = nullptr);
    ~smartstudio();

private slots:
    void on_pushButton_82_clicked();

    void on_pushButton_clicked();
    void on_pushButton_84_clicked();

    void on_pushButton_99_clicked();

    void on_pushButton_85_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_86_clicked();

    void on_pushButton_113_clicked();

    void on_pushButton_101_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_80_clicked();

    void on_pushButton_81_clicked();

    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_comboBox_10_currentIndexChanged(int index);
    void on_l2_59_editingFinished();
    void on_pushButton_114_clicked();
    void on_pushButton_2_clicked();



    void on_pushButton_83_clicked();





  void on_pushButton_3_clicked();




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
  void on_tab_commqr_clicked(const QModelIndex &index);
  void checkMaterialStateForAlert(const Materielle::MaterielData &data);
  bool materialNeedsMaintenance(const Materielle::MaterielData &data) const;


  void on_button_avail_generate_2_clicked();
  QString generateQrDataFromRow(int row);


  QPixmap generateAndDisplayQrCode(const QString &data, QLabel *targetLabel, int size);
  void copyQrCodeToClipboard(const QString &data);
  QPixmap generateQrCodeFromData(const QString &data, int size);
private:
    Ui::smartstudio *ui;
    void Add_New_Client_Connection(QTcpSocket *socket);

    // Détecteur de flamme
    FlameDetector *flameDetector;
    void setupFlameDetector();
    
    // Capteur de température
    TemperatureSensor *temperatureSensor;
    void setupTemperatureSensor();
    QLabel *tempLabel;
    QLabel *humLabel;

    void chargerSponsors();



    Materielle materielle; // Instance de la classe Materielle pour les opérations CRUD
    void loadTableData(); // Load data from database
    void clearInputs(); // Clear input fields
    Materielle::MaterielData getDataFromUI(); // Get data from UI fields
    void updateCharts();

   // QChartView *chartTypesView = nullptr;
    //QChartView *chartLocView = nullptr;
   // QrCodeGenerator m_generator;

};
#endif // SMARTSTUDIO_H
