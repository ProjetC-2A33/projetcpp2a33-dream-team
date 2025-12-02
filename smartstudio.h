#ifndef SMARTSTUDIO_H
#define SMARTSTUDIO_H
#include "employe.h"

#include <QMainWindow>
#include <QCalendarWidget>
#include "employeepiewidget.h"




QT_BEGIN_NAMESPACE
namespace Ui {
class smartstudio;
}
QT_END_NAMESPACE

class smartstudio : public QMainWindow
{
    Q_OBJECT
private:
    Employe Etmp; // objet global pour manipuler les employés
    EmployeePieWidget *m_employeePieWidget;
    QCalendarWidget *m_employeeCalendar;
    void fillFieldsFromTableRow(int row); // Helper pour remplir les champs depuis une ligne du tableau
    void updateEmployeeTypeChart();

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

    void on_pushButton_81_clicked();

    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_comboBox_10_currentIndexChanged(int index);
    void on_l2_59_editingFinished();
    void on_pushButton_114_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::smartstudio *ui;
};
#endif // SMARTSTUDIO_H
