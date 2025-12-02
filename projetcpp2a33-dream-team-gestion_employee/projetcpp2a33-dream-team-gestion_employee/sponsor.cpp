#include "sponsor.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Sponsor::Sponsor()
    : idSponsor(-1), budget(0.0)
{}
int Sponsor::getIdSponsor() const { return idSponsor; }
QString Sponsor::getNom() const { return nom; }
QString Sponsor::getSecteur() const { return secteur; }
double Sponsor::getBudget() const { return budget; }
QString Sponsor::getContrat() const { return contrat_associe; }
QString Sponsor::getTypePartenariat() const { return type_partenariat; }
QString Sponsor::getStatut() const { return statut; }
QString Sponsor::getEmail() const { return email; }
QString Sponsor::getTelephone() const { return telephone; }
QString Sponsor::getDateDebutContrat() const { return date_debut_contrat; }
QString Sponsor::getDateFinContrat() const { return date_fin_contrat; }

void Sponsor::setIdSponsor(int value) { idSponsor = value; }
void Sponsor::setNom(const QString &value) { nom = value; }
void Sponsor::setSecteur(const QString &value) { secteur = value; }
void Sponsor::setBudget(double value) { budget = value; }
void Sponsor::setContrat(const QString &value) { contrat_associe = value; }
void Sponsor::setTypePartenariat(const QString &value) { type_partenariat = value; }
void Sponsor::setStatut(const QString &value) { statut = value; }
void Sponsor::setEmail(const QString &value) { email = value; }
void Sponsor::setTelephone(const QString &value) { telephone = value; }
void Sponsor::setDateDebutContrat(const QString &value) { date_debut_contrat = value; }
void Sponsor::setDateFinContrat(const QString &value) { date_fin_contrat = value; }

Sponsor::Sponsor(int idSponsor,
                 QString nom, QString secteur, double budget,
                 QString contrat, QString type, QString statut,
                 QString email, QString tel,
                 QString dateDebut, QString dateFin)
    : idSponsor(idSponsor),
    nom(nom), secteur(secteur), budget(budget),
    contrat_associe(contrat), type_partenariat(type),
    statut(statut), email(email), telephone(tel),
    date_debut_contrat(dateDebut), date_fin_contrat(dateFin)
{}

// ==================== AJOUTER ====================

bool Sponsor::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::ajouter()";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO SPONSOR "
        "(NOM, SECTEUR, BUDGET, CONTRAT_ASSOCIE, TYPE_PARTENARIAT, "
        " STATUT, EMAIL, TELEPHONE, DATE_DEBUT, DATE_FIN) "
        "VALUES (:nom, :secteur, :budget, :contrat, :typePart, "
        "        :statut, :email, :tel, "
        "        TO_DATE(:dateDebut, 'YYYY-MM-DD'), "
        "        TO_DATE(:dateFin,   'YYYY-MM-DD'))"
        );

    query.bindValue(":nom",      nom);
    query.bindValue(":secteur",  secteur);
    query.bindValue(":budget",   budget);
    query.bindValue(":contrat",  contrat_associe);
    query.bindValue(":typePart", type_partenariat);
    query.bindValue(":statut",   statut);
    query.bindValue(":email",    email);
    query.bindValue(":tel",      telephone);
    query.bindValue(":dateDebut", date_debut_contrat);
    query.bindValue(":dateFin",   date_fin_contrat);

    if (!query.exec()) {
        qDebug() << "Erreur INSERT SPONSOR :" << query.lastError().text();
        return false;
    }
    return true;
}

// ==================== MODIFIER ====================

bool Sponsor::modifier()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::modifier()";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "UPDATE SPONSOR SET "
        "NOM = :nom, SECTEUR = :secteur, BUDGET = :budget, "
        "CONTRAT_ASSOCIE = :contrat, TYPE_PARTENARIAT = :typePart, "
        "STATUT = :statut, EMAIL = :email, TELEPHONE = :tel, "
        "DATE_DEBUT = TO_DATE(:dateDebut, 'YYYY-MM-DD'), "
        "DATE_FIN   = TO_DATE(:dateFin,   'YYYY-MM-DD') "
        "WHERE ID_SPONSOR = :id"
        );

    query.bindValue(":nom",      nom);
    query.bindValue(":secteur",  secteur);
    query.bindValue(":budget",   budget);
    query.bindValue(":contrat",  contrat_associe);
    query.bindValue(":typePart", type_partenariat);
    query.bindValue(":statut",   statut);
    query.bindValue(":email",    email);
    query.bindValue(":tel",      telephone);
    query.bindValue(":dateDebut", date_debut_contrat);
    query.bindValue(":dateFin",   date_fin_contrat);
    query.bindValue(":id",       idSponsor);

    if (!query.exec()) {
        qDebug() << "Erreur UPDATE SPONSOR :" << query.lastError().text();
        return false;
    }
    return true;
}

// ==================== SUPPRIMER ====================

bool Sponsor::supprimer()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::supprimer()";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM SPONSOR WHERE ID_SPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (!query.exec()) {
        qDebug() << "Erreur DELETE SPONSOR :" << query.lastError().text();
        return false;
    }
    return true;
}

// ==================== AFFICHER ====================

QList<Sponsor> Sponsor::afficher(const QString &orderBy,
                                 const QString &filtre)
{
    QList<Sponsor> liste;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::afficher()";
        return liste;
    }

    QString sql =
        "SELECT ID_SPONSOR, NOM, SECTEUR, BUDGET, CONTRAT_ASSOCIE, "
        "TYPE_PARTENARIAT, STATUT, EMAIL, TELEPHONE, "
        "TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT_STR, "
        "TO_CHAR(DATE_FIN,   'YYYY-MM-DD') AS DATE_FIN_STR "
        "FROM SPONSOR";

    bool hasFilter = !filtre.trimmed().isEmpty();
    if (hasFilter) {
        sql += " WHERE LOWER(NOM) LIKE :filtre OR LOWER(SECTEUR) LIKE :filtre2";
    }

    if (!orderBy.isEmpty()) {
        sql += " ORDER BY " + orderBy;
    }

    QSqlQuery query(db);
    query.prepare(sql);

    if (hasFilter) {
        QString f = "%" + filtre.trimmed().toLower() + "%";
        query.bindValue(":filtre",  f);
        query.bindValue(":filtre2", f);
    }

    if (!query.exec()) {
        qDebug() << "Erreur SELECT SPONSOR :" << query.lastError().text();
        return liste;
    }

    while (query.next()) {
        int     id        = query.value("ID_SPONSOR").toInt();
        QString nom       = query.value("NOM").toString();
        QString secteur   = query.value("SECTEUR").toString();
        double  budget    = query.value("BUDGET").toDouble();
        QString contrat   = query.value("CONTRAT_ASSOCIE").toString();
        QString type      = query.value("TYPE_PARTENARIAT").toString();
        QString statut    = query.value("STATUT").toString();
        QString email     = query.value("EMAIL").toString();
        QString tel       = query.value("TELEPHONE").toString();
        QString dateDebut = query.value("DATE_DEBUT_STR").toString();
        QString dateFin   = query.value("DATE_FIN_STR").toString();

        liste.append(Sponsor(id, nom, secteur, budget, contrat, type, statut,
                             email, tel, dateDebut, dateFin));
    }

    return liste;
}

QList<Sponsor> Sponsor::trierParNom(bool ordreAscendant)
{
    QList<Sponsor> liste;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::trierParNom()";
        return liste;
    }

    QString orderDirection = ordreAscendant ? "ASC" : "DESC";

    QString sql =
        "SELECT ID_SPONSOR, NOM, SECTEUR, BUDGET, CONTRAT_ASSOCIE, "
        "TYPE_PARTENARIAT, STATUT, EMAIL, TELEPHONE, "
        "TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT_STR, "
        "TO_CHAR(DATE_FIN,   'YYYY-MM-DD') AS DATE_FIN_STR "
        "FROM SPONSOR "
        "ORDER BY NOM " + orderDirection;

    QSqlQuery query(db);

    if (!query.exec(sql)) {
        qDebug() << "Erreur TRI PAR NOM :" << query.lastError().text();
        return liste;
    }

    while (query.next()) {
        int     id        = query.value("ID_SPONSOR").toInt();
        QString nom       = query.value("NOM").toString();
        QString secteur   = query.value("SECTEUR").toString();
        double  budget    = query.value("BUDGET").toDouble();
        QString contrat   = query.value("CONTRAT_ASSOCIE").toString();
        QString type      = query.value("TYPE_PARTENARIAT").toString();
        QString statut    = query.value("STATUT").toString();
        QString email     = query.value("EMAIL").toString();
        QString tel       = query.value("TELEPHONE").toString();
        QString dateDebut = query.value("DATE_DEBUT_STR").toString();
        QString dateFin   = query.value("DATE_FIN_STR").toString();

        liste.append(Sponsor(id, nom, secteur, budget, contrat, type, statut,
                             email, tel, dateDebut, dateFin));
    }

    return liste;
}


QList<Sponsor> Sponsor::rechercherParNomEtTelephone(const QString &filtre, const QString &orderBy)
{
    QList<Sponsor> liste;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::rechercherParNomEtTelephone()";
        return liste;
    }

    QString sql =
        "SELECT ID_SPONSOR, NOM, SECTEUR, BUDGET, CONTRAT_ASSOCIE, "
        "TYPE_PARTENARIAT, STATUT, EMAIL, TELEPHONE, "
        "TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT_STR, "
        "TO_CHAR(DATE_FIN,   'YYYY-MM-DD') AS DATE_FIN_STR "
        "FROM SPONSOR "
        "WHERE LOWER(NOM) LIKE :filtre OR TELEPHONE LIKE :filtreTel";

    if (!orderBy.isEmpty()) {
        sql += " ORDER BY " + orderBy;
    }

    QSqlQuery query(db);
    query.prepare(sql);

    QString filtreNom = "%" + filtre.trimmed().toLower() + "%";
    QString filtreTel = "%" + filtre.trimmed() + "%";

    query.bindValue(":filtre", filtreNom);
    query.bindValue(":filtreTel", filtreTel);

    if (!query.exec()) {
        qDebug() << "Erreur RECHERCHE NOM/TEL :" << query.lastError().text();
        return liste;
    }

    while (query.next()) {
        int     id        = query.value("ID_SPONSOR").toInt();
        QString nom       = query.value("NOM").toString();
        QString secteur   = query.value("SECTEUR").toString();
        double  budget    = query.value("BUDGET").toDouble();
        QString contrat   = query.value("CONTRAT_ASSOCIE").toString();
        QString type      = query.value("TYPE_PARTENARIAT").toString();
        QString statut    = query.value("STATUT").toString();
        QString email     = query.value("EMAIL").toString();
        QString tel       = query.value("TELEPHONE").toString();
        QString dateDebut = query.value("DATE_DEBUT_STR").toString();
        QString dateFin   = query.value("DATE_FIN_STR").toString();

        liste.append(Sponsor(id, nom, secteur, budget, contrat, type, statut,
                             email, tel, dateDebut, dateFin));
    }

    return liste;
}


// Ajouter cette fonction dans sponsor.cpp
QMap<QString, int> Sponsor::getStatistiquesPartenariat()
{
    QMap<QString, int> statistiques;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::getStatistiquesPartenariat()";
        return statistiques;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT TYPE_PARTENARIAT, COUNT(*) as nombre "
        "FROM SPONSOR "
        "GROUP BY TYPE_PARTENARIAT"
        );

    if (!query.exec()) {
        qDebug() << "Erreur SELECT statistiques :" << query.lastError().text();
        return statistiques;
    }

    while (query.next()) {
        QString type = query.value("TYPE_PARTENARIAT").toString();
        int count = query.value("nombre").toInt();
        statistiques[type] = count;
    }

    return statistiques;
}
QList<Sponsor> Sponsor::trierParBudget(bool ordreAscendant)
{
    QList<Sponsor> liste;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "BD non ouverte dans Sponsor::trierParBudget()";
        return liste;
    }

    QString orderDirection = ordreAscendant ? "ASC" : "DESC";

    QString sql =
        "SELECT ID_SPONSOR, NOM, SECTEUR, BUDGET, CONTRAT_ASSOCIE, "
        "TYPE_PARTENARIAT, STATUT, EMAIL, TELEPHONE, "
        "TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT_STR, "
        "TO_CHAR(DATE_FIN,   'YYYY-MM-DD') AS DATE_FIN_STR "
        "FROM SPONSOR "
        "ORDER BY BUDGET " + orderDirection;

    QSqlQuery query(db);

    if (!query.exec(sql)) {
        qDebug() << "Erreur TRI PAR BUDGET :" << query.lastError().text();
        return liste;
    }

    while (query.next()) {
        int     id        = query.value("ID_SPONSOR").toInt();
        QString nom       = query.value("NOM").toString();
        QString secteur   = query.value("SECTEUR").toString();
        double  budget    = query.value("BUDGET").toDouble();
        QString contrat   = query.value("CONTRAT_ASSOCIE").toString();
        QString type      = query.value("TYPE_PARTENARIAT").toString();
        QString statut    = query.value("STATUT").toString();
        QString email     = query.value("EMAIL").toString();
        QString tel       = query.value("TELEPHONE").toString();
        QString dateDebut = query.value("DATE_DEBUT_STR").toString();
        QString dateFin   = query.value("DATE_FIN_STR").toString();

        liste.append(Sponsor(id, nom, secteur, budget, contrat, type, statut,
                             email, tel, dateDebut, dateFin));
    }

    return liste;
}
