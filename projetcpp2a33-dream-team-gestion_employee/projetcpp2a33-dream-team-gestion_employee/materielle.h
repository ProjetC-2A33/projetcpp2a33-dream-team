#ifndef MATERIELLE_H
#define MATERIELLE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>
//#include <QrCodeGenerator.h>
class Materielle
{
public:
    Materielle();

    // Structure pour représenter un matériel
    struct MaterielData {
        QString idMateriel;
        QString etatMateriel;
        QString localisation;
        QDate dateAchat;
        int nbMateriel;
        QString typeMateriel;
        QString suivi;
    };

    // CRUD Operations
    bool ajouter(const MaterielData &data, QWidget *parent = nullptr);
    bool modifier(const QString &oldId, const MaterielData &data, QWidget *parent = nullptr);
    bool supprimer(const QString &idMateriel, QWidget *parent = nullptr);
    bool lire(QTableWidget *tableWidget, QWidget *parent = nullptr);

    // Helper functions
    QString findTableName();
    bool idExists(const QString &idMateriel);
    MaterielData getMaterielById(const QString &idMateriel);

private:
    QString currentTableName;
    QString getTableName();
};

#endif // MATERIELLE_H

