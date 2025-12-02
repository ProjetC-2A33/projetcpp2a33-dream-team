#include "employe.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>

namespace {

QString normalizeColumn(const QString &name)
{
    QString normalized = name;
    normalized.remove('_');
    normalized = normalized.toLower();
    return normalized;
}

struct ColumnMapping {
    QString id;
    QString nomPrenom;
    QString numTel;
    QString salaire;
    QString email;
    QString typeEmploye;
    bool isValid() const {
        return !id.isEmpty() && !nomPrenom.isEmpty() && !numTel.isEmpty() &&
               !salaire.isEmpty() && !email.isEmpty() && !typeEmploye.isEmpty();
    }
};

ColumnMapping resolveColumns()
{
    static ColumnMapping mapping;
    static bool resolved = false;

    if (resolved) {
        return mapping;
    }

    QSqlQuery query("SELECT * FROM EMPLOYE WHERE 1=0");
    QSqlRecord record = query.record();
    QStringList columns;
    for (int i = 0; i < record.count(); ++i) {
        columns << record.fieldName(i);
    }

    auto findColumn = [&](std::initializer_list<QString> keys) -> QString {
        for (const QString &key : keys) {
            const QString normalizedKey = normalizeColumn(key);
            for (const QString &column : columns) {
                if (normalizeColumn(column) == normalizedKey) {
                    return column;
                }
            }
        }
        return QString();
    };

    mapping.id = findColumn({"IDEMPLOYE", "ID_EMPLOYE", "ID"});

    mapping.nomPrenom = findColumn({"NOM_PRENOM", "NOM", "NOMEMPLOYE", "NOM_COMPLET"});

    mapping.numTel = findColumn({"NUM_TEL", "NUMTEL", "TELEPHONE", "TEL", "NOM_TEL"});

    mapping.salaire = findColumn({"SALAIRE", "SALAIRE_EMPLOYE", "SALARY"});

    mapping.email = findColumn({"E_MAIL", "EMAIL", "MAIL"});

    mapping.typeEmploye = findColumn({"TYPE_EMPLOYE", "TYPE", "POSTE"});

    resolved = true;

    if (!mapping.isValid()) {
        qDebug() << "❗ Impossible de résoudre toutes les colonnes de la table EMPLOYE."
                 << "Colonnes trouvées:" << columns;
    } else {
        qDebug() << "Colonnes EMPLOYE détectées:" << mapping.id << mapping.nomPrenom
                 << mapping.numTel << mapping.salaire << mapping.email << mapping.typeEmploye;
    }

    return mapping;
}

}

Employe::Employe() {
    id_employe = 0;
    num_tel = 0;
    salaire = 0.0;
}

Employe::Employe(int id, QString nom, int tel, float sal, QString mail, QString type) {
    id_employe = id;
    nom_prenom = nom;
    num_tel = tel;
    salaire = sal;
    e_mail = mail;
    type_employe = type;
}

// --- Getters ---
int Employe::getId() { return id_employe; }
QString Employe::getNomPrenom() { return nom_prenom; }
int Employe::getNumTel() { return num_tel; }
float Employe::getSalaire() { return salaire; }
QString Employe::getEmail() { return e_mail; }
QString Employe::getTypeEmploye() { return type_employe; }

// --- Ajouter ---
bool Employe::ajouter() {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Ajout employe impossible: colonnes manquantes.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("INSERT INTO EMPLOYE (%1, %2, %3, %4, %5, %6) "
                                 "VALUES (:id, :nom, :tel, :sal, :mail, :type)")
                      .arg(columns.id, columns.nomPrenom, columns.numTel,
                           columns.salaire, columns.email, columns.typeEmploye));
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom_prenom);
    query.bindValue(":tel", num_tel);
    query.bindValue(":sal", salaire);
    query.bindValue(":mail", e_mail);
    query.bindValue(":type", type_employe);
    const bool ok = query.exec();
    if (!ok) {
        qDebug() << "Ajout employe échoué:" << query.lastError().text();
    }
    return ok;
}

// --- Afficher ---
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom et prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("E-mail"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type d'employé"));
    return model;
}

QSqlQueryModel* Employe::trierParSalaire(bool ascending) {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Tri par salaire impossible: colonnes manquantes.";
        return nullptr;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    const QString order = ascending ? QStringLiteral("ASC") : QStringLiteral("DESC");
    model->setQuery(QStringLiteral("SELECT * FROM EMPLOYE ORDER BY %1 %2")
                        .arg(columns.salaire, order));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom et prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("E-mail"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type d'employé"));
    return model;
}

QSqlQueryModel* Employe::rechercherParId(int id) {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Recherche par ID impossible: colonnes manquantes.";
        return nullptr;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT * FROM EMPLOYE WHERE %1 = :id").arg(columns.id));
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Recherche par ID échouée:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Employé"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom et prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("E-mail"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type d'employé"));
    return model;
}

// --- Supprimer ---
bool Employe::supprimer(int id) {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Suppression employe impossible: colonnes manquantes.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("DELETE FROM EMPLOYE WHERE %1 = :id").arg(columns.id));
    query.bindValue(":id", id);
    const bool ok = query.exec();
    if (!ok) {
        qDebug() << "Suppression employe échouée:" << query.lastError().text();
    }
    return ok;
}

// --- Modifier ---
bool Employe::modifier() {
    const ColumnMapping columns = resolveColumns();
    if (!columns.isValid()) {
        qDebug() << "Modification employe impossible: colonnes manquantes.";
        return false;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("UPDATE EMPLOYE SET %1=:nom, %2=:tel, %3=:sal, %4=:mail, %5=:type "
                                 "WHERE %6=:id")
                      .arg(columns.nomPrenom, columns.numTel, columns.salaire,
                           columns.email, columns.typeEmploye, columns.id));
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom_prenom);
    query.bindValue(":tel", num_tel);
    query.bindValue(":sal", salaire);
    query.bindValue(":mail", e_mail);
    query.bindValue(":type", type_employe);
    const bool ok = query.exec();
    if (!ok) {
        qDebug() << "Modification employe échouée:" << query.lastError().text();
    }
    return ok;
}
