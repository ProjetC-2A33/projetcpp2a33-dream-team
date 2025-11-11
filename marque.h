#ifndef MARQUE_H
#define MARQUE_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class marque;
}
QT_END_NAMESPACE

class marque : public QMainWindow
{
    Q_OBJECT

public:
    marque(QWidget *parent = nullptr);
    ~marque();

private slots:
    void on_pushButton_60_clicked();  // Ajouter/Modifier marque
    void on_pushButton_63_clicked();  // Ajouter projet
    void on_pushButton_59_clicked();  // Annuler/Supprimer
    void modifierMarque();
    void supprimerMarque();
    void on_tableWidget_2_clicked(const QModelIndex &index);
    void on_tableWidget_2_itemSelectionChanged();
    void showContextMenu(const QPoint &pos);

private:
    Ui::marque *ui;
    bool modeEdition; // true si on est en mode modification, false si en mode ajout
    void afficherMarques();
    void afficherProjets();
    void mettreAJourBoutons();
    bool validateInputs(QString &errorMessage) const; // contrôle de saisie commun
    int findRowByNom(const QString &nom) const;       // utilitaire pour retrouver la ligne par nom
    void markFieldValidity(QWidget *w, bool ok) const; // surligner champ invalide
};
#endif // MARQUE_H
