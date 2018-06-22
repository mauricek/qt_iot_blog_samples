#ifndef SENSORINFORMATION_H
#define SENSORINFORMATION_H

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtMqtt/QMqttSubscription>
#include <QtMqtt/QMqttMessage>

class SensorInformation : public QObject
{
    Q_OBJECT
public:
    explicit SensorInformation(QMqttSubscription *sub, QObject *parent = nullptr);

    ~SensorInformation();

    QByteArray serialize();
signals:
    void publishRequested(QByteArray);

public slots:
    void handleMessage(const QMqttMessage &msg);
    void publishToAzure();
private:
    QMqttSubscription *m_subscription{nullptr};
    QTimer *m_timer{nullptr};
    QMutex m_mutex;
    float m_gyroX{0.};
    float m_gyroY{0.};
    float m_gyroZ{0.};
    double m_ambient{0.};
    double m_object{0.};
    double m_humid{0.};
    double m_light{0.};
    double m_altitude{0.};
    double m_hpa{0.};
    double m_accelX{0.};
    double m_accelY{0.};
    double m_accelZ{0.};
    double m_magnetX{0.};
    double m_magnetY{0.};
    double m_magnetZ{0.};
    double m_rotationX{0.};
    double m_rotationY{0.};
    double m_rotationZ{0.};
};

#endif // SENSORINFORMATION_H
