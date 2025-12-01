#ifndef CREATEURCRUD_H
#define CREATEURCRUD_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPair>
#include <QVector>

class createurcrud
{
private:
    int nbabonnes;
    QString idc, nom, prenom, email, platforme, numtel;

public:
    createurcrud();
    createurcrud(QString idc, QString nom, QString prenom, QString email,
                 QString platforme, QString numtel, int nbabonnes);

    // CRUD Operations
    bool Ajouter();
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercherParId(const QString &idPattern);
    QSqlQueryModel* afficherTrieParAbonnes(bool asc);
    bool Supprime(QString idc);
    bool modifier();
    bool reset();

    // Statistics
    QVector<QPair<QString, int>> statistiquesParPlatforme() const;

    // Getters
    QString getidc();
    QString getnom();
    QString getprenom();
    QString getemail();
    QString getplatforme();
    QString getnumtel();
    int getnbabonnes();

    // Setters
    void setidc(QString idc);
    void setnom(QString nom);
    void setprenom(QString prenom);
    void setemail(QString email);
    void setplatforme(QString platforme);
    void setnumtel(QString numtel);
    void setnbabonnes(int nbabonnes);
};

#endif // CREATEURCRUD_H
