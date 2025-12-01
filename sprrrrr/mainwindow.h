#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void on_Openhercules_clicked();

private:
    void Add_New_Client_Connection(QTcpSocket *socket);
    Ui::MainWindow *ui;
    QList<Sponsor> sponsors;

    void chargerSponsors();
    bool controleSaisie();
    QTcpServer *TCP_Server;
    QList<QTcpSocket*>   Client_Connection_List;
    QNetworkAccessManager *networkManager;
    QString infobipApiKey = "a7aecf6331ab45c15b855f0d70651ddb-00bafb3f-796b-4691-a900-e0d1be0540bf";
    QString infobipBaseUrl = "https://nmvl3e.api.infobip.com";

};

#endif // MAINWINDOW_H
