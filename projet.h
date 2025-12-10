#ifndef PROJET_H
#define PROJET_H

#include <QString>
#include <QDate>
#include <QVector>

// Forward declarations
class QTableWidget;
class QGroupBox;
class QCheckBox;

class Projet {
private:
    QString m_id;
    QString m_email;
    QDate m_deadline;
    double m_budget;
    QString m_typeProjet;

    static QString s_lastError;  // Declaration of static member

public:
    // Constructors
    Projet();
    Projet(const QString& id, const QString& email, const QDate& deadline,
           double budget, const QString& typeProjet);

    // Getters and setters
    QString getId() const { return m_id; }
    void setId(const QString& id) { m_id = id; }

    QString getEmail() const { return m_email; }
    void setEmail(const QString& email) { m_email = email; }

    QDate getDeadline() const { return m_deadline; }
    void setDeadline(const QDate& deadline) { m_deadline = deadline; }

    double getBudget() const { return m_budget; }
    void setBudget(double budget) { m_budget = budget; }

    QString getTypeProjet() const { return m_typeProjet; }
    void setTypeProjet(const QString& typeProjet) { m_typeProjet = typeProjet; }

    // Static database operations
    static bool add(const Projet& p);
    static bool update(const Projet& p);
    static bool updateById(const QString& oldId, const Projet& p);
    static bool removeById(const QString& id);
    static bool exportTableToPdf(QTableWidget* table, const QString& filePath);

    // Static helper methods
    static QString typeProjetFromGroupBox(const QGroupBox* box);
    static QVector<Projet> fetchAll(const QString& filter = QString());
    static void fillTable(QTableWidget* table, const QVector<Projet>& rows);

    // Static error handling
    static QString lastError() { return s_lastError; }
    static void setLastError(const QString& error) { s_lastError = error; }

private:
    // Validation
    static bool isValid(const Projet& p);
};

#endif // PROJET_H
