#ifndef SENSORINFORMATIONDDS_H
#define SENSORINFORMATIONDDS_H

#include "sensor.h"

#include <QtCore/QObject>
#include <QtCore/QString>

class SensorInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double ambientTemperature READ ambientTemperature WRITE setAmbientTemperature NOTIFY ambientTemperatureChanged)
    Q_PROPERTY(double objectTemperature READ objectTemperature WRITE setObjectTemperature NOTIFY objectTemperatureChanged)
    Q_PROPERTY(double accelerometerX READ accelerometerX WRITE setAccelerometerX NOTIFY accelerometerXChanged)
    Q_PROPERTY(double accelerometerY READ accelerometerY WRITE setAccelerometerY NOTIFY accelerometerYChanged)
    Q_PROPERTY(double accelerometerZ READ accelerometerZ WRITE setAccelerometerZ NOTIFY accelerometerZChanged)
    Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(double light READ light WRITE setLight NOTIFY lightChanged)
    Q_PROPERTY(double humidity READ humidity WRITE setHumidity NOTIFY humidityChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)

public:
    explicit SensorInformation(QObject *parent = nullptr);
    void init();
    void sync();

    double ambientTemperature() const;
    double objectTemperature() const;
    double accelerometerX() const;
    double accelerometerY() const;
    double accelerometerZ() const;
    double altitude() const;
    double light() const;
    double humidity() const;
    QString identifier() const;

signals:
    void ambientTemperatureChanged(double ambientTemperature);
    void objectTemperatureChanged(double objectTemperature);
    void accelerometerXChanged(double accelerometerX);
    void accelerometerYChanged(double accelerometerY);
    void accelerometerZChanged(double accelerometerZ);
    void altitudeChanged(double altitude);
    void lightChanged(double light);
    void humidityChanged(double humidity);
    void identifierChanged(QString id);

public slots:
    void setAmbientTemperature(double ambientTemperature);
    void setObjectTemperature(double objectTemperature);
    void setAccelerometerX(double accelerometerX);
    void setAccelerometerY(double accelerometerY);
    void setAccelerometerZ(double accelerometerZ);
    void setAltitude(double altitude);
    void setLight(double light);
    void setHumidity(double humidity);
    void setIdentifier(const QString &id);

private:
    sensor_information *m_info;
    sensor_informationDataWriter *m_sensorWriter;
    DDS_InstanceHandle_t m_handle;
    QString m_id;
};

#endif // SENSORINFORMATIONDDS_H
