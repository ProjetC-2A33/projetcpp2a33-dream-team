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
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindow
{
public:
    QGroupBox *groupBox_2;
    QComboBox *comboBox_2;
    QGroupBox *groupBox_12;
    QLabel *label_129;
    QLabel *label_130;
    QLabel *label_131;
    QLineEdit *lineEdit_43;
    QLineEdit *l2_13;
    QDateTimeEdit *dateTimeEdit_13;
    QLabel *label_135;
    QLabel *label_136;
    QLineEdit *l2_17;
    QPushButton *pushButton_59;
    QComboBox *comboBox_3;
    QGroupBox *groupBox;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QPushButton *pushButton_60;
    QPushButton *pushButton_63;
    QTableWidget *tableWidget_2;
    QLabel *label_138;
    QLabel *label_139;
    QLineEdit *l2_15;
    QPushButton *pushButton_61;
    QLabel *label;
    QPushButton *pushButton_62;
    QPushButton *pushButton_65;
    QPushButton *pushButton_66;
    QPushButton *pushButton_64;

    void setupUi(QDialog *mainwindow)
    {
        if (mainwindow->objectName().isEmpty())
            mainwindow->setObjectName("mainwindow");
        mainwindow->resize(1311, 716);
        groupBox_2 = new QGroupBox(mainwindow);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(-10, -10, 1441, 691));
        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(560, 200, 82, 28));
        groupBox_12 = new QGroupBox(groupBox_2);
        groupBox_12->setObjectName("groupBox_12");
        groupBox_12->setGeometry(QRect(10, 60, 431, 591));
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
        label_131->setGeometry(QRect(10, 210, 111, 20));
        label_131->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        lineEdit_43 = new QLineEdit(groupBox_12);
        lineEdit_43->setObjectName("lineEdit_43");
        lineEdit_43->setGeometry(QRect(180, 40, 131, 41));
        lineEdit_43->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        l2_13 = new QLineEdit(groupBox_12);
        l2_13->setObjectName("l2_13");
        l2_13->setGeometry(QRect(180, 90, 131, 41));
        l2_13->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        dateTimeEdit_13 = new QDateTimeEdit(groupBox_12);
        dateTimeEdit_13->setObjectName("dateTimeEdit_13");
        dateTimeEdit_13->setGeometry(QRect(180, 210, 141, 41));
        dateTimeEdit_13->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_135 = new QLabel(groupBox_12);
        label_135->setObjectName("label_135");
        label_135->setGeometry(QRect(10, 250, 181, 41));
        label_135->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        label_136 = new QLabel(groupBox_12);
        label_136->setObjectName("label_136");
        label_136->setGeometry(QRect(10, 160, 131, 20));
        label_136->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        l2_17 = new QLineEdit(groupBox_12);
        l2_17->setObjectName("l2_17");
        l2_17->setGeometry(QRect(180, 260, 141, 41));
        l2_17->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_59->setGeometry(QRect(290, 520, 101, 51));
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
        comboBox_3 = new QComboBox(groupBox_12);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(180, 160, 161, 28));
        groupBox = new QGroupBox(groupBox_12);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(40, 310, 331, 191));
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
        checkBox_4 = new QCheckBox(groupBox);
        checkBox_4->setObjectName("checkBox_4");
        checkBox_4->setGeometry(QRect(190, 80, 101, 25));
        checkBox_5 = new QCheckBox(groupBox);
        checkBox_5->setObjectName("checkBox_5");
        checkBox_5->setGeometry(QRect(190, 40, 131, 25));
        checkBox_6 = new QCheckBox(groupBox);
        checkBox_6->setObjectName("checkBox_6");
        checkBox_6->setGeometry(QRect(10, 160, 171, 25));
        pushButton_60 = new QPushButton(groupBox_12);
        pushButton_60->setObjectName("pushButton_60");
        pushButton_60->setGeometry(QRect(30, 520, 81, 51));
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
        pushButton_63 = new QPushButton(groupBox_12);
        pushButton_63->setObjectName("pushButton_63");
        pushButton_63->setGeometry(QRect(160, 520, 101, 51));
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
        tableWidget_2 = new QTableWidget(groupBox_2);
        if (tableWidget_2->columnCount() < 18)
            tableWidget_2->setColumnCount(18);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableWidget_2->rowCount() < 3)
            tableWidget_2->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 13, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 14, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 15, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 16, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_2->setItem(2, 17, __qtablewidgetitem16);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(460, 240, 921, 201));
        tableWidget_2->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #DAB1DA;\n"
"    color: white;\n"
"    border-radius: 15px;\n"
"    padding: 10px;\n"
"    font-size: 16px;\n"
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
        tableWidget_2->setRowCount(3);
        tableWidget_2->setColumnCount(18);
        label_138 = new QLabel(groupBox_2);
        label_138->setObjectName("label_138");
        label_138->setGeometry(QRect(470, 200, 111, 21));
        label_138->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_139 = new QLabel(groupBox_2);
        label_139->setObjectName("label_139");
        label_139->setGeometry(QRect(650, 200, 131, 20));
        label_139->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        l2_15 = new QLineEdit(groupBox_2);
        l2_15->setObjectName("l2_15");
        l2_15->setGeometry(QRect(770, 190, 131, 41));
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
        pushButton_61->setGeometry(QRect(1150, 590, 281, 51));
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
        label->setGeometry(QRect(460, 60, 841, 101));
        label->setPixmap(QPixmap(QString::fromUtf8("../../../../Downloads/9220705.jpg")));
        label->setScaledContents(true);
        pushButton_62 = new QPushButton(groupBox_2);
        pushButton_62->setObjectName("pushButton_62");
        pushButton_62->setGeometry(QRect(460, 590, 171, 51));
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
        pushButton_65 = new QPushButton(groupBox_2);
        pushButton_65->setObjectName("pushButton_65");
        pushButton_65->setGeometry(QRect(1240, 450, 111, 41));
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
        pushButton_66 = new QPushButton(groupBox_2);
        pushButton_66->setObjectName("pushButton_66");
        pushButton_66->setGeometry(QRect(1250, 190, 111, 41));
        pushButton_66->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_66->setIcon(icon);
        pushButton_66->setIconSize(QSize(20, 20));
        pushButton_64 = new QPushButton(groupBox_2);
        pushButton_64->setObjectName("pushButton_64");
        pushButton_64->setGeometry(QRect(710, 590, 171, 51));
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

        retranslateUi(mainwindow);

        QMetaObject::connectSlotsByName(mainwindow);
    } // setupUi

    void retranslateUi(QDialog *mainwindow)
    {
        mainwindow->setWindowTitle(QCoreApplication::translate("mainwindow", "Dialog", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("mainwindow", "GroupBox", nullptr));
        groupBox_12->setTitle(QCoreApplication::translate("mainwindow", "Ajouter Projet", nullptr));
        label_129->setText(QCoreApplication::translate("mainwindow", "Identifiant ", nullptr));
        label_130->setText(QCoreApplication::translate("mainwindow", "E-Mail", nullptr));
        label_131->setText(QCoreApplication::translate("mainwindow", "Deadline ", nullptr));
        label_135->setText(QCoreApplication::translate("mainwindow", "Budget ", nullptr));
        label_136->setText(QCoreApplication::translate("mainwindow", "Location ", nullptr));
        pushButton_59->setText(QCoreApplication::translate("mainwindow", "supprimer", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("mainwindow", "On site ", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("mainwindow", "Remote ", nullptr));
        comboBox_3->setItemText(2, QCoreApplication::translate("mainwindow", "Hybrid", nullptr));
        comboBox_3->setItemText(3, QString());

        groupBox->setTitle(QCoreApplication::translate("mainwindow", "Type De Projet ", nullptr));
        checkBox->setText(QCoreApplication::translate("mainwindow", "Video Shoot", nullptr));
        checkBox_2->setText(QCoreApplication::translate("mainwindow", "Produit photo shoot", nullptr));
        checkBox_3->setText(QCoreApplication::translate("mainwindow", "Influencer Collab", nullptr));
        checkBox_4->setText(QCoreApplication::translate("mainwindow", "Audio Ad", nullptr));
        checkBox_5->setText(QCoreApplication::translate("mainwindow", "Event Coverage ", nullptr));
        checkBox_6->setText(QCoreApplication::translate("mainwindow", "Social Media Content ", nullptr));
        pushButton_60->setText(QCoreApplication::translate("mainwindow", "Ajouter ", nullptr));
        pushButton_63->setText(QCoreApplication::translate("mainwindow", "Modifier", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("mainwindow", "Brand Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("mainwindow", "E-Mail ", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("mainwindow", "Location ", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("mainwindow", "Type de Projet", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("mainwindow", "Budget", nullptr));

        const bool __sortingEnabled = tableWidget_2->isSortingEnabled();
        tableWidget_2->setSortingEnabled(false);
        tableWidget_2->setSortingEnabled(__sortingEnabled);

        label_138->setText(QCoreApplication::translate("mainwindow", "Trier par", nullptr));
        label_139->setText(QCoreApplication::translate("mainwindow", "Chercher ", nullptr));
        pushButton_61->setText(QCoreApplication::translate("mainwindow", "Exportation format PDF  ", nullptr));
        label->setText(QString());
        pushButton_62->setText(QCoreApplication::translate("mainwindow", "AI assistant ", nullptr));
        pushButton_65->setText(QCoreApplication::translate("mainwindow", "Envoyer E-mail", nullptr));
        pushButton_66->setText(QCoreApplication::translate("mainwindow", "Statistique", nullptr));
        pushButton_64->setText(QCoreApplication::translate("mainwindow", "Quitter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainwindow: public Ui_mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
