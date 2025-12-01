/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtQuickWidgets/QQuickWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox_2;
    QComboBox *comboBox_2;
    QGroupBox *groupBox_12;
    QLabel *label_129;
    QLabel *label_130;
    QLabel *label_131;
    QLineEdit *lineEdit_idcreateur;
    QLineEdit *lineEdut_nom;
    QLabel *label_135;
    QLineEdit *lineEdit_prenom;
    QLabel *label_136;
    QLabel *label_137;
    QLineEdit *LineEdit_nbabonnes;
    QPushButton *pushButton_59;
    QGroupBox *groupBox;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_6;
    QPushButton *pushButton_60;
    QLineEdit *LineEdit_email;
    QLabel *label_132;
    QLineEdit *LineEdit_numtel;
    QLabel *label_138;
    QLabel *label_139;
    QLineEdit *l2_15;
    QPushButton *pushButton_61;
    QLabel *label;
    QPushButton *pushButton_62;
    QPushButton *pushButton_63;
    QPushButton *pushButton_64;
    QPushButton *pushButton_65;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton_68;
    QPushButton *pushButton_69;
    QTableView *tableView;
    QQuickWidget *quickWidgetstat;
    QPushButton *pushButton_stat;
    QPushButton *pb_qrcode;
    QPushButton *pb_calendar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1251, 730);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 0, 1441, 701));
        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(530, 180, 191, 28));
        groupBox_12 = new QGroupBox(groupBox_2);
        groupBox_12->setObjectName("groupBox_12");
        groupBox_12->setGeometry(QRect(10, 60, 411, 601));
        groupBox_12->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    font-family: \"Arial\";\n"
"    font-size: 25px;\n"
"    font-weight: bold;\n"
"    color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    background-color: #F6CFFC;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 5px;\n"
"    background-color: #1E1E5A;\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        label_129 = new QLabel(groupBox_12);
        label_129->setObjectName("label_129");
        label_129->setGeometry(QRect(10, 50, 211, 20));
        label_129->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        label_130 = new QLabel(groupBox_12);
        label_130->setObjectName("label_130");
        label_130->setGeometry(QRect(10, 100, 201, 20));
        label_130->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_131 = new QLabel(groupBox_12);
        label_131->setObjectName("label_131");
        label_131->setGeometry(QRect(10, 240, 161, 31));
        label_131->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        lineEdit_idcreateur = new QLineEdit(groupBox_12);
        lineEdit_idcreateur->setObjectName("lineEdit_idcreateur");
        lineEdit_idcreateur->setGeometry(QRect(180, 50, 191, 31));
        lineEdit_idcreateur->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        lineEdut_nom = new QLineEdit(groupBox_12);
        lineEdut_nom->setObjectName("lineEdut_nom");
        lineEdut_nom->setGeometry(QRect(180, 90, 191, 31));
        lineEdut_nom->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_135 = new QLabel(groupBox_12);
        label_135->setObjectName("label_135");
        label_135->setGeometry(QRect(10, 270, 181, 41));
        label_135->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        lineEdit_prenom = new QLineEdit(groupBox_12);
        lineEdit_prenom->setObjectName("lineEdit_prenom");
        lineEdit_prenom->setGeometry(QRect(180, 140, 191, 31));
        lineEdit_prenom->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_136 = new QLabel(groupBox_12);
        label_136->setObjectName("label_136");
        label_136->setGeometry(QRect(10, 200, 131, 20));
        label_136->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_137 = new QLabel(groupBox_12);
        label_137->setObjectName("label_137");
        label_137->setGeometry(QRect(10, 140, 201, 41));
        label_137->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        LineEdit_nbabonnes = new QLineEdit(groupBox_12);
        LineEdit_nbabonnes->setObjectName("LineEdit_nbabonnes");
        LineEdit_nbabonnes->setGeometry(QRect(180, 290, 191, 31));
        LineEdit_nbabonnes->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_59 = new QPushButton(groupBox_12);
        pushButton_59->setObjectName("pushButton_59");
        pushButton_59->setGeometry(QRect(290, 550, 91, 41));
        pushButton_59->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_59->setIcon(icon);
        pushButton_59->setIconSize(QSize(20, 20));
        groupBox = new QGroupBox(groupBox_12);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(40, 340, 331, 191));
        groupBox->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"background-color: rgb(170, 0, 255);"));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(10, 80, 121, 25));
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setGeometry(QRect(10, 40, 161, 25));
        checkBox_3 = new QCheckBox(groupBox);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(10, 120, 161, 25));
        checkBox_6 = new QCheckBox(groupBox);
        checkBox_6->setObjectName("checkBox_6");
        checkBox_6->setGeometry(QRect(10, 160, 171, 25));
        pushButton_60 = new QPushButton(groupBox_12);
        pushButton_60->setObjectName("pushButton_60");
        pushButton_60->setGeometry(QRect(30, 550, 91, 41));
        pushButton_60->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_60->setIcon(icon);
        pushButton_60->setIconSize(QSize(20, 20));
        LineEdit_email = new QLineEdit(groupBox_12);
        LineEdit_email->setObjectName("LineEdit_email");
        LineEdit_email->setGeometry(QRect(180, 190, 191, 31));
        LineEdit_email->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_132 = new QLabel(groupBox_12);
        label_132->setObjectName("label_132");
        label_132->setGeometry(QRect(10, 200, 111, 20));
        label_132->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        LineEdit_numtel = new QLineEdit(groupBox_12);
        LineEdit_numtel->setObjectName("LineEdit_numtel");
        LineEdit_numtel->setGeometry(QRect(180, 240, 191, 31));
        LineEdit_numtel->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_138 = new QLabel(groupBox_2);
        label_138->setObjectName("label_138");
        label_138->setGeometry(QRect(430, 180, 131, 31));
        label_138->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_139 = new QLabel(groupBox_2);
        label_139->setObjectName("label_139");
        label_139->setGeometry(QRect(820, 180, 131, 20));
        label_139->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        l2_15 = new QLineEdit(groupBox_2);
        l2_15->setObjectName("l2_15");
        l2_15->setGeometry(QRect(930, 180, 281, 31));
        l2_15->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_61 = new QPushButton(groupBox_2);
        pushButton_61->setObjectName("pushButton_61");
        pushButton_61->setGeometry(QRect(1090, 440, 131, 41));
        pushButton_61->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_61->setIcon(icon);
        pushButton_61->setIconSize(QSize(20, 20));
        label = new QLabel(groupBox_2);
        label->setObjectName("label");
        label->setGeometry(QRect(450, 60, 911, 101));
        label->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/Screenshot 2025-10-06 124855.png")));
        label->setScaledContents(true);
        pushButton_62 = new QPushButton(groupBox_2);
        pushButton_62->setObjectName("pushButton_62");
        pushButton_62->setGeometry(QRect(0, 0, 171, 51));
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
        pushButton_63 = new QPushButton(groupBox_2);
        pushButton_63->setObjectName("pushButton_63");
        pushButton_63->setGeometry(QRect(180, 0, 161, 51));
        pushButton_63->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_63->setIcon(icon);
        pushButton_63->setIconSize(QSize(20, 20));
        pushButton_64 = new QPushButton(groupBox_2);
        pushButton_64->setObjectName("pushButton_64");
        pushButton_64->setGeometry(QRect(350, 0, 171, 51));
        pushButton_64->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_64->setIcon(icon);
        pushButton_64->setIconSize(QSize(20, 20));
        pushButton_65 = new QPushButton(groupBox_2);
        pushButton_65->setObjectName("pushButton_65");
        pushButton_65->setGeometry(QRect(530, 0, 161, 51));
        pushButton_65->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_65->setIcon(icon);
        pushButton_65->setIconSize(QSize(20, 20));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, -50, 1421, 651));
        label_2->setPixmap(QPixmap(QString::fromUtf8("../../../Downloads/Screenshot 2025-10-06 123537.png")));
        label_2->setScaledContents(true);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(600, 560, 63, 20));
        label_3->setScaledContents(true);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(2, -1, 1381, 681));
        label_4->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/Screenshot 2025-10-06 123537.png")));
        label_4->setScaledContents(true);
        pushButton_68 = new QPushButton(groupBox_2);
        pushButton_68->setObjectName("pushButton_68");
        pushButton_68->setGeometry(QRect(700, 0, 181, 51));
        pushButton_68->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_68->setIcon(icon);
        pushButton_68->setIconSize(QSize(20, 20));
        pushButton_69 = new QPushButton(groupBox_2);
        pushButton_69->setObjectName("pushButton_69");
        pushButton_69->setGeometry(QRect(900, 0, 171, 51));
        pushButton_69->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_69->setIcon(icon);
        pushButton_69->setIconSize(QSize(20, 20));
        tableView = new QTableView(groupBox_2);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(440, 230, 781, 201));
        tableView->setStyleSheet(QString::fromUtf8("/* Main Table View */\n"
"QTableView {\n"
"    background-color: #DAB1DA;\n"
"    color: #FFFFFF;\n"
"    font-size: 16px;\n"
"    font-family: \"Segoe UI\", \"Arial\";\n"
"    border: none;\n"
"    border-radius: 15px;\n"
"    padding: 10px;\n"
"    gridline-color: #C78AC7;\n"
"    selection-background-color: #5555FF;\n"
"    alternate-background-color: #E8C8E8;\n"
"}\n"
"\n"
"/* Table Cells */\n"
"QTableView::item {\n"
"    padding: 12px 15px;\n"
"    border: none;\n"
"    border-bottom: 1px solid #C78AC7;\n"
"}\n"
"\n"
"QTableView::item:selected {\n"
"    background-color: #5555FF;\n"
"    color: #FFFFFF;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableView::item:hover {\n"
"    background-color: #E8C8E8;\n"
"    color: #000000;\n"
"}\n"
"\n"
"/* Header Styling */\n"
"QHeaderView::section {\n"
"    background-color: #341539;\n"
"    color: #FFFFFF;\n"
"    padding: 14px 16px;\n"
"    font-weight: bold;\n"
"    font-size: 15px;\n"
"    border: none;\n"
"    border-right: 1px solid #4A1F4A;\n"
"    text-align"
                        ": center;\n"
"}\n"
"\n"
"QHeaderView::section:first {\n"
"    border-top-left-radius: 15px;\n"
"}\n"
"\n"
"QHeaderView::section:last {\n"
"    border-top-right-radius: 15px;\n"
"    border-right: none;\n"
"}\n"
"\n"
"/* Scrollbars */\n"
"QScrollBar:vertical {\n"
"    background: #C78AC7;\n"
"    width: 14px;\n"
"    margin: 0px;\n"
"    border-radius: 7px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #341539;\n"
"    min-height: 30px;\n"
"    border-radius: 7px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #5555FF;\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"\n"
"/* Horizontal Scrollbar */\n"
"QScrollBar:horizontal {\n"
"    background: #C78AC7;\n"
"    height: 14px;\n"
"    margin: 0px;\n"
"    border-radius: 7px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"    background: #341539;\n"
"    min-width: 30px;\n"
"    border-radius: 7px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal:hover {\n"
"   "
                        " background: #5555FF;\n"
"}"));
        quickWidgetstat = new QQuickWidget(groupBox_2);
        quickWidgetstat->setObjectName("quickWidgetstat");
        quickWidgetstat->setGeometry(QRect(440, 440, 431, 231));
        quickWidgetstat->setResizeMode(QQuickWidget::ResizeMode::SizeViewToRootObject);
        pushButton_stat = new QPushButton(groupBox_2);
        pushButton_stat->setObjectName("pushButton_stat");
        pushButton_stat->setGeometry(QRect(890, 620, 101, 41));
        pushButton_stat->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_stat->setIcon(icon);
        pushButton_stat->setIconSize(QSize(20, 20));
        pb_qrcode = new QPushButton(groupBox_2);
        pb_qrcode->setObjectName("pb_qrcode");
        pb_qrcode->setGeometry(QRect(890, 570, 101, 41));
        pb_qrcode->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pb_qrcode->setIcon(icon);
        pb_qrcode->setIconSize(QSize(20, 20));
        pb_calendar = new QPushButton(groupBox_2);
        pb_calendar->setObjectName("pb_calendar");
        pb_calendar->setGeometry(QRect(890, 520, 101, 41));
        pb_calendar->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pb_calendar->setIcon(icon);
        pb_calendar->setIconSize(QSize(20, 20));
        label_4->raise();
        label_2->raise();
        comboBox_2->raise();
        groupBox_12->raise();
        label_138->raise();
        label_139->raise();
        l2_15->raise();
        pushButton_61->raise();
        label->raise();
        pushButton_62->raise();
        pushButton_63->raise();
        pushButton_64->raise();
        pushButton_65->raise();
        label_3->raise();
        pushButton_68->raise();
        pushButton_69->raise();
        tableView->raise();
        quickWidgetstat->raise();
        pushButton_stat->raise();
        pb_qrcode->raise();
        pb_calendar->raise();
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1251, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_12->setTitle(QCoreApplication::translate("MainWindow", "Ajouter createur", nullptr));
        label_129->setText(QCoreApplication::translate("MainWindow", "Id_createur:", nullptr));
        label_130->setText(QCoreApplication::translate("MainWindow", "Nom:", nullptr));
        label_131->setText(QCoreApplication::translate("MainWindow", "num_tel:", nullptr));
        lineEdit_idcreateur->setText(QString());
        label_135->setText(QCoreApplication::translate("MainWindow", "Nb_abonnes:", nullptr));
        label_136->setText(QString());
        label_137->setText(QCoreApplication::translate("MainWindow", "Prenom:", nullptr));
        pushButton_59->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Platforme", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "Tiktok", nullptr));
        checkBox_2->setText(QCoreApplication::translate("MainWindow", "Instagram", nullptr));
        checkBox_3->setText(QCoreApplication::translate("MainWindow", "Youtube", nullptr));
        checkBox_6->setText(QCoreApplication::translate("MainWindow", "Autres", nullptr));
        pushButton_60->setText(QCoreApplication::translate("MainWindow", "Ajouter ", nullptr));
        label_132->setText(QCoreApplication::translate("MainWindow", "E_mail:", nullptr));
        label_138->setText(QCoreApplication::translate("MainWindow", "Trier par", nullptr));
        label_139->setText(QCoreApplication::translate("MainWindow", "Chercher ", nullptr));
        pushButton_61->setText(QCoreApplication::translate("MainWindow", "Exportation  pdf  ", nullptr));
        label->setText(QString());
        pushButton_62->setText(QCoreApplication::translate("MainWindow", "Gestion De Projets", nullptr));
        pushButton_63->setText(QCoreApplication::translate("MainWindow", "Gestion de Materielles ", nullptr));
        pushButton_64->setText(QCoreApplication::translate("MainWindow", "Gestion de Createurs ", nullptr));
        pushButton_65->setText(QCoreApplication::translate("MainWindow", "Gestion de Sponsors  ", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        pushButton_68->setText(QCoreApplication::translate("MainWindow", "Gestion de marques  ", nullptr));
        pushButton_69->setText(QCoreApplication::translate("MainWindow", "Gestion d'employees  ", nullptr));
        pushButton_stat->setText(QCoreApplication::translate("MainWindow", "statistique", nullptr));
        pb_qrcode->setText(QCoreApplication::translate("MainWindow", "qrcode", nullptr));
        pb_calendar->setText(QCoreApplication::translate("MainWindow", "calendar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
