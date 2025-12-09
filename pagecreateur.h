#ifndef pagecreateur_H
#define pagecreateur_H

#include <QDialog>
#include <QCheckBox>
#include "createurcrud.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QPixmap>
#include <QHeaderView>
#include <QPainter>
#include <qrcodegen.h>   // Bibliothèque QR Code (voir plus bas)
#include <QCalendarWidget>
#include <QTextEdit>

using namespace qrcodegen;


QT_BEGIN_NAMESPACE
namespace Ui { class pagecreateur; }
QT_END_NAMESPACE

class pagecreateur : public QDialog
{
    Q_OBJECT

public:
    pagecreateur(QWidget *parent = nullptr);
    ~pagecreateur();

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

    void on_pushButton_113_clicked();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::pagecreateur *ui;
    createurcrud crc;
    QString m_selectedPlatform;

    void updatePlatformSelection(const QString &platform, QCheckBox *trigger);
    void resetPlatformSelection();
};
#endif // pagecreateur_H
