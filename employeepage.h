#ifndef EMPLOYEEPAGE_H
#define EMPLOYEEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QAbstractItemModel>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class EmployeePage;
}
QT_END_NAMESPACE

// Classe Employe - intégrée dans le même fichier
class Employe
{
public:
    Employe();
    Employe(int, QString, int, float, QString, QString);

    // Getters
    int getId();
    QString getNomPrenom();
    int getNumTel();
    float getSalaire();
    QString getEmail();
    QString getTypeEmploye();

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();

private:
    int id_employe;
    QString nom_prenom;
    int num_tel;
    float salaire;
    QString e_mail;
    QString type_employe;
};

// Classe EmployeePage - page de gestion des employés
class EmployeePage : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeePage(QWidget *parent = nullptr);
    ~EmployeePage();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_113_clicked();

signals:
    void backToMenu();

private:
    Ui::EmployeePage *ui;
    Employe Etmp; // objet global pour manipuler les employés
    void fillFieldsFromTableRow(int row);
    void refreshTable();
};

#endif // EMPLOYEEPAGE_H

