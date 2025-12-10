#include "arduino.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QTimer>
#include <QSqlError>
#include <QSqlDatabase>
#include <QApplication>

// ==================== CLASSE ARDUINO SIMPLE ====================

arduino::arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

int arduino::connect_arduino()
{
    // Rechercher les ports disponibles
    foreach(const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if ((serial_port_info.vendorIdentifier() == arduino_uno_vendor_id) && 
                (serial_port_info.productIdentifier() == arduino_uno_prod_id)) {
                qDebug() << "Vendor ID:" << serial_port_info.vendorIdentifier();
                qDebug() << "Product ID:" << serial_port_info.productIdentifier();
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }
    
    qDebug() << "Arduino disponible:" << arduino_is_available;
    qDebug() << "Arduino port name:" << arduino_port_name;

    if (arduino_is_available) {
        // Configuration
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;  // Succès
        }
        return 1;  // Erreur ouverture
    }
    return -1;  // Arduino non trouvé
}

QString arduino::getarduino_port_name()
{
    return this->arduino_port_name;
}

QSerialPort* arduino::getserial()
{
    return serial;
}

int arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}

QByteArray arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll();
        return data;
    } else {
        qDebug() << "Erreur lecture Arduino";
    }
    return QByteArray();
}

int arduino::write_to_arduino(QByteArray msg)
{
    if (serial->isWritable()) {
        serial->write(msg);
        qDebug() << "Envoyé à Arduino:" << msg;
        return 0;
    } else {
        qDebug() << "Impossible d'écrire sur le port série!";
    }
    return -1;
}

int arduino::writeStringToArduino(const char *msg)
{
    if (serial->isWritable()) {
        serial->write(msg);
        qDebug() << "Envoyé à Arduino:" << msg;
        return 0;
    } else {
        qDebug() << "Impossible d'écrire sur le port série!";
    }
    return -1;
}

int arduino::write1_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d);
        return 0;
    } else {
        qDebug() << "Erreur en écriture";
    }
    return -1;
}

// ==================== CLASSE RFID READER ====================

RFIDReader::RFIDReader(QObject *parent) : QObject(parent), serialPort(new QSerialPort(this))
{
    connect(serialPort, &QSerialPort::readyRead, this, &RFIDReader::readData);
}

RFIDReader::~RFIDReader()
{
    stopRFID();
    delete serialPort;
}

// ==================== GESTION CONNEXION ====================
bool RFIDReader::startRFID()
{
    QStringList portNames = {"COM11", "COM6", "COM4", "COM5", "COM3", "COM7", "COM8", "COM9", "COM10"};

    foreach (const QString &portName, portNames) {
        qDebug() << "🔄 Tentative de connexion à" << portName << "...";
        serialPort->setPortName(portName);
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << "🎉✅ CONNECTÉ À" << portName << "- ARDUINO DÉTECTÉ!";
            serialPort->clear();
            emit connectionStatusChanged(true);

            // Envoyer un message de test
            sendCommandToArduino("TEST:QtConnecte");

            return true;
        } else {
            qDebug() << "💥 ERREUR" << portName << ":" << serialPort->errorString();
        }
    }

    qDebug() << "❌ Aucun port COM disponible";
    emit connectionStatusChanged(false);
    emit errorOccurred("Aucun port COM disponible pour la connexion Arduino");
    return false;
}

void RFIDReader::stopRFID()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "🔴 RFID déconnecté";
        emit connectionStatusChanged(false);
    }
}

void RFIDReader::sendCommandToArduino(const QString &command)
{
    if (serialPort && serialPort->isOpen()) {
        QString fullCommand = command + "\n";  // ✅ Assurez-vous d'ajouter \n
        QByteArray data = fullCommand.toUtf8();
        qint64 bytesWritten = serialPort->write(data);

        // ✅ Forcer l'écriture immédiate
        serialPort->waitForBytesWritten(1000);
        serialPort->flush();

        if (bytesWritten > 0) {
            qDebug() << "✅ Commande envoyée à Arduino:" << command;
        } else {
            qDebug() << "❌ Erreur envoi commande à Arduino:" << serialPort->errorString();
        }
    } else {
        qDebug() << "❌ Port série non ouvert, impossible d'envoyer:" << command;
    }
}

// ==================== VÉRIFICATION BASE DE DONNÉES ====================
QSqlDatabase RFIDReader::getDatabase()
{
    return QSqlDatabase::database();
}

bool RFIDReader::checkUID(const QString &uid, QString &name, QString &role)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non connectée";
        return false;
    }

    QString cleanedUID = uid.trimmed().toUpper();
    qDebug() << "🔍 Recherche UID dans base:" << cleanedUID;

    // ✅ CORRIGÉ : Utiliser EMPLOYE (sans S) et le bon schéma
    QSqlQuery query(db);

    // Option 1 : Avec le schéma
    query.prepare("SELECT NOM_PRENOM, TYPE_EMPLOYE FROM GST_SPONSOR.EMPLOYE WHERE UID_RFID = :uid");

    // Option 2 : Sans le schéma (si vous êtes déjà connecté avec GST_SPONSOR)
    // query.prepare("SELECT NOM_PRENOM, TYPE_EMPLOYE FROM EMPLOYE WHERE UID_RFID = :uid");

    query.bindValue(":uid", cleanedUID);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL checkUID:" << query.lastError().text();
        qDebug() << "Dernière requête:" << query.lastQuery();

        // Essayer une autre version sans schéma
        query.prepare("SELECT NOM_PRENOM, TYPE_EMPLOYE FROM EMPLOYE WHERE UID_RFID = :uid");
        query.bindValue(":uid", cleanedUID);

        if (!query.exec()) {
            qDebug() << "❌ Erreur SQL (2e tentative):" << query.lastError().text();
            return false;
        }
    }

    if (query.next()) {
        name = query.value(0).toString();
        role = query.value(1).toString();
        qDebug() << "✅ Employé trouvé:" << name << "-" << role;
        return true;
    } else {
        qDebug() << "❌ UID non trouvé dans la base:" << cleanedUID;
        return false;
    }
}

bool RFIDReader::checkPassword(const QString &password, QString &name, QString &role)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non connectée";
        return false;
    }

    QSqlQuery query(db);

    // ✅ CORRIGÉ : Utiliser EMPLOYE (sans S)
    query.prepare("SELECT NOM_PRENOM, TYPE_EMPLOYE FROM GST_SPONSOR.EMPLOYE WHERE PASSWORD = :password");
    query.bindValue(":password", password.trimmed());

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL checkPassword:" << query.lastError().text();

        // Essayer sans schéma
        query.prepare("SELECT NOM_PRENOM, TYPE_EMPLOYE FROM EMPLOYE WHERE PASSWORD = :password");
        query.bindValue(":password", password.trimmed());

        if (!query.exec()) {
            qDebug() << "❌ Erreur SQL (2e tentative):" << query.lastError().text();
            return false;
        }
    }

    if (query.next()) {
        name = query.value(0).toString();
        role = query.value(1).toString();
        qDebug() << "✅ Mot de passe correct - Employé:" << name << "-" << role;
        return true;
    } else {
        qDebug() << "❌ Mot de passe incorrect ou utilisateur non trouvé";
        return false;
    }
}

// ==================== TRAITEMENT DONNÉES SÉRIE ====================
void RFIDReader::readData()
{
    static QByteArray buffer;

    if (!serialPort || !serialPort->isOpen()) {
        return;
    }

    buffer.append(serialPort->readAll());

    // Chercher des lignes complètes
    while (true) {
        int lineEnd = buffer.indexOf('\n');
        if (lineEnd == -1) {
            break;
        }

        QByteArray lineData = buffer.left(lineEnd);
        if (lineData.endsWith('\r')) {
            lineData.chop(1);
        }

        buffer = buffer.mid(lineEnd + 1);

        QString line = QString::fromUtf8(lineData).trimmed();

        if (!line.isEmpty()) {
            qDebug() << "📨 Arduino → Qt:" << line;
            emit arduinoMessageReceived(line);
            processReceivedLine(line);
        }
    }

    // Sécurité buffer
    if (buffer.size() > 1024) {
        buffer.clear();
    }
}

void RFIDReader::processReceivedLine(const QString &line)
{
    QString trimmedLine = line.trimmed();

    if (trimmedLine.isEmpty()) {
        return;
    }

    if (trimmedLine.startsWith("RFID:DETECTED:")) {
        processRFIDLine(trimmedLine);
    }
    else if (trimmedLine.startsWith("PASSWORD:")) {
        processPasswordLine(trimmedLine);
    }
    else if (trimmedLine.startsWith("FLAME:") || trimmedLine.startsWith("FLAMME:")) {
        processFlammeLine(trimmedLine);
    }
    else if (trimmedLine.startsWith("ARDUINO:")) {
        qDebug() << "🤖" << trimmedLine.mid(8);
    }
    else {
        qDebug() << "📝 Arduino:" << trimmedLine;
    }
}

void RFIDReader::processRFIDLine(const QString &line)
{
    QString uid;

    if (line.startsWith("RFID:DETECTED:")) {
        uid = line.mid(14).trimmed();
    } else {
        uid = line.mid(5).trimmed();
    }

    cleanupUID(uid);

    if (!uid.isEmpty() && uid.length() >= 4) {
        qDebug() << "🎫 UID RFID reçu:" << uid;
        emit cardDetected(uid);
    } else {
        qDebug() << "❌ UID RFID invalide:" << uid;
    }
}

void RFIDReader::processPasswordLine(const QString &line)
{
    QString password = line.mid(9).trimmed();

    if (!password.isEmpty()) {
        qDebug() << "🔑 Mot de passe reçu:" << password;
        emit passwordEntered(password);
    }
}

void RFIDReader::processFlammeLine(const QString &line)
{
    QString data;
    if (line.startsWith("FLAME:")) {
        data = line.mid(6).trimmed();
    } else {
        data = line.mid(7).trimmed();  // FLAMME:
    }

    qDebug() << "🔥 ALERTE FLAMME reçue:" << data;
    emit flameDetected(data);
}

void RFIDReader::cleanupUID(QString &uid)
{
    uid = uid.toUpper();
    uid.remove(QRegularExpression("[^A-F0-9]"));
}

// ==================== FONCTIONS D'AUTHENTIFICATION ====================
bool authenticateWithRFID(QString &employeeName, QString &employeeRole)
{
    qDebug() << "\n=== DÉMARRAGE AUTHENTIFICATION RFID ===";

    RFIDReader rfid;
    bool authenticated = false;
    bool carteRefusee = false;
    bool annule = false;

    if (!rfid.startRFID()) {
        QMessageBox::critical(nullptr, "ERREUR",
                              "IMPOSSIBLE DE SE CONNECTER À L'ARDUINO!\n\n"
                              "Vérifiez que:\n"
                              "• L'Arduino est branché sur USB\n"
                              "• Le câble USB fonctionne\n"
                              "• Le bon port COM est sélectionné");
        return false;
    }

    // Dialogue d'attente
    QDialog waitDialog;
    waitDialog.setWindowTitle("Authentification RFID");
    waitDialog.setFixedSize(400, 200);
    waitDialog.setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #0c0c0c, stop:0.5 #1a1a2e, stop:1 #16213e);"
        "    border: 2px solid #00ffff;"
        "    border-radius: 15px;"
        "}"
        );

    QVBoxLayout *layout = new QVBoxLayout(&waitDialog);

    QLabel *label = new QLabel(
        "🎫 APPROCHEZ VOTRE CARTE RFID\n\n"
        "Système prêt à lire les badges...");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 14px; font-weight: bold; color: #00ffff;");
    layout->addWidget(label);

    QLabel *statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 12px; color: #ffff00;");
    layout->addWidget(statusLabel);

    QPushButton *cancelButton = new QPushButton("Annuler");
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #ff4444, stop:1 #cc0000);"
        "    border: 2px solid #ff0000;"
        "    color: #ffffff;"
        "    border-radius: 10px;"
        "    font-weight: bold;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover { background: #ff6666; }"
        );
    layout->addWidget(cancelButton);

    // Connexion signal RFID détecté
    QObject::connect(&rfid, &RFIDReader::cardDetected,
                     [&](const QString &uid) {
                         statusLabel->setText("Carte détectée: " + uid);

                         qApp->processEvents();

                         if (rfid.checkUID(uid, employeeName, employeeRole)) {
                             authenticated = true;
                             qDebug() << "🎉 AUTHENTIFICATION RFID RÉUSSIE!";

                             // ✅ ENVOYER UNE COMMANDE SPÉCIALE POUR ALLUMER LA LED VERTE IMMÉDIATEMENT
                             QString commande = "LED_VERTE_ON:" + employeeName;
                             qDebug() << "Envoi à Arduino (LED verte):" << commande;
                             rfid.sendCommandToArduino(commande);

                             statusLabel->setText("✅ Accès autorisé pour " + employeeName);
                             statusLabel->setStyleSheet("font-size: 12px; color: #00ff00; font-weight: bold;");

                             // Attendre que l'utilisateur voie le message (2 secondes seulement)
                             QTimer::singleShot(2000, &waitDialog, [&]() {
                                 waitDialog.accept();
                             });
                         } else {
                             qDebug() << "❌ Carte non reconnue";
                             carteRefusee = true;

                             // Envoyer REFUSEE à Arduino
                             rfid.sendCommandToArduino("REFUSEE");

                             statusLabel->setText("❌ Carte non reconnue");
                             statusLabel->setStyleSheet("font-size: 12px; color: #ff0000; font-weight: bold;");

                             // Attendre qu'Arduino termine l'animation rouge
                             QElapsedTimer timer;
                             timer.start();
                             while (timer.elapsed() < 6000) {
                                 qApp->processEvents();
                                 QThread::msleep(100);

                                 if (waitDialog.isHidden()) {
                                     annule = true;
                                     break;
                                 }
                             }

                             if (!annule) {
                                 qDebug() << "🔄 Prêt pour une nouvelle carte...";
                                 statusLabel->setText("Approchez une autre carte...");
                                 statusLabel->setStyleSheet("font-size: 12px; color: #ffff00;");
                             }
                         }
                     });

    QObject::connect(cancelButton, &QPushButton::clicked, [&]() {
        qDebug() << "❌ Authentification RFID annulée";
        annule = true;
        waitDialog.reject();
    });

    qDebug() << "⏳ En attente d'une carte RFID...";

    waitDialog.exec();

    // Si authentifié, envoyer la commande pour ouvrir la porte
    if (authenticated && !annule) {
        qDebug() << "🚪 Envoi commande ouverture porte...";
        QString commandePorte = "OUVRIR_PORTE:" + employeeName;
        rfid.sendCommandToArduino(commandePorte);

        // Attendre que la porte s'ouvre et se ferme
        QThread::sleep(5);
    }

    if (carteRefusee && !annule) {
        qDebug() << "🕒 Attente supplémentaire pour LED rouge Arduino...";
        QThread::sleep(2);
    }

    rfid.stopRFID();

    if (authenticated) {
        qDebug() << "✅ Authentification réussie pour:" << employeeName;
        QMessageBox::information(nullptr, "Accès Autorisé",
                                 "Bienvenue " + employeeName + "!\n"
                                                               "Rôle: " + employeeRole);
        return true;
    } else {
        qDebug() << "❌ Authentification échouée";
        if (!annule && carteRefusee) {
            QMessageBox::warning(nullptr, "Accès Refusé",
                                 "Carte RFID non reconnue.\n"
                                 "Veuillez contacter l'administrateur.");
        }
        return false;
    }
}

bool authenticateWithKeyboard(QString &employeeName, QString &employeeRole)
{
    qDebug() << "\n=== DÉMARRAGE AUTHENTIFICATION CLAVIER ===";

    RFIDReader rfid;
    bool authenticated = false;

    if (!rfid.startRFID()) {
        QMessageBox::critical(nullptr, "ERREUR",
                              "IMPOSSIBLE DE SE CONNECTER À L'ARDUINO!");
        return false;
    }

    // Dialogue clavier
    QDialog keyboardDialog;
    keyboardDialog.setWindowTitle("Authentification Clavier");
    keyboardDialog.setFixedSize(450, 250);
    keyboardDialog.setStyleSheet(
        "QDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #0c0c0c, stop:0.5 #1a1a2e, stop:1 #16213e);"
        "    border: 2px solid #00ff00;"
        "    border-radius: 15px;"
        "}"
        );

    QVBoxLayout *layout = new QVBoxLayout(&keyboardDialog);

    QLabel *titleLabel = new QLabel("🔑 AUTHENTIFICATION CLAVIER");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #00ff00;");
    layout->addWidget(titleLabel);

    QLabel *instructionLabel = new QLabel(
        "Utilisez le clavier membrane Arduino:\n"
        "1. Entrez votre code PIN\n"
        "2. Appuyez sur '#' pour valider\n"
        "3. Appuyez sur '*' pour effacer\n\n"
        "Attente de saisie...");
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-size: 13px; color: #ffffff;");
    layout->addWidget(instructionLabel);

    QLabel *statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 12px; color: #ffff00;");
    layout->addWidget(statusLabel);

    QPushButton *cancelButton = new QPushButton("Annuler");
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #ff4444, stop:1 #cc0000);"
        "    border: 2px solid #ff0000;"
        "    color: #ffffff;"
        "    border-radius: 10px;"
        "    font-weight: bold;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover { background: #ff6666; }"
        );
    layout->addWidget(cancelButton);

    // Connexion mot de passe reçu
    QObject::connect(&rfid, &RFIDReader::passwordEntered,
                     [&](const QString &password) {
                         statusLabel->setText("Vérification en cours...");

                         if (rfid.checkPassword(password, employeeName, employeeRole)) {
                             authenticated = true;
                             qDebug() << "🎉 AUTHENTIFICATION CLAVIER RÉUSSIE!";

                             rfid.sendCommandToArduino("PASSWORD_OK:" + employeeName);

                             statusLabel->setText("✅ Authentification réussie!");
                             statusLabel->setStyleSheet("font-size: 12px; color: #00ff00; font-weight: bold;");

                             QTimer::singleShot(2000, &keyboardDialog, [&]() {
                                 keyboardDialog.accept();
                             });
                         } else {
                             rfid.sendCommandToArduino("PASSWORD_WRONG");

                             statusLabel->setText("❌ Mot de passe incorrect");
                             statusLabel->setStyleSheet("font-size: 12px; color: #ff0000; font-weight: bold;");

                             QTimer::singleShot(2000, &keyboardDialog, [&]() {
                                 statusLabel->setText("Réessayez...");
                                 statusLabel->setStyleSheet("font-size: 12px; color: #ffff00;");
                            });
                         }
                     });

    QObject::connect(cancelButton, &QPushButton::clicked, [&]() {
        keyboardDialog.reject();
    });

    qDebug() << "⌨ En attente de saisie clavier...";
    keyboardDialog.exec();

    rfid.stopRFID();

    if (authenticated) {
        qDebug() << "✅ Authentification clavier réussie pour:" << employeeName;
    }

    return authenticated;
}

// ==================== CLASSE DÉTECTEUR DE FLAMME ====================

FlameDetector::FlameDetector(QObject *parent) : QObject(parent), 
    serialPort(new QSerialPort(this)), 
    flameActive(false),
    currentLocalisation("Site Principal")
{
    connect(serialPort, &QSerialPort::readyRead, this, &FlameDetector::readData);
}

FlameDetector::~FlameDetector()
{
    stopDetection();
    delete serialPort;
}

bool FlameDetector::startDetection()
{
    QStringList portNames = {"COM6", "COM11", "COM4", "COM5", "COM3", "COM7", "COM8", "COM9", "COM10"};

    foreach (const QString &portName, portNames) {
        qDebug() << "🔥 Tentative connexion détecteur flamme sur" << portName << "...";
        serialPort->setPortName(portName);
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << "🔥✅ DÉTECTEUR FLAMME CONNECTÉ sur" << portName;
            serialPort->clear();
            emit connectionStatusChanged(true);
            return true;
        }
    }

    qDebug() << "❌ Impossible de connecter le détecteur de flamme";
    emit connectionStatusChanged(false);
    emit errorOccurred("Aucun port COM disponible pour le détecteur de flamme");
    return false;
}

void FlameDetector::stopDetection()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "🔥🔴 Détecteur de flamme déconnecté";
        emit connectionStatusChanged(false);
    }
}

bool FlameDetector::isConnected() const
{
    return serialPort && serialPort->isOpen();
}

void FlameDetector::readData()
{
    static QByteArray buffer;

    if (!serialPort || !serialPort->isOpen()) {
        return;
    }

    buffer.append(serialPort->readAll());

    while (true) {
        int lineEnd = buffer.indexOf('\n');
        if (lineEnd == -1) break;

        QByteArray lineData = buffer.left(lineEnd);
        if (lineData.endsWith('\r')) {
            lineData.chop(1);
        }
        buffer = buffer.mid(lineEnd + 1);

        QString line = QString::fromUtf8(lineData).trimmed();
        if (!line.isEmpty()) {
            qDebug() << "🔥 Arduino Flamme →" << line;
            processReceivedLine(line);
        }
    }

    if (buffer.size() > 1024) {
        buffer.clear();
    }
}

void FlameDetector::processReceivedLine(const QString &line)
{
    QString trimmedLine = line.trimmed().toUpper();

    // Détection de flamme - Arduino envoie "FLAME:DETECTED" ou "FLAMME:DETECTEE"
    if (trimmedLine.contains("FLAME:DETECTED") || 
        trimmedLine.contains("FLAMME:DETECTEE") ||
        trimmedLine.contains("ALERTE") ||
        trimmedLine.contains("FEU")) {
        
        if (!flameActive) {
            flameActive = true;
            qDebug() << "🔥🔥🔥 FLAMME DÉTECTÉE! Mise à jour base de données...";
            
            // Mettre à jour la base de données
            if (updateMaterielEtatDanger(currentLocalisation)) {
                emit databaseUpdated("État matériel mis à jour: EN DANGER");
            }
            
            emit flameDetected(currentLocalisation);
        }
    }
    // Flamme disparue - Arduino envoie "FLAME:CLEAR" ou "FLAMME:OK"
    else if (trimmedLine.contains("FLAME:CLEAR") || 
             trimmedLine.contains("FLAMME:OK") ||
             trimmedLine.contains("NORMAL")) {
        
        if (flameActive) {
            flameActive = false;
            qDebug() << "✅ Flamme disparue - Retour à la normale";
            emit flameCleared(currentLocalisation);
        }
    }
}

bool FlameDetector::updateMaterielEtatDanger(const QString &localisation)
{
    return updateAllMaterielOnSite(localisation, "EN DANGER");
}

bool FlameDetector::updateAllMaterielOnSite(const QString &localisation, const QString &nouvelEtat)
{
    Q_UNUSED(localisation);  // On va chercher "sur site" directement
    
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non connectée";
        emit errorOccurred("Base de données non connectée");
        return false;
    }

    // Trouver le nom de la table
    QStringList possibleNames = {"MATERIELS", "ATELIER", "MATERIEL", "GST_SPONSOR.MATERIEL", "GST_SPONSOR.ATELIER", "GST_SPONSOR.MATERIELS"};
    QString tableName;

    for (const QString &name : possibleNames) {
        QSqlQuery testQuery(db);
        if (testQuery.exec("SELECT COUNT(*) FROM " + name)) {
            tableName = name;
            qDebug() << "✅ Table trouvée:" << tableName;
            break;
        }
    }

    if (tableName.isEmpty()) {
        qDebug() << "❌ Table matériel non trouvée";
        emit errorOccurred("Table matériel non trouvée");
        return false;
    }

    qDebug() << "🔥 Mise à jour ETAT_MATERIEL → '" << nouvelEtat << "' pour matériels 'On site'";

    QSqlQuery query(db);
    
    // Mettre à jour tous les matériels qui sont "On site"
    query.prepare("UPDATE " + tableName + " SET ETAT_MATERIEL = :etat WHERE UPPER(LOCALISATION) LIKE '%ON SITE%'");
    query.bindValue(":etat", nouvelEtat);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL mise à jour:" << query.lastError().text();
        
        // Essayer avec d'autres variantes
        query.prepare("UPDATE " + tableName + " SET ETAT_MATERIEL = :etat WHERE UPPER(LOCALISATION) LIKE '%SITE%'");
        query.bindValue(":etat", nouvelEtat);
        
        if (!query.exec()) {
            qDebug() << "❌ Erreur SQL (2e tentative):" << query.lastError().text();
            emit errorOccurred("Erreur mise à jour base: " + query.lastError().text());
            return false;
        }
    }

    int rowsAffected = query.numRowsAffected();
    qDebug() << "✅ Mise à jour réussie!" << rowsAffected << "matériel(s) mis en état:" << nouvelEtat;
    
    // Si aucune ligne affectée, essayer de mettre à jour TOUS les matériels
    if (rowsAffected == 0) {
        qDebug() << "⚠️ Aucun matériel 'sur site' trouvé, mise à jour de TOUS les matériels...";
        query.prepare("UPDATE " + tableName + " SET ETAT_MATERIEL = :etat");
        query.bindValue(":etat", nouvelEtat);
        
        if (query.exec()) {
            rowsAffected = query.numRowsAffected();
            qDebug() << "✅ Tous les matériels mis à jour:" << rowsAffected;
        }
    }
    
    return rowsAffected > 0;
}

// ==================== FONCTION DE SURVEILLANCE INCENDIE ====================

void startFireMonitoring(const QString &localisation)
{
    qDebug() << "\n=== DÉMARRAGE SURVEILLANCE INCENDIE ===";
    qDebug() << "📍 Localisation surveillée:" << localisation;

    static FlameDetector* detector = nullptr;
    
    if (detector) {
        detector->stopDetection();
        delete detector;
    }
    
    detector = new FlameDetector();

    if (!detector->startDetection()) {
        QMessageBox::critical(nullptr, "ERREUR",
                              "IMPOSSIBLE DE CONNECTER LE DÉTECTEUR DE FLAMME!\n\n"
                              "Vérifiez que:\n"
                              "• L'Arduino est branché sur USB\n"
                              "• Le capteur de flamme est connecté\n"
                              "• Le bon port COM est disponible");
        return;
    }

    // Connexion des signaux
    QObject::connect(detector, &FlameDetector::flameDetected,
                     [localisation](const QString &loc) {
                         qDebug() << "🔥🔥🔥 ALERTE INCENDIE sur" << loc;
                         QMessageBox::critical(nullptr, "🔥 ALERTE INCENDIE 🔥",
                                               "FLAMME DÉTECTÉE!\n\n"
                                               "Localisation: " + localisation + "\n\n"
                                               "⚠️ Tous les matériels sur ce site\n"
                                               "ont été mis en état: EN DANGER\n\n"
                                               "ÉVACUEZ IMMÉDIATEMENT!");
                     });

    QObject::connect(detector, &FlameDetector::databaseUpdated,
                     [](const QString &message) {
                         qDebug() << "💾" << message;
                     });

    QObject::connect(detector, &FlameDetector::errorOccurred,
                     [](const QString &error) {
                         qDebug() << "❌ Erreur détecteur:" << error;
                     });

    qDebug() << "✅ Surveillance incendie active!";
    QMessageBox::information(nullptr, "Surveillance Active",
                             "🔥 Détecteur de flamme activé\n\n"
                             "Localisation: " + localisation + "\n\n"
                             "En cas de détection de flamme,\n"
                             "le matériel sera automatiquement\n"
                             "mis en état: EN DANGER");
}

// ==================== IMPLÉMENTATION CAPTEUR DE TEMPÉRATURE ====================

TemperatureSensor::TemperatureSensor(QObject *parent) 
    : QObject(parent),
      serialPort(nullptr),
      currentTemperature(0.0f),
      currentHumidity(0.0f),
      temperatureHighThreshold(35.0f),  // Alerte si > 35°C
      temperatureLowThreshold(5.0f),    // Alerte si < 5°C
      humidityHighThreshold(80.0f)      // Alerte si > 80% humidité
{
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &TemperatureSensor::readData);
}

TemperatureSensor::~TemperatureSensor()
{
    stopReading();
}

bool TemperatureSensor::startReading()
{
    // Chercher le port Arduino (CH340)
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QString portName;

    qDebug() << "\n🌡️ ==================== CONNEXION CAPTEUR TEMPÉRATURE ====================";
    qDebug() << "Ports disponibles:";

    for (const QSerialPortInfo &info : ports) {
        qDebug() << "  Port:" << info.portName() 
                 << "| Description:" << info.description()
                 << "| Fabricant:" << info.manufacturer();

        if (info.description().contains("CH340", Qt::CaseInsensitive) ||
            info.description().contains("USB-SERIAL", Qt::CaseInsensitive) ||
            info.description().contains("Arduino", Qt::CaseInsensitive) ||
            info.manufacturer().contains("wch", Qt::CaseInsensitive)) {
            portName = info.portName();
            qDebug() << "🌡️✅ Port Arduino détecté:" << portName;
        }
    }

    // Si pas trouvé, essayer COM7 par défaut
    if (portName.isEmpty()) {
        portName = "COM7";
        qDebug() << "⚠️ Port par défaut:" << portName;
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "🌡️✅ CAPTEUR TEMPÉRATURE CONNECTÉ sur" << portName;
        serialPort->clear();
        emit connectionStatusChanged(true);
        return true;
    }

    qDebug() << "❌ Impossible de connecter le capteur de température sur" << portName;
    emit connectionStatusChanged(false);
    emit errorOccurred("Impossible de se connecter au capteur de température");
    return false;
}

void TemperatureSensor::stopReading()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "🌡️🔴 Capteur de température déconnecté";
        emit connectionStatusChanged(false);
    }
}

bool TemperatureSensor::isConnected() const
{
    return serialPort && serialPort->isOpen();
}

void TemperatureSensor::readData()
{
    static QByteArray buffer;

    if (!serialPort || !serialPort->isOpen()) {
        return;
    }

    buffer.append(serialPort->readAll());

    while (true) {
        int lineEnd = buffer.indexOf('\n');
        if (lineEnd == -1) break;

        QByteArray lineData = buffer.left(lineEnd);
        if (lineData.endsWith('\r')) {
            lineData.chop(1);
        }
        buffer = buffer.mid(lineEnd + 1);

        QString line = QString::fromUtf8(lineData).trimmed();
        if (!line.isEmpty()) {
            qDebug() << "🌡️ Arduino Temp →" << line;
            processReceivedLine(line);
        }
    }

    if (buffer.size() > 1024) {
        buffer.clear();
    }
}

void TemperatureSensor::processReceivedLine(const QString &line)
{
    QString trimmedLine = line.trimmed();

    // Format attendu: "TEMP:25.5" ou "TEMPERATURE:25.5"
    if (trimmedLine.startsWith("TEMP:", Qt::CaseInsensitive) || 
        trimmedLine.startsWith("TEMPERATURE:", Qt::CaseInsensitive)) {
        
        QString valueStr = trimmedLine.mid(trimmedLine.indexOf(':') + 1).trimmed();
        bool ok;
        float temp = valueStr.toFloat(&ok);
        
        if (ok) {
            currentTemperature = temp;
            qDebug() << "🌡️ Température:" << currentTemperature << "°C";
            emit temperatureChanged(currentTemperature);
            checkThresholds();
        }
    }
    // Format attendu: "HUM:65.0" ou "HUMIDITY:65.0"
    else if (trimmedLine.startsWith("HUM:", Qt::CaseInsensitive) || 
             trimmedLine.startsWith("HUMIDITY:", Qt::CaseInsensitive)) {
        
        QString valueStr = trimmedLine.mid(trimmedLine.indexOf(':') + 1).trimmed();
        bool ok;
        float hum = valueStr.toFloat(&ok);
        
        if (ok) {
            currentHumidity = hum;
            qDebug() << "💧 Humidité:" << currentHumidity << "%";
            emit humidityChanged(currentHumidity);
            checkThresholds();
        }
    }
    // Format combiné: "TEMP:25.5,HUM:65.0"
    else if (trimmedLine.contains("TEMP:") && trimmedLine.contains("HUM:")) {
        QStringList parts = trimmedLine.split(',');
        for (const QString &part : parts) {
            processReceivedLine(part.trimmed());
        }
    }
    // Format JSON-like: {"temp":25.5,"hum":65.0}
    else if (trimmedLine.startsWith("{") && trimmedLine.endsWith("}")) {
        // Parse simple JSON
        if (trimmedLine.contains("temp")) {
            int tempIdx = trimmedLine.indexOf("temp");
            int colonIdx = trimmedLine.indexOf(':', tempIdx);
            int commaIdx = trimmedLine.indexOf(',', colonIdx);
            if (commaIdx == -1) commaIdx = trimmedLine.indexOf('}', colonIdx);
            
            QString tempStr = trimmedLine.mid(colonIdx + 1, commaIdx - colonIdx - 1).trimmed();
            bool ok;
            float temp = tempStr.toFloat(&ok);
            if (ok) {
                currentTemperature = temp;
                emit temperatureChanged(currentTemperature);
            }
        }
        if (trimmedLine.contains("hum")) {
            int humIdx = trimmedLine.indexOf("hum");
            int colonIdx = trimmedLine.indexOf(':', humIdx);
            int commaIdx = trimmedLine.indexOf(',', colonIdx);
            if (commaIdx == -1) commaIdx = trimmedLine.indexOf('}', colonIdx);
            
            QString humStr = trimmedLine.mid(colonIdx + 1, commaIdx - colonIdx - 1).trimmed();
            bool ok;
            float hum = humStr.toFloat(&ok);
            if (ok) {
                currentHumidity = hum;
                emit humidityChanged(currentHumidity);
            }
        }
        emit dataReceived(currentTemperature, currentHumidity);
        checkThresholds();
    }
}

void TemperatureSensor::checkThresholds()
{
    // Vérifier température haute
    if (currentTemperature > temperatureHighThreshold) {
        emit alertTriggered("TEMPÉRATURE ÉLEVÉE", currentTemperature);
        qDebug() << "⚠️🌡️ ALERTE: Température trop élevée!" << currentTemperature << "°C";
    }
    
    // Vérifier température basse
    if (currentTemperature < temperatureLowThreshold && currentTemperature > -40) {
        emit alertTriggered("TEMPÉRATURE BASSE", currentTemperature);
        qDebug() << "⚠️🌡️ ALERTE: Température trop basse!" << currentTemperature << "°C";
    }
    
    // Vérifier humidité haute
    if (currentHumidity > humidityHighThreshold) {
        emit alertTriggered("HUMIDITÉ ÉLEVÉE", currentHumidity);
        qDebug() << "⚠️💧 ALERTE: Humidité trop élevée!" << currentHumidity << "%";
    }
}
