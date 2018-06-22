#include "brokerconnection.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#define MQTT_BROKER ""
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

BrokerConnection::BrokerConnection(QObject *parent) : QObject(parent)
{
}

void BrokerConnection::setAzure(AzureConnection *az)
{
    m_azure = az;
}

void BrokerConnection::init()
{
    if (!m_client)
        m_client = new QMqttClient;

    m_client->setHostname(MQTT_BROKER);
    m_client->setPort(MQTT_PORT);
    m_client->setUsername(MQTT_USERNAME);
    m_client->setPassword(MQTT_PASSWORD);
    connect(m_client, &QMqttClient::connected, this, &BrokerConnection::connectedToBroker);
    m_client->connectToHost();
}

void BrokerConnection::connectedToBroker()
{
    m_activeSub = m_client->subscribe(QLatin1String("sensors/active"), 1);
    if (!m_activeSub) {
        qWarning() << "Could not create subscription to sensors/active";
        return;
    }

    connect(m_activeSub, &QMqttSubscription::messageReceived, this, &BrokerConnection::activeClientMessageReceived);
}

void BrokerConnection::activeClientMessageReceived(const QMqttMessage &msg)
{
    const QByteArray content = msg.payload();
    auto split = content.split('>');
    if (split.size() != 2) {
        qWarning() << "Received unknown message type";
        return;
    }

    if (split.at(1) == "Online" && !m_sensors.contains(split.at(0))) {
        const QString subName = QString::fromLocal8Bit("sensors/%1/#").arg(QString::fromUtf8(split.at(0)));
        auto sub = m_client->subscribe(subName, 1);
        if (!sub) {
            qWarning() << "Could not subscribe to sensordetails of " << split.at(0);
            return;
        }
        auto sensor = new SensorInformation(sub, this);
        connect(sensor, &SensorInformation::publishRequested, m_azure, &AzureConnection::publishMessage);
        m_sensors.insert(split.at(0), sensor);
    } else if (split.at(1) == "Offline") {
        if (!m_sensors.contains(split.at(0))) {
            qWarning() << "Unknown sensor went offline:" << split.at(0);
            return;
        }
        m_sensors.take(split.at(0))->deleteLater();
    } else {
        // Either not understood or rather no changes happened
        return;
    }

    // Send update message to azure on device count
    static int messageCounter = 0;
    QJsonObject jobject;
    jobject["Id"] = QLatin1String("Active_") + QString::number(messageCounter++);
    jobject["ActiveDevices"] = QString::number(m_sensors.size());
    QJsonDocument doc( jobject );

    m_azure->publishMessage(doc.toJson());
}
