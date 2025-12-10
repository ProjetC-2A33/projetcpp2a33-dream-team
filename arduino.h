#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// ==================== CLASSE ARDUINO SIMPLE ====================
class arduino
{
public:
    arduino();
    int write1_to_arduino(QByteArray);
    int connect_arduino();
    int close_arduino();
    int write_to_arduino(QByteArray);
    int writeStringToArduino(const char*);
    QByteArray read_from_arduino();

    // Getters
    QSerialPort* getserial();
    QString getarduino_port_name();
    
private:
    QSerialPort *serial;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_prod_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
};

// ==================== CLASSE RFID READER ====================
class RFIDReader : public QObject
{
    Q_OBJECT

public:
    explicit RFIDReader(QObject *parent = nullptr);
    ~RFIDReader();

    bool startRFID();
    void stopRFID();
    void sendCommandToArduino(const QString &command);

    QSqlDatabase getDatabase();
    bool checkUID(const QString &uid, QString &name, QString &role);
    bool checkPassword(const QString &password, QString &name, QString &role);

signals:
    void cardDetected(const QString &uid);
    void passwordEntered(const QString &password);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &message);
    void arduinoMessageReceived(const QString &line);
    void flameDetected(const QString &localisation);  // Signal pour détection flamme

private slots:
    void readData();

private:
    void processReceivedLine(const QString &line);
    void processRFIDLine(const QString &line);
    void processPasswordLine(const QString &line);
    void processFlammeLine(const QString &line);  // Traitement détection flamme
    void cleanupUID(QString &uid);

    QSerialPort *serialPort;
};

// ==================== CLASSE DÉTECTEUR DE FLAMME ====================
class FlameDetector : public QObject
{
    Q_OBJECT

public:
    explicit FlameDetector(QObject *parent = nullptr);
    ~FlameDetector();

    bool startDetection();
    void stopDetection();
    bool isConnected() const;
    
    // Mettre à jour l'état du matériel en base de données
    bool updateMaterielEtatDanger(const QString &localisation);
    bool updateAllMaterielOnSite(const QString &localisation, const QString &nouvelEtat);

signals:
    void flameDetected(const QString &localisation);
    void flameCleared(const QString &localisation);
    void connectionStatusChanged(bool connected);
    void databaseUpdated(const QString &message);
    void errorOccurred(const QString &message);

private slots:
    void readData();

private:
    void processReceivedLine(const QString &line);
    QSerialPort *serialPort;
    bool flameActive;
    QString currentLocalisation;
};

bool authenticateWithRFID(QString &employeeName, QString &employeeRole);
bool authenticateWithKeyboard(QString &employeeName, QString &employeeRole);

// Fonction pour démarrer la surveillance incendie
void startFireMonitoring(const QString &localisation);

// ==================== CLASSE CAPTEUR DE TEMPÉRATURE ====================
class TemperatureSensor : public QObject
{
    Q_OBJECT

public:
    explicit TemperatureSensor(QObject *parent = nullptr);
    ~TemperatureSensor();

    bool startReading();
    void stopReading();
    bool isConnected() const;
    
    // Getters pour les dernières valeurs
    float getTemperature() const { return currentTemperature; }
    float getHumidity() const { return currentHumidity; }

signals:
    void temperatureChanged(float temperature);
    void humidityChanged(float humidity);
    void dataReceived(float temperature, float humidity);
    void connectionStatusChanged(bool connected);
    void alertTriggered(const QString &alertType, float value);
    void errorOccurred(const QString &message);

private slots:
    void readData();

private:
    void processReceivedLine(const QString &line);
    void checkThresholds();
    
    QSerialPort *serialPort;
    float currentTemperature;
    float currentHumidity;
    
    // Seuils d'alerte
    float temperatureHighThreshold;
    float temperatureLowThreshold;
    float humidityHighThreshold;
};

#endif
//montassar

