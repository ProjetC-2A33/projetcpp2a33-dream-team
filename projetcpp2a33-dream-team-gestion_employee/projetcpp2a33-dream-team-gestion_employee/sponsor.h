#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QList>
#include <QMap>

class Sponsor
{
private:
    int idSponsor;
    QString nom;
    QString secteur;
    double budget;
    QString contrat_associe;
    QString type_partenariat;
    QString statut;
    QString email;
    QString telephone;
    QString date_debut_contrat;
    QString date_fin_contrat;

public:
    Sponsor();
    Sponsor(int idSponsor,
            QString nom, QString secteur, double budget,
            QString contrat, QString type, QString statut,
            QString email, QString tel,
            QString dateDebut, QString dateFin);

    // CRUD BD
    bool ajouter();
    bool modifier();
    bool supprimer();
    static QList<Sponsor> afficher(const QString &orderBy = QString(),
                                   const QString &filtre = QString());

    // Nouvelles fonctions
    static QList<Sponsor> trierParNom(bool ordreAscendant = true);
    static QMap<QString, int> getStatistiquesPartenariat();
    static QList<Sponsor> rechercherParNomEtTelephone(const QString &filtre, const QString &orderBy = QString());
static QList<Sponsor> trierParBudget(bool ordreAscendant);
    // Getters
    int getIdSponsor() const;
    QString getNom() const;
    QString getSecteur() const;
    double getBudget() const;
    QString getContrat() const;
    QString getTypePartenariat() const;
    QString getStatut() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getDateDebutContrat() const;
    QString getDateFinContrat() const;

    // Setters
    void setIdSponsor(int value);
    void setNom(const QString &value);
    void setSecteur(const QString &value);
    void setBudget(double value);
    void setContrat(const QString &value);
    void setTypePartenariat(const QString &value);
    void setStatut(const QString &value);
    void setEmail(const QString &value);
    void setTelephone(const QString &value);
    void setDateDebutContrat(const QString &value);
    void setDateFinContrat(const QString &value);
};

#endif // SPONSOR_H
