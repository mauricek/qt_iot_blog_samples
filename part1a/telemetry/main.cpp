#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QMap>

#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttSubscription>

//const QLatin1String skTopicPrefix("qtdemosensors/");
const QLatin1String skTopicPrefix("qtds/");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QMqttClient *client = new QMqttClient();
    client->setHostname("test.mosquitto.org");
    client->setPort(1883);

    QMap<QByteArray, QMqttSubscription *> subscriptions;
    QMqttSubscription *availableDevices = nullptr;

    client->connect(client, &QMqttClient::connected, [&client, &subscriptions, &availableDevices]() {
        qDebug() << "Connected to broker.";
        availableDevices = client->subscribe(skTopicPrefix + "active");

        availableDevices->connect(availableDevices, &QMqttSubscription::messageReceived, [&client, &subscriptions](const QMqttMessage &msg) {
            QList<QByteArray> data = msg.payload().split('>');
            if (data.size() != 2) {
                qWarning() << "Invalid active sensor data received!:" << msg.payload();
                return;
            }
            if (data.at(1) == QByteArray("Online")) {
                if (!subscriptions.contains(data.at(0))) {
                    qDebug() << "Registering new device:" << data.at(0);
                    const QString deviceSubTopics = skTopicPrefix + data.at(0) + "/#";
                    auto sub = client->subscribe(deviceSubTopics);
                    client->connect(sub, &QMqttSubscription::messageReceived, [](const QMqttMessage &msg) {
                        qDebug() << "Update:" << msg.topic() << " : " << msg.payload();
                    });
                    subscriptions.insert(data.at(0), sub);
                }
            } else if (data.at(1) == QByteArray("Offline")) {
                if (subscriptions.contains(data.at(0))) {
                    qDebug() << "Removing existing device:" << data.at(0);
                    auto sub = subscriptions.take(data.at(0));
                    sub->unsubscribe();
                }
            }
        });
    });
    client->connectToHost();

    return a.exec();
}
