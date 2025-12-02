#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QWidget>
#include <QVector>
#include <QPair>
#include <QPaintEvent>

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
    QSqlQueryModel* trierParSalaire(bool ascending);
    QSqlQueryModel* rechercherParId(int id);
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



#endif // EMPLOYE_H

