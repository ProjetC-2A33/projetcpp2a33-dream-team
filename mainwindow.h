#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include "createurcrud.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QPixmap>
#include <QHeaderView>
#include <QPainter>
#include <qrcodegen.hpp>   // Bibliothèque QR Code (voir plus bas)
#include <QCalendarWidget>
#include <QTextEdit>

using namespace qrcodegen;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_60_clicked();
    void on_pushButton_stat_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_6_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_59_clicked();
    void on_pushButton_61_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_l2_15_textChanged(const QString &arg1);

    bool isValidName(const QString &s);

    bool isValidEmail(const QString &s);

    bool isValidPhone(const QString &s);

    bool isValidNumber(const QString &s);

    void clearForm();

    void on_pb_qrcode_clicked();

    QImage qrToImage(const qrcodegen::QrCode &qr, int scale = 10, int border = 4);

    void on_pb_calendar_clicked();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
    createurcrud crc;
    QString m_selectedPlatform;

    void updatePlatformSelection(const QString &platform, QCheckBox *trigger);
    void resetPlatformSelection();
};
#endif // MAINWINDOW_H
