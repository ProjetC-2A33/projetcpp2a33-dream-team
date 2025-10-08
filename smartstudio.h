#ifndef SMARTSTUDIO_H
#define SMARTSTUDIO_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class smartstudio;
}
QT_END_NAMESPACE

class smartstudio : public QMainWindow
{
    Q_OBJECT

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

private:
    Ui::smartstudio *ui;
};
#endif // SMARTSTUDIO_H
