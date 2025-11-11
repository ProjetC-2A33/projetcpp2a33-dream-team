#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "createurcrud.h"

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

    void on_checkBox_2_clicked();

    void on_checkBox_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_6_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_59_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    bool isValidName(const QString &s);

    bool isValidEmail(const QString &s);

    bool isValidPhone(const QString &s);

    bool isValidNumber(const QString &s);

    void clearForm();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
    createurcrud crc;
};
#endif // MAINWINDOW_H
