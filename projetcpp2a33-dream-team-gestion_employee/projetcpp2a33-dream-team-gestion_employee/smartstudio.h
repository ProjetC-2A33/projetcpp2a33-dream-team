#ifndef SMARTSTUDIO_H
#define SMARTSTUDIO_H
#include "employe.h"

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



    void ajouterSponsor();
    void modifierSponsor();
    void supprimerSponsor();

    void appliquerRecherche(const QString &texte);
    void exporterPdf();
    void afficherStatistiquesCirculaire();
    void newConnection();
    void Read_Data_From_Socket();
    void on_pushButton_Send_Message_Type_clicked();
    void envoyerSMS(const QString& telephone, const QString& message);
       void onSMSReply(QNetworkReply *reply);
       void on_pushButton_envoyerSMS_clicked();

 void appliquerTri();
  void afficherSponsors();
  void on_Openhercules_clicked();


  void on_pushButton_3_clicked();

private:
    Ui::smartstudio *ui;
    void Add_New_Client_Connection(QTcpSocket *socket);

    QList<Sponsor> sponsors;

    void chargerSponsors();
    bool controleSaisie();
    QTcpServer *TCP_Server;
    QList<QTcpSocket*>   Client_Connection_List;
    QNetworkAccessManager *networkManager;
    QString infobipApiKey = "2f7a7cd6ff27df26631851d4386b5575-d4645e64-39c3-469c-aafd-26a0fe1162c4 ";
    QString infobipBaseUrl = "https://nmvl3e.api.infobip.com";

};
#endif // SMARTSTUDIO_H
