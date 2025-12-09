/********************************************************************************
** Form generated from reading UI file 'pagesponsor.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGESPONSOR_H
#define UI_PAGESPONSOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pagesponsor
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *lineEdit_chercher;
    QLabel *label_6;
    QComboBox *comboBox_statut;
    QLabel *label_2;
    QDateEdit *dateEdit_fin;
    QListView *listView;
    QLineEdit *lineEdit_nom;
    QLineEdit *lineEdit_secteur;
    QLabel *label;
    QPushButton *pushButton_modifier;
    QComboBox *comboBox_type;
    QLabel *label_9;
    QLabel *label_7;
    QLineEdit *lineEdit_contrat;
    QLineEdit *lineEdit_budget;
    QPushButton *pushButton_supprimer;
    QLabel *label_10;
    QPushButton *pushButton_ajouter;
    QLabel *label_4;
    QLabel *label_139;
    QLineEdit *lineEdit_email;
    QPushButton *trier_sponsor;
    QLabel *label_8;
    QLabel *label_5;
    QDateEdit *dateEdit_debut;
    QTableWidget *tableWidget_sponsors;
    QListView *liststat;
    QPushButton *pushButton_pdf;
    QLineEdit *lineEdit_tel;
    QPushButton *stat;
    QLabel *label_3;
    QPushButton *pushButton_afficher;
    QPushButton *pushButton_envoyerSMS;
    QComboBox *comboBox_tri;
    QPushButton *pushButton_62;
    QPushButton *moteur;
    QWidget *tab_2;
    QPushButton *pushButton_Send_Message_Type;
    QComboBox *comboBox_Client_list;
    QLineEdit *lineEdit_Message_For_Client;
    QComboBox *comboBox_Send_Message_Type;
    QTextEdit *textEdit_Client_Messages;
    QPushButton *Openhercules;

    void setupUi(QDialog *pagesponsor)
    {
        if (pagesponsor->objectName().isEmpty())
            pagesponsor->setObjectName("pagesponsor");
        pagesponsor->resize(1581, 952);
        tabWidget = new QTabWidget(pagesponsor);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(-10, 0, 1741, 911));
        tab = new QWidget();
        tab->setObjectName("tab");
        lineEdit_chercher = new QLineEdit(tab);
        lineEdit_chercher->setObjectName("lineEdit_chercher");
        lineEdit_chercher->setGeometry(QRect(1180, 260, 131, 41));
        lineEdit_chercher->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
"\n"
""));
        label_6 = new QLabel(tab);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(40, 360, 201, 41));
        label_6->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        comboBox_statut = new QComboBox(tab);
        comboBox_statut->setObjectName("comboBox_statut");
        comboBox_statut->setGeometry(QRect(260, 360, 161, 41));
        comboBox_statut->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 180, 201, 41));
        label_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        dateEdit_fin = new QDateEdit(tab);
        dateEdit_fin->setObjectName("dateEdit_fin");
        dateEdit_fin->setGeometry(QRect(260, 600, 161, 41));
        dateEdit_fin->setStyleSheet(QString::fromUtf8(""));
        listView = new QListView(tab);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(20, 0, 411, 751));
        listView->setStyleSheet(QString::fromUtf8("QListView {\n"
"    background-color: #F6CFFC;        /* m\303\252me fond rose clair */\n"
"    border: 2px solid #1E1E5A;        /* m\303\252me bordure bleu fonc\303\251 */\n"
"    border-radius: 10px;              /* arrondi identique */\n"
"    font-family: \"Arial\";\n"
"    font-size: 18px;\n"
"    color: #0A0A2A;                    /* m\303\252me couleur de texte */\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QListView::item {\n"
"    background-color: transparent;\n"
"    padding: 6px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QListView::item:selected {\n"
"    background-color: #1E1E5A;        /* m\303\252me bleu fonc\303\251 pour la s\303\251lection */\n"
"    color: white;\n"
"}\n"
"\n"
"QListView::item:hover {\n"
"    background-color: #B38DFF;        /* violet clair sur hover */\n"
"    color: white;\n"
"}\n"
""));
        lineEdit_nom = new QLineEdit(tab);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(260, 70, 161, 41));
        lineEdit_nom->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        lineEdit_secteur = new QLineEdit(tab);
        lineEdit_secteur->setObjectName("lineEdit_secteur");
        lineEdit_secteur->setGeometry(QRect(260, 120, 161, 41));
        lineEdit_secteur->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 60, 211, 41));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        pushButton_modifier = new QPushButton(tab);
        pushButton_modifier->setObjectName("pushButton_modifier");
        pushButton_modifier->setGeometry(QRect(300, 660, 122, 41));
        pushButton_modifier->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        comboBox_type = new QComboBox(tab);
        comboBox_type->setObjectName("comboBox_type");
        comboBox_type->setGeometry(QRect(260, 300, 161, 41));
        comboBox_type->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label_9 = new QLabel(tab);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(40, 540, 191, 41));
        label_9->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        label_7 = new QLabel(tab);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(40, 420, 171, 41));
        label_7->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        lineEdit_contrat = new QLineEdit(tab);
        lineEdit_contrat->setObjectName("lineEdit_contrat");
        lineEdit_contrat->setGeometry(QRect(260, 240, 161, 41));
        lineEdit_contrat->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        lineEdit_budget = new QLineEdit(tab);
        lineEdit_budget->setObjectName("lineEdit_budget");
        lineEdit_budget->setGeometry(QRect(260, 180, 161, 41));
        lineEdit_budget->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        pushButton_supprimer = new QPushButton(tab);
        pushButton_supprimer->setObjectName("pushButton_supprimer");
        pushButton_supprimer->setGeometry(QRect(160, 660, 122, 41));
        pushButton_supprimer->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label_10 = new QLabel(tab);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(40, 600, 191, 41));
        label_10->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        pushButton_ajouter = new QPushButton(tab);
        pushButton_ajouter->setObjectName("pushButton_ajouter");
        pushButton_ajouter->setGeometry(QRect(30, 660, 122, 41));
        pushButton_ajouter->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label_4 = new QLabel(tab);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 240, 201, 41));
        label_4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        label_139 = new QLabel(tab);
        label_139->setObjectName("label_139");
        label_139->setGeometry(QRect(1000, 260, 171, 41));
        label_139->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        lineEdit_email = new QLineEdit(tab);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setGeometry(QRect(260, 420, 161, 41));
        lineEdit_email->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        trier_sponsor = new QPushButton(tab);
        trier_sponsor->setObjectName("trier_sponsor");
        trier_sponsor->setGeometry(QRect(462, 260, 111, 41));
        trier_sponsor->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label_8 = new QLabel(tab);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 480, 191, 41));
        label_8->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(40, 300, 181, 41));
        label_5->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        dateEdit_debut = new QDateEdit(tab);
        dateEdit_debut->setObjectName("dateEdit_debut");
        dateEdit_debut->setGeometry(QRect(260, 540, 161, 41));
        dateEdit_debut->setStyleSheet(QString::fromUtf8(""));
        tableWidget_sponsors = new QTableWidget(tab);
        tableWidget_sponsors->setObjectName("tableWidget_sponsors");
        tableWidget_sponsors->setGeometry(QRect(430, 310, 971, 192));
        tableWidget_sponsors->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #DAB1DA;\n"
"    color: white;\n"
"    border-radius: 15px;\n"
"    padding: 10px;\n"
"    font-size: 13px;\n"
"    border: none;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #5555FF;\n"
"    color: white;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #341539;\n"
"    color: white;\n"
"    padding: 10px;\n"
"    border: none;\n"
"}"));
        liststat = new QListView(tab);
        liststat->setObjectName("liststat");
        liststat->setGeometry(QRect(550, 520, 631, 341));
        pushButton_pdf = new QPushButton(tab);
        pushButton_pdf->setObjectName("pushButton_pdf");
        pushButton_pdf->setGeometry(QRect(1200, 530, 281, 51));
        pushButton_pdf->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icones/addition.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_pdf->setIcon(icon);
        pushButton_pdf->setIconSize(QSize(20, 20));
        lineEdit_tel = new QLineEdit(tab);
        lineEdit_tel->setObjectName("lineEdit_tel");
        lineEdit_tel->setGeometry(QRect(260, 480, 161, 41));
        lineEdit_tel->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        stat = new QPushButton(tab);
        stat->setObjectName("stat");
        stat->setGeometry(QRect(1200, 630, 181, 61));
        stat->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 120, 201, 41));
        label_3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        pushButton_afficher = new QPushButton(tab);
        pushButton_afficher->setObjectName("pushButton_afficher");
        pushButton_afficher->setGeometry(QRect(160, 10, 122, 41));
        pushButton_afficher->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #4a69bd;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 10px 16px;\n"
"    font-weight: bold;\n"
"    font-size: 10pt;\n"
"    min-width: 90px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3a59ad;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #2a499d;\n"
"}"));
        pushButton_envoyerSMS = new QPushButton(tab);
        pushButton_envoyerSMS->setObjectName("pushButton_envoyerSMS");
        pushButton_envoyerSMS->setGeometry(QRect(450, 550, 93, 28));
        comboBox_tri = new QComboBox(tab);
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->setObjectName("comboBox_tri");
        comboBox_tri->setGeometry(QRect(600, 270, 111, 31));
        pushButton_62 = new QPushButton(tab);
        pushButton_62->setObjectName("pushButton_62");
        pushButton_62->setGeometry(QRect(1210, 760, 171, 51));
        pushButton_62->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    font-family: \"Arial Black\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1E1E5A;\n"
"}\n"
""));
        pushButton_62->setIcon(icon);
        pushButton_62->setIconSize(QSize(20, 20));
        moteur = new QPushButton(tab);
        moteur->setObjectName("moteur");
        moteur->setGeometry(QRect(780, 260, 121, 41));
        tabWidget->addTab(tab, QString());
        pushButton_62->raise();
        listView->raise();
        lineEdit_chercher->raise();
        label_6->raise();
        comboBox_statut->raise();
        label_2->raise();
        dateEdit_fin->raise();
        lineEdit_nom->raise();
        lineEdit_secteur->raise();
        label->raise();
        pushButton_modifier->raise();
        comboBox_type->raise();
        label_9->raise();
        label_7->raise();
        lineEdit_contrat->raise();
        lineEdit_budget->raise();
        pushButton_supprimer->raise();
        label_10->raise();
        pushButton_ajouter->raise();
        label_4->raise();
        label_139->raise();
        lineEdit_email->raise();
        trier_sponsor->raise();
        label_8->raise();
        label_5->raise();
        dateEdit_debut->raise();
        tableWidget_sponsors->raise();
        liststat->raise();
        pushButton_pdf->raise();
        lineEdit_tel->raise();
        stat->raise();
        label_3->raise();
        pushButton_afficher->raise();
        pushButton_envoyerSMS->raise();
        comboBox_tri->raise();
        moteur->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        pushButton_Send_Message_Type = new QPushButton(tab_2);
        pushButton_Send_Message_Type->setObjectName("pushButton_Send_Message_Type");
        pushButton_Send_Message_Type->setGeometry(QRect(1040, 670, 181, 41));
        comboBox_Client_list = new QComboBox(tab_2);
        comboBox_Client_list->setObjectName("comboBox_Client_list");
        comboBox_Client_list->setGeometry(QRect(40, 670, 351, 41));
        lineEdit_Message_For_Client = new QLineEdit(tab_2);
        lineEdit_Message_For_Client->setObjectName("lineEdit_Message_For_Client");
        lineEdit_Message_For_Client->setGeometry(QRect(780, 670, 261, 41));
        comboBox_Send_Message_Type = new QComboBox(tab_2);
        comboBox_Send_Message_Type->addItem(QString());
        comboBox_Send_Message_Type->addItem(QString());
        comboBox_Send_Message_Type->setObjectName("comboBox_Send_Message_Type");
        comboBox_Send_Message_Type->setGeometry(QRect(430, 670, 341, 41));
        textEdit_Client_Messages = new QTextEdit(tab_2);
        textEdit_Client_Messages->setObjectName("textEdit_Client_Messages");
        textEdit_Client_Messages->setGeometry(QRect(40, 0, 1171, 661));
        Openhercules = new QPushButton(tab_2);
        Openhercules->setObjectName("Openhercules");
        Openhercules->setGeometry(QRect(350, 750, 93, 28));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(pagesponsor);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(pagesponsor);
    } // setupUi

    void retranslateUi(QDialog *pagesponsor)
    {
        pagesponsor->setWindowTitle(QCoreApplication::translate("pagesponsor", "Dialog", nullptr));
        label_6->setText(QCoreApplication::translate("pagesponsor", "Statut (actif/inactif) :", nullptr));
        label_2->setText(QCoreApplication::translate("pagesponsor", "Montant / Budget :", nullptr));
        label->setText(QCoreApplication::translate("pagesponsor", "Nom du sponsor :", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("pagesponsor", "Modifier", nullptr));
        label_9->setText(QCoreApplication::translate("pagesponsor", "Date d\303\251but contrat :", nullptr));
        label_7->setText(QCoreApplication::translate("pagesponsor", "Adresse e-mail :", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("pagesponsor", "Supprimer", nullptr));
        label_10->setText(QCoreApplication::translate("pagesponsor", "Date fin contrat :", nullptr));
        pushButton_ajouter->setText(QCoreApplication::translate("pagesponsor", "Ajouter", nullptr));
        label_4->setText(QCoreApplication::translate("pagesponsor", "Contrat associ\303\251 :", nullptr));
        label_139->setText(QCoreApplication::translate("pagesponsor", "Cherche sponsor :", nullptr));
        trier_sponsor->setText(QCoreApplication::translate("pagesponsor", "trier_selon:", nullptr));
        label_8->setText(QCoreApplication::translate("pagesponsor", "T\303\251l\303\251phone :", nullptr));
        label_5->setText(QCoreApplication::translate("pagesponsor", "Partenariat :", nullptr));
        pushButton_pdf->setText(QCoreApplication::translate("pagesponsor", "Exportation format PDF sponsor ", nullptr));
        stat->setText(QCoreApplication::translate("pagesponsor", "stat_partenariat_sponsor", nullptr));
        label_3->setText(QCoreApplication::translate("pagesponsor", "Secteur d\342\200\231activit\303\251 :", nullptr));
        pushButton_afficher->setText(QCoreApplication::translate("pagesponsor", "Afficher", nullptr));
        pushButton_envoyerSMS->setText(QCoreApplication::translate("pagesponsor", "send sms", nullptr));
        comboBox_tri->setItemText(0, QCoreApplication::translate("pagesponsor", "Budget", nullptr));
        comboBox_tri->setItemText(1, QCoreApplication::translate("pagesponsor", "Nom", nullptr));

        pushButton_62->setText(QCoreApplication::translate("pagesponsor", "quitter", nullptr));
        moteur->setText(QCoreApplication::translate("pagesponsor", "start moteur", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("pagesponsor", "Tab 1", nullptr));
        pushButton_Send_Message_Type->setText(QCoreApplication::translate("pagesponsor", "Send Message", nullptr));
        comboBox_Send_Message_Type->setItemText(0, QCoreApplication::translate("pagesponsor", "All", nullptr));
        comboBox_Send_Message_Type->setItemText(1, QCoreApplication::translate("pagesponsor", "Individual", nullptr));

        Openhercules->setText(QCoreApplication::translate("pagesponsor", "Hercules", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("pagesponsor", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pagesponsor: public Ui_pagesponsor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGESPONSOR_H
