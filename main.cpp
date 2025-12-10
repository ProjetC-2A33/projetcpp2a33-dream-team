#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QPushButton>

// ==================== INCLUDES ====================
#include "smartstudio.h"  // Remplacez smartstudio.h par smartstudio.h
#include "connection.h"  // Connexion base de données
#include "arduino.h"     // Gestion Arduino

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "========================================";
    qDebug() << "   SYSTÈME DE POINTAGE RFID";
    qDebug() << "========================================";

    // Connexion base de données
    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::warning(nullptr, "Avertissement",
                             "Connexion base de données limitée.\n"
                             "Certaines fonctionnalités peuvent être restreintes.");
    }

    // Boîte de dialogue d'authentification
    QMessageBox msgBox;
    msgBox.setWindowTitle("🎯 SYSTÈME D'AUTHENTIFICATION");
    msgBox.setText("🔐 CHOISISSEZ VOTRE MODE D'ENTRÉE :");
    msgBox.setIcon(QMessageBox::Question);

    // Style
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #0c0c0c, stop:0.5 #1a1a2e, stop:1 #16213e);"
        "    color: white;"
        "    border: 3px solid #2a5298;"
        "    border-radius: 15px;"
        "    padding: 20px;"
        "}"
        "QMessageBox QLabel {"
        "    color: #00ffff;"
        "    font-weight: bold;"
        "    font-size: 16px;"
        "}"
        );

    // Boutons
    QPushButton *keyboardBtn = new QPushButton("⌨ CLAVIER MEMBRANE");
    QPushButton *rfidBtn = new QPushButton("🎫 BADGE RFID");
    QPushButton *authBtn = new QPushButton("🔑 AUTHENTIFICATION MANUELLE");
    QPushButton *quitBtn = new QPushButton("🚪 QUITTER");

    // Couleurs boutons
    keyboardBtn->setStyleSheet("background: #8e44ad; color: white; font-weight: bold; padding: 10px;");
    rfidBtn->setStyleSheet("background: #c0392b; color: white; font-weight: bold; padding: 10px;");
    authBtn->setStyleSheet("background: #d35400; color: white; font-weight: bold; padding: 10px;");
    quitBtn->setStyleSheet("background: #7f8c8d; color: white; font-weight: bold; padding: 10px;");

    // Ajout boutons
    msgBox.addButton(keyboardBtn, QMessageBox::AcceptRole);
    msgBox.addButton(rfidBtn, QMessageBox::AcceptRole);
    msgBox.addButton(authBtn, QMessageBox::AcceptRole);
    msgBox.addButton(quitBtn, QMessageBox::RejectRole);

    // Affichage
    msgBox.exec();
    QAbstractButton *clickedButton = msgBox.clickedButton();

    // Variables authentification
    QString employeeName = "";
    QString employeeRole = "";
    bool authenticated = false;

    if (clickedButton == keyboardBtn) {
        qDebug() << "⌨ Mode Clavier sélectionné";
        authenticated = authenticateWithKeyboard(employeeName, employeeRole);
    }
    else if (clickedButton == rfidBtn) {
        qDebug() << "🎫 Mode RFID sélectionné";
        authenticated = authenticateWithRFID(employeeName, employeeRole);
    }
    else if (clickedButton == authBtn) {
        qDebug() << "🔑 Mode Manuel sélectionné";
        // Ouvrir directement l'interface avec onglet auth
        smartstudio *w = new smartstudio();
        w->show();
        return app.exec();
    }
    else if (clickedButton == quitBtn) {
        qDebug() << "🚪 Application quittée";
        return 0;
    }

    // Si authentification réussie
    if (authenticated) {
        qDebug() << "✅ Lancement application pour:" << employeeName;

        // Ouvrir interface principale avec infos utilisateur
        smartstudio *w = new smartstudio();
        w->show();

        return app.exec();
    } else {
        qDebug() << "❌ Authentification échouée";
        if (clickedButton != authBtn && clickedButton != quitBtn) {
            QMessageBox::warning(nullptr, "Échec",
                                 "Authentification échouée.\n"
                                 "Veuillez réessayer.");
        }
        return 1;
    }
}
