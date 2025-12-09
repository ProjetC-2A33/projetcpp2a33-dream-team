#ifndef pagesponsor_H
#define pagesponsor_H

#include <QDialog>
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
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui { class pagesponsor; }
QT_END_NAMESPACE

class pagesponsor : public QDialog
{
    Q_OBJECT

public:
    explicit pagesponsor(QWidget *parent = nullptr);
    ~pagesponsor();

private slots:
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
  void sendAutomaticSMS(const QString &phoneNumber, const QString &message);
  void on_Openhercules_clicked();
  bool isValidPhoneNumber(const QString &phone);





void on_pushButton_62_clicked();


void on_startButton_clicked();
void on_stopButton_clicked();


private:
    void Add_New_Client_Connection(QTcpSocket *socket);
    Ui::pagesponsor *ui;
    QList<Sponsor> sponsors;

    void chargerSponsors();
    bool controleSaisie();
    QTcpServer *TCP_Server;
    QList<QTcpSocket*>   Client_Connection_List;
    QNetworkAccessManager *networkManager;
    QString infobipApiKey = "2f7a7cd6ff27df26631851d4386b5575-d4645e64-39c3-469c-aafd-26a0fe1162c4 ";
    QString infobipBaseUrl = "https://nmvl3e.api.infobip.com";
      Arduino A;

   };

                            #endif // pagesponsor_H
