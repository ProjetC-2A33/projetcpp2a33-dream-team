#include "createurcrud.h"
#include <QtDebug>
#include <QObject>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHash>
#include <QStringList>

createurcrud::createurcrud()
{
    idc = "";
    nom = "";
    prenom = "";
    email = "";
    platforme = "";
    numtel = "";
    nbabonnes = 0;
}

createurcrud::createurcrud(QString idc, QString nom, QString prenom, QString email,
                           QString platforme, QString numtel, int nbabonnes)
{
    this->idc = idc;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->platforme = platforme;
    this->numtel = numtel;
    this->nbabonnes = nbabonnes;
}

// === Getters ===
QString createurcrud::getidc() { return idc; }
QString createurcrud::getnom() { return nom; }
QString createurcrud::getprenom() { return prenom; }
QString createurcrud::getemail() { return email; }
QString createurcrud::getplatforme() { return platforme; }
QString createurcrud::getnumtel() { return numtel; }  // Fixed: was getnum()
int createurcrud::getnbabonnes() { return nbabonnes; }

// === Setters ===
void createurcrud::setidc(QString idc) { this->idc = idc; }
void createurcrud::setnom(QString nom) { this->nom = nom; }
void createurcrud::setprenom(QString prenom) { this->prenom = prenom; }
void createurcrud::setemail(QString email) { this->email = email; }
void createurcrud::setplatforme(QString platforme) { this->platforme = platforme; }
void createurcrud::setnumtel(QString numtel) { this->numtel = numtel; }
void createurcrud::setnbabonnes(int nbabonnes) { this->nbabonnes = nbabonnes; }

// === CRUD Operations ===

bool createurcrud::Ajouter()
{
    QSqlQuery query;
    QString nbabonnes_string = QString::number(nbabonnes);

    query.prepare("INSERT INTO createur (idc, nom, prenom, email, platforme, numtel, nbabonnes) "
                  "VALUES (:idc, :nom, :prenom, :email, :platforme, :numtel, :nbabonnes)");

    query.bindValue(":idc", idc);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":platforme", platforme);
    query.bindValue(":numtel", numtel);
    query.bindValue(":nbabonnes", nbabonnes_string);

    return query.exec();
}

QSqlQueryModel* createurcrud::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM createur");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Plateforme"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Abonnés"));

    return model;
}

QSqlQueryModel* createurcrud::rechercherParId(const QString &idPattern)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString escaped = idPattern;
    escaped.replace("'", "''");
    QString queryStr = QString("SELECT * FROM createur WHERE idc LIKE '%%1%'").arg(escaped);
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Plateforme"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Abonnés"));

    return model;
}

QSqlQueryModel* createurcrud::afficherTrieParAbonnes(bool asc)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString order = asc ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM createur ORDER BY nbabonnes " + order);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Plateforme"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Abonnés"));

    return model;
}

bool createurcrud::Supprime(QString idc)
{
    QSqlQuery query;
    query.prepare("DELETE FROM createur WHERE idc = :idc");
    query.bindValue(":idc", idc);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(nullptr, QObject::tr("Succès !"),
                                     QObject::tr("Créateur supprimé avec succès."), QMessageBox::Ok);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur !"),
                                  QObject::tr("Aucun créateur trouvé avec cet ID.\n"
                                              "Suppression échouée."), QMessageBox::Cancel);
        }
    }
    return query.numRowsAffected() > 0;
}

bool createurcrud::modifier()
{
    QSqlQuery query;
    QString nbabonnes_string = QString::number(nbabonnes);

    query.prepare("UPDATE createur SET nom=:nom, prenom=:prenom, email=:email, "
                  "platforme=:platforme, numtel=:numtel, nbabonnes=:nbabonnes "
                  "WHERE idc=:idc");

    query.bindValue(":idc", idc);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":platforme", platforme);
    query.bindValue(":numtel", numtel);
    query.bindValue(":nbabonnes", nbabonnes_string);

    return query.exec();
}

bool createurcrud::reset()
{
    QSqlQuery query;
    query.prepare("DELETE FROM createur");  // Warning: Deletes ALL records!
    return query.exec();
}

QVector<QPair<QString, int>> createurcrud::statistiquesParPlatforme() const
{
    QHash<QString, int> counts({
        {QStringLiteral("Instagram"), 0},
        {QStringLiteral("Tiktok"), 0},
        {QStringLiteral("Youtube"), 0},
        {QStringLiteral("Autres"), 0}
    });

    QSqlQuery query("SELECT platforme, COUNT(*) FROM createur GROUP BY platforme");
    while (query.next())
    {
        const QString platform = query.value(0).toString();
        const int value = query.value(1).toInt();
        if (counts.contains(platform))
            counts[platform] = value;
        else
            counts[QStringLiteral("Autres")] += value;
    }

    QVector<QPair<QString, int>> data;
    data.reserve(4);
    const QStringList orderedKeys = {QStringLiteral("Instagram"),
                                     QStringLiteral("Tiktok"),
                                     QStringLiteral("Youtube"),
                                     QStringLiteral("Autres")};
    for (const QString &key : orderedKeys)
        data.append(qMakePair(key, counts.value(key)));

    return data;
}
