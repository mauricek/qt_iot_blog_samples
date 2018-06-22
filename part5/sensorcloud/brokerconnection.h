#ifndef BROKERCONNECTION_H
#define BROKERCONNECTION_H

#include "AzureConnection.h"
#include "sensorinformation.h"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtMqtt/QMqttClient>

class BrokerConnection : public QObject
{
    Q_OBJECT
public:
    explicit BrokerConnection(QObject *parent = nullptr);

    void setAzure(AzureConnection *az);

public slots:
    void init();
    void connectedToBroker();
    void activeClientMessageReceived(const QMqttMessage &msg);

private:
    QMqttClient *m_client{nullptr};
    AzureConnection *m_azure{nullptr};
    QMqttSubscription *m_activeSub{nullptr};
    QMap<QByteArray, SensorInformation *> m_sensors;
};

#endif // BROKERCONNECTION_H
