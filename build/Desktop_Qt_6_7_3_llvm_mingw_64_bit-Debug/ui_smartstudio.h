/********************************************************************************
** Form generated from reading UI file 'smartstudio.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMARTSTUDIO_H
#define UI_SMARTSTUDIO_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_smartstudio
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page_5;
    QLabel *label_6;
    QComboBox *comboBox_6;
    QPushButton *pushButton_79;
    QTableWidget *tableWidget_4;
    QGroupBox *groupBox_15;
    QWidget *widget_chart_types;
    QWidget *widget_chart_loc;
    QGroupBox *groupBox_14;
    QLabel *label_150;
    QLabel *label_151;
    QLabel *label_152;
    QLineEdit *lineEdit_47;
    QDateTimeEdit *dateTimeEdit_15;
    QLabel *label_155;
    QLineEdit *l2_27;
    QLabel *label_156;
    QLabel *label_157;
    QLineEdit *l2_28;
    QPushButton *pushButton_73;
    QComboBox *comboBox_7;
    QGroupBox *groupBox_4;
    QCheckBox *checkBox_13;
    QCheckBox *checkBox_14;
    QCheckBox *checkBox_15;
    QCheckBox *checkBox_16;
    QCheckBox *checkBox_17;
    QCheckBox *checkBox_18;
    QPushButton *pushButton_74;
    QPushButton *pushButton_75;
    QPushButton *pushButton_76;
    QLineEdit *l2_29;
    QLabel *label_158;
    QLabel *label_159;
    QComboBox *comboBox_11;
    QPushButton *button_avail_generate_2;
    QLabel *label_qr_preview_2;
    QPushButton *pushButton_99;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *smartstudio)
    {
        if (smartstudio->objectName().isEmpty())
            smartstudio->setObjectName("smartstudio");
        smartstudio->resize(1330, 714);
        centralwidget = new QWidget(smartstudio);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(-50, 10, 1301, 701));
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        label_6 = new QLabel(page_5);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, -30, 1311, 681));
        label_6->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/aarriere plan.png")));
        label_6->setScaledContents(true);
        comboBox_6 = new QComboBox(page_5);
        comboBox_6->setObjectName("comboBox_6");
        comboBox_6->setGeometry(QRect(570, 160, 82, 28));
        pushButton_79 = new QPushButton(page_5);
        pushButton_79->setObjectName("pushButton_79");
        pushButton_79->setGeometry(QRect(710, 100, 181, 51));
        pushButton_79->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_79->setIcon(icon);
        pushButton_79->setIconSize(QSize(20, 20));
        tableWidget_4 = new QTableWidget(page_5);
        if (tableWidget_4->columnCount() < 20)
            tableWidget_4->setColumnCount(20);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (tableWidget_4->rowCount() < 6)
            tableWidget_4->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_4->setVerticalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_4->setVerticalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_4->setVerticalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 7, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 8, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 9, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 10, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 11, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 12, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 13, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 15, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 16, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 17, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 18, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_4->setItem(5, 19, __qtablewidgetitem21);
        tableWidget_4->setObjectName("tableWidget_4");
        tableWidget_4->setGeometry(QRect(450, 220, 941, 231));
        tableWidget_4->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
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
        tableWidget_4->setRowCount(6);
        tableWidget_4->setColumnCount(20);
        groupBox_15 = new QGroupBox(page_5);
        groupBox_15->setObjectName("groupBox_15");
        groupBox_15->setGeometry(QRect(460, 460, 941, 240));
        widget_chart_types = new QWidget(groupBox_15);
        widget_chart_types->setObjectName("widget_chart_types");
        widget_chart_types->setGeometry(QRect(10, 20, 460, 200));
        widget_chart_loc = new QWidget(widget_chart_types);
        widget_chart_loc->setObjectName("widget_chart_loc");
        widget_chart_loc->setGeometry(QRect(130, -40, 451, 200));
        groupBox_14 = new QGroupBox(page_5);
        groupBox_14->setObjectName("groupBox_14");
        groupBox_14->setGeometry(QRect(60, 40, 391, 591));
        groupBox_14->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
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
        label_150 = new QLabel(groupBox_14);
        label_150->setObjectName("label_150");
        label_150->setGeometry(QRect(10, 50, 211, 20));
        label_150->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        label_151 = new QLabel(groupBox_14);
        label_151->setObjectName("label_151");
        label_151->setGeometry(QRect(10, 100, 201, 20));
        label_151->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_152 = new QLabel(groupBox_14);
        label_152->setObjectName("label_152");
        label_152->setGeometry(QRect(10, 230, 111, 20));
        label_152->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        lineEdit_47 = new QLineEdit(groupBox_14);
        lineEdit_47->setObjectName("lineEdit_47");
        lineEdit_47->setGeometry(QRect(180, 40, 131, 41));
        lineEdit_47->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        dateTimeEdit_15 = new QDateTimeEdit(groupBox_14);
        dateTimeEdit_15->setObjectName("dateTimeEdit_15");
        dateTimeEdit_15->setGeometry(QRect(180, 210, 141, 41));
        dateTimeEdit_15->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_155 = new QLabel(groupBox_14);
        label_155->setObjectName("label_155");
        label_155->setGeometry(QRect(10, 280, 181, 41));
        label_155->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}"));
        l2_27 = new QLineEdit(groupBox_14);
        l2_27->setObjectName("l2_27");
        l2_27->setGeometry(QRect(180, 100, 141, 41));
        l2_27->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_156 = new QLabel(groupBox_14);
        label_156->setObjectName("label_156");
        label_156->setGeometry(QRect(10, 170, 131, 20));
        label_156->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_157 = new QLabel(groupBox_14);
        label_157->setObjectName("label_157");
        label_157->setGeometry(QRect(10, 100, 201, 41));
        label_157->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        l2_28 = new QLineEdit(groupBox_14);
        l2_28->setObjectName("l2_28");
        l2_28->setGeometry(QRect(180, 270, 141, 41));
        l2_28->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_73 = new QPushButton(groupBox_14);
        pushButton_73->setObjectName("pushButton_73");
        pushButton_73->setGeometry(QRect(300, 520, 81, 51));
        pushButton_73->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_73->setIcon(icon);
        pushButton_73->setIconSize(QSize(20, 20));
        comboBox_7 = new QComboBox(groupBox_14);
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->setObjectName("comboBox_7");
        comboBox_7->setGeometry(QRect(180, 170, 161, 28));
        groupBox_4 = new QGroupBox(groupBox_14);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(30, 320, 331, 191));
        groupBox_4->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"background-color: rgb(170, 0, 255);"));
        checkBox_13 = new QCheckBox(groupBox_4);
        checkBox_13->setObjectName("checkBox_13");
        checkBox_13->setGeometry(QRect(10, 80, 121, 25));
        checkBox_14 = new QCheckBox(groupBox_4);
        checkBox_14->setObjectName("checkBox_14");
        checkBox_14->setGeometry(QRect(10, 40, 161, 25));
        checkBox_15 = new QCheckBox(groupBox_4);
        checkBox_15->setObjectName("checkBox_15");
        checkBox_15->setGeometry(QRect(10, 120, 161, 25));
        checkBox_16 = new QCheckBox(groupBox_4);
        checkBox_16->setObjectName("checkBox_16");
        checkBox_16->setGeometry(QRect(190, 80, 101, 25));
        checkBox_17 = new QCheckBox(groupBox_4);
        checkBox_17->setObjectName("checkBox_17");
        checkBox_17->setGeometry(QRect(190, 40, 131, 25));
        checkBox_18 = new QCheckBox(groupBox_4);
        checkBox_18->setObjectName("checkBox_18");
        checkBox_18->setGeometry(QRect(10, 160, 171, 25));
        pushButton_74 = new QPushButton(groupBox_14);
        pushButton_74->setObjectName("pushButton_74");
        pushButton_74->setGeometry(QRect(30, 520, 81, 51));
        pushButton_74->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_74->setIcon(icon);
        pushButton_74->setIconSize(QSize(20, 20));
        pushButton_75 = new QPushButton(groupBox_14);
        pushButton_75->setObjectName("pushButton_75");
        pushButton_75->setGeometry(QRect(120, 520, 81, 51));
        pushButton_75->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_75->setIcon(icon);
        pushButton_75->setIconSize(QSize(20, 20));
        pushButton_76 = new QPushButton(groupBox_14);
        pushButton_76->setObjectName("pushButton_76");
        pushButton_76->setGeometry(QRect(210, 520, 81, 51));
        pushButton_76->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_76->setIcon(icon);
        pushButton_76->setIconSize(QSize(20, 20));
        l2_29 = new QLineEdit(page_5);
        l2_29->setObjectName("l2_29");
        l2_29->setGeometry(QRect(910, 170, 131, 41));
        l2_29->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        label_158 = new QLabel(page_5);
        label_158->setObjectName("label_158");
        label_158->setGeometry(QRect(710, 180, 201, 20));
        label_158->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        label_159 = new QLabel(page_5);
        label_159->setObjectName("label_159");
        label_159->setGeometry(QRect(450, 180, 131, 31));
        label_159->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 6pt \"Segoe UI\";\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #002366;  /* Bleu fonc\303\251 */\n"
"}\n"
""));
        comboBox_11 = new QComboBox(page_5);
        comboBox_11->addItem(QString());
        comboBox_11->addItem(QString());
        comboBox_11->addItem(QString());
        comboBox_11->addItem(QString());
        comboBox_11->addItem(QString());
        comboBox_11->addItem(QString());
        comboBox_11->addItem(QString());
        comboBox_11->setObjectName("comboBox_11");
        comboBox_11->setGeometry(QRect(560, 170, 141, 41));
        comboBox_11->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    font-family: \"Arial\";\n"
"    font-size: 14px;\n"
"    color: white;\n"
"    background-color: #0A0A2A;\n"
"    border: 2px solid #1E1E5A;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
""));
        button_avail_generate_2 = new QPushButton(page_5);
        button_avail_generate_2->setObjectName("button_avail_generate_2");
        button_avail_generate_2->setGeometry(QRect(920, 60, 120, 24));
        label_qr_preview_2 = new QLabel(page_5);
        label_qr_preview_2->setObjectName("label_qr_preview_2");
        label_qr_preview_2->setGeometry(QRect(1060, 0, 241, 221));
        label_qr_preview_2->setFrameShape(QFrame::Shape::Box);
        pushButton_99 = new QPushButton(page_5);
        pushButton_99->setObjectName("pushButton_99");
        pushButton_99->setGeometry(QRect(270, 630, 171, 51));
        pushButton_99->setStyleSheet(QString::fromUtf8("QLineEdit, QDateTimeEdit {\n"
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
        pushButton_99->setIcon(icon);
        pushButton_99->setIconSize(QSize(20, 20));
        stackedWidget->addWidget(page_5);
        smartstudio->setCentralWidget(centralwidget);
        menubar = new QMenuBar(smartstudio);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1330, 26));
        smartstudio->setMenuBar(menubar);
        statusbar = new QStatusBar(smartstudio);
        statusbar->setObjectName("statusbar");
        smartstudio->setStatusBar(statusbar);

        retranslateUi(smartstudio);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(smartstudio);
    } // setupUi

    void retranslateUi(QMainWindow *smartstudio)
    {
        smartstudio->setWindowTitle(QCoreApplication::translate("smartstudio", "smartstudio", nullptr));
        label_6->setText(QString());
        pushButton_79->setText(QCoreApplication::translate("smartstudio", "Exportation format PDF  ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_4->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("smartstudio", "idMateriel", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_4->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("smartstudio", "Marque", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_4->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("smartstudio", "Etat Materiel", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_4->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("smartstudio", "Localisation ", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_4->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("smartstudio", "Date Achat", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_4->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("smartstudio", "Nbr Materiel", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_4->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("smartstudio", "Type de Materiel", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_4->verticalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("smartstudio", "2", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_4->verticalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("smartstudio", "3", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_4->verticalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("smartstudio", "4", nullptr));

        const bool __sortingEnabled = tableWidget_4->isSortingEnabled();
        tableWidget_4->setSortingEnabled(false);
        tableWidget_4->setSortingEnabled(__sortingEnabled);

        groupBox_15->setTitle(QCoreApplication::translate("smartstudio", "Statistiques", nullptr));
        groupBox_14->setTitle(QCoreApplication::translate("smartstudio", "Ajouter Materielle", nullptr));
        label_150->setText(QCoreApplication::translate("smartstudio", "idMateriel", nullptr));
        label_151->setText(QString());
        label_152->setText(QCoreApplication::translate("smartstudio", "dateAchat", nullptr));
        lineEdit_47->setText(QString());
        label_155->setText(QCoreApplication::translate("smartstudio", "Nbr Materiel", nullptr));
        label_156->setText(QCoreApplication::translate("smartstudio", "localisation", nullptr));
        label_157->setText(QCoreApplication::translate("smartstudio", "etat Materiel", nullptr));
        pushButton_73->setText(QCoreApplication::translate("smartstudio", "Annuler ", nullptr));
        comboBox_7->setItemText(0, QCoreApplication::translate("smartstudio", "On site ", nullptr));
        comboBox_7->setItemText(1, QCoreApplication::translate("smartstudio", "Remote ", nullptr));
        comboBox_7->setItemText(2, QCoreApplication::translate("smartstudio", "Hybrid", nullptr));
        comboBox_7->setItemText(3, QString());

        groupBox_4->setTitle(QCoreApplication::translate("smartstudio", "Type De Materielle", nullptr));
        checkBox_13->setText(QCoreApplication::translate("smartstudio", "Camera", nullptr));
        checkBox_14->setText(QCoreApplication::translate("smartstudio", "Produit photo shoot", nullptr));
        checkBox_15->setText(QCoreApplication::translate("smartstudio", "Micro", nullptr));
        checkBox_16->setText(QString());
        checkBox_17->setText(QCoreApplication::translate("smartstudio", "pc ", nullptr));
        checkBox_18->setText(QCoreApplication::translate("smartstudio", "Tablette graphique", nullptr));
        pushButton_74->setText(QCoreApplication::translate("smartstudio", "Ajouter ", nullptr));
        pushButton_75->setText(QCoreApplication::translate("smartstudio", "Modifier", nullptr));
        pushButton_76->setText(QCoreApplication::translate("smartstudio", "Supprimer", nullptr));
        label_158->setText(QCoreApplication::translate("smartstudio", " CHERCHER PAR ID", nullptr));
        label_159->setText(QCoreApplication::translate("smartstudio", "Trier par", nullptr));
        comboBox_11->setItemText(0, QCoreApplication::translate("smartstudio", "ID", nullptr));
        comboBox_11->setItemText(1, QCoreApplication::translate("smartstudio", "Etat", nullptr));
        comboBox_11->setItemText(2, QCoreApplication::translate("smartstudio", "Localisation", nullptr));
        comboBox_11->setItemText(3, QCoreApplication::translate("smartstudio", "Date Achat", nullptr));
        comboBox_11->setItemText(4, QCoreApplication::translate("smartstudio", "Nombre", nullptr));
        comboBox_11->setItemText(5, QCoreApplication::translate("smartstudio", "Type", nullptr));
        comboBox_11->setItemText(6, QCoreApplication::translate("smartstudio", "Suivi", nullptr));

        button_avail_generate_2->setText(QCoreApplication::translate("smartstudio", "G\303\251n\303\251rer QR", nullptr));
        label_qr_preview_2->setText(QString());
        pushButton_99->setText(QCoreApplication::translate("smartstudio", "quitter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class smartstudio: public Ui_smartstudio {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMARTSTUDIO_H
