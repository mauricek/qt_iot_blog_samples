#include "sensorinformation.h"

#include <QtCore/QRandomGenerator>
#include <QtCore/QTimer>
#include <QtCore/QUuid>

#include <QtMqtt/QMqttClient>

inline double moveValue(double currentValue, double min, double max, double maxStep)
{
    static QRandomGenerator generator;
    double result = currentValue + generator.bounded(maxStep * 2.) - maxStep;
    if (result < min)
        return min;
    else if (result > max)
        return max;
    return result;
}

SensorInformation::SensorInformation(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid().toString();

    m_fiveSecondTimer = new QTimer(this);
    m_fiveSecondTimer->setInterval(5000);
    m_fiveSecondTimer->setSingleShot(false);
    connect(m_fiveSecondTimer, &QTimer::timeout, [this]() {
        // Every five seconds report device still online
        if (m_client->state() == QMqttClient::Connected) {
            const QString content = m_id + QLatin1String(">Online");
            m_client->publish(QLatin1String("qtdemosensors/active"), content.toUtf8());
        }
    });

    m_secondTimer = new QTimer(this);
    m_secondTimer->setInterval(1000);
    m_secondTimer->setSingleShot(false);
    connect(m_secondTimer, &QTimer::timeout, [this]() {
        setAmbientTemperature(moveValue(ambientTemperature(), -10., 40., 0.2));
        setObjectTemperature(moveValue(objectTemperature(), -10., 40, 0.2));
        setLight(moveValue(light(), 0., 1000., 100.));
        setHumidity(moveValue(humidity(), 0., 100., 1.));
        setAltitude(moveValue(altitude(), 0., 5000., 1.));
    });

    m_tenMsTimer = new QTimer(this);
    connect(m_tenMsTimer, &QTimer::timeout, [this]() {
        setAccelerometerX(moveValue(accelerometerX(), -10., 10., 1.));
        setAccelerometerY(moveValue(accelerometerY(), -10., 10., 1.));
        setAccelerometerZ(moveValue(accelerometerZ(), -10., 10., 1.));
    });
    m_tenMsTimer->setInterval(20);
    m_tenMsTimer->setSingleShot(false);

    m_client = new QMqttClient(this);
    m_client->setWillTopic(QLatin1String("qtdemosensors/active"));
    const QString will = m_id + QLatin1String(">Offline");
    m_client->setWillMessage(will.toUtf8());
    m_client->setHostname("test.mosquitto.org");
    m_client->setPort(1883);
    connect(m_client, &QMqttClient::stateChanged, [](QMqttClient::ClientState s) {
        qDebug() << "Client state changed:" << s;
    });

}

void SensorInformation::start()
{
    m_client->connectToHost();
    m_fiveSecondTimer->start();
    m_secondTimer->start();
    m_tenMsTimer->start();
}

double SensorInformation::ambientTemperature() const
{
    return m_ambientTemperature;
}

double SensorInformation::objectTemperature() const
{
    return m_objectTemperature;
}

double SensorInformation::accelerometerX() const
{
    return m_accelerometerX;
}

double SensorInformation::accelerometerY() const
{
    return m_accelerometerY;
}

double SensorInformation::accelerometerZ() const
{
    return m_accelerometerZ;
}

double SensorInformation::altitude() const
{
    return m_altitude;
}

double SensorInformation::light() const
{
    return m_light;
}

double SensorInformation::humidity() const
{
    return m_humidity;
}

void SensorInformation::setAmbientTemperature(double ambientTemperature)
{
    if (qFuzzyCompare(m_ambientTemperature, ambientTemperature))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/ambientTemperature").arg(m_id),
                      QByteArray::number(ambientTemperature));
    m_ambientTemperature = ambientTemperature;
    emit ambientTemperatureChanged(m_ambientTemperature);
}

void SensorInformation::setObjectTemperature(double objectTemperature)
{
    if (qFuzzyCompare(m_objectTemperature, objectTemperature))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/objectTemperature").arg(m_id),
                      QByteArray::number(objectTemperature));
    m_objectTemperature = objectTemperature;
    emit objectTemperatureChanged(m_objectTemperature);
}

void SensorInformation::setAccelerometerX(double accelerometerX)
{
    if (qFuzzyCompare(m_accelerometerX, accelerometerX))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/accelerometerX").arg(m_id),
                      QByteArray::number(accelerometerX));
    m_accelerometerX = accelerometerX;
    emit accelerometerXChanged(m_accelerometerX);
}

void SensorInformation::setAccelerometerY(double accelerometerY)
{
    if (qFuzzyCompare(m_accelerometerY, accelerometerY))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/accelerometerY").arg(m_id),
                      QByteArray::number(accelerometerY));
    m_accelerometerY = accelerometerY;
    emit accelerometerYChanged(m_accelerometerY);
}

void SensorInformation::setAccelerometerZ(double accelerometerZ)
{
    if (qFuzzyCompare(m_accelerometerZ, accelerometerZ))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/accelerometerZ").arg(m_id),
                      QByteArray::number(accelerometerZ));
    m_accelerometerZ = accelerometerZ;
    emit accelerometerZChanged(m_accelerometerZ);
}

void SensorInformation::setAltitude(double altitude)
{
    if (qFuzzyCompare(m_altitude, altitude))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/altitude").arg(m_id),
                      QByteArray::number(altitude));
    m_altitude = altitude;
    emit altitudeChanged(m_altitude);
}

void SensorInformation::setLight(double light)
{
    if (qFuzzyCompare(m_light, light))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/light").arg(m_id),
                      QByteArray::number(light));
    m_light = light;
    emit lightChanged(m_light);
}

void SensorInformation::setHumidity(double humidity)
{
    if (qFuzzyCompare(m_humidity, humidity))
        return;

    m_client->publish(QString::fromLatin1("qtdemosensors/%1/humidity").arg(m_id),
                      QByteArray::number(humidity));
    m_humidity = humidity;
    emit humidityChanged(m_humidity);
}
