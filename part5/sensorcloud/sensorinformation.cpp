#include "sensorinformation.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

SensorInformation::SensorInformation(QMqttSubscription *sub, QObject *parent)
    : QObject(parent)
    , m_subscription(sub)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);

    connect(m_timer, &QTimer::timeout, this, &SensorInformation::publishToAzure);
    connect(m_subscription, &QMqttSubscription::messageReceived, this, &SensorInformation::handleMessage);
}

SensorInformation::~SensorInformation()
{
    m_timer->stop();
    m_subscription->unsubscribe();
}

QByteArray SensorInformation::serialize()
{
    const QString deviceName = m_subscription->topic().filter().split(QLatin1Char('/')).at(1);

    static int messageCounter = 0;
    QJsonObject jobject;
    jobject["Id"] = deviceName + QLatin1String("_") + QString::number(messageCounter++);
    jobject["SensorName"] = deviceName;
    jobject["GyroX"] = QString::number(m_gyroX);
    jobject["GyroY"] = QString::number(m_gyroY);
    jobject["GyroZ"] = QString::number(m_gyroZ);
    jobject["AmbientTemperature"] = QString::number(m_ambient);
    jobject["ObjectTemperature"] = QString::number(m_object);
    jobject["Humidity"] = QString::number(m_humid);
    jobject["Lux"] = QString::number(m_light);
    jobject["Altitude"] = QString::number(m_altitude);
    jobject["Pressure"] = QString::number(m_hpa);
    jobject["AccelerometerX"] = QString::number(m_accelX);
    jobject["AccelerometerY"] = QString::number(m_accelY);
    jobject["AccelerometerZ"] = QString::number(m_accelZ);
    jobject["MagnetometerX"] = QString::number(m_magnetX);
    jobject["MagnetometerY"] = QString::number(m_magnetY);
    jobject["MagnetometerZ"] = QString::number(m_magnetZ);
    jobject["RotationX"] = QString::number(m_rotationX);
    jobject["RotationY"] = QString::number(m_rotationY);
    jobject["RotationZ"] = QString::number(m_rotationZ);

    QJsonDocument doc(jobject);
    return doc.toJson();
}

void SensorInformation::handleMessage(const QMqttMessage &msg)
{
    if (!m_timer->isActive())
        m_timer->start(5000);

    QMutexLocker l(&m_mutex);

    const QString topic = msg.topic().name();
    if (topic.endsWith("gyro")) {
        auto items = msg.payload().split('_');
        m_gyroX = items.at(0).toFloat();
        m_gyroY = items.at(1).toFloat();
        m_gyroZ = items.at(2).toFloat();
    } else if (topic.endsWith("temperature")) {
        auto items = msg.payload().split('_');
        m_ambient = items.at(0).toDouble();
        m_object = items.at(1).toDouble();
    } else if (topic.endsWith("humid")) {
        m_humid = msg.payload().toDouble();
    } else if (topic.endsWith("light")) {
        m_light = msg.payload().toDouble();
    } else if (topic.endsWith("altitude")) {
        m_altitude = msg.payload().toDouble();
    } else if (topic.endsWith("barometer")) {
        m_hpa = msg.payload().toDouble();
    } else if (topic.endsWith("accel")) {
        auto items = msg.payload().split('_');
        m_accelX = items.at(0).toDouble();
        m_accelY = items.at(1).toDouble();
        m_accelZ = items.at(2).toDouble();
    } else if (topic.endsWith("magnet")) {
        auto items = msg.payload().split('_');
        m_magnetX = items.at(0).toDouble();
        m_magnetY = items.at(1).toDouble();
        m_magnetZ = items.at(2).toDouble();
    } else if (topic.endsWith("rotation")) {
        auto items = msg.payload().split('_');
        m_rotationX = items.at(0).toDouble();
        m_rotationY = items.at(1).toDouble();
        m_rotationZ = items.at(2).toDouble();
    }
}

void SensorInformation::publishToAzure()
{
    QMutexLocker l(&m_mutex);
    const QByteArray data = serialize();
    emit publishRequested(data);
}
