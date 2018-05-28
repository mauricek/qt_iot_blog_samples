#ifndef SENSORINFORMATION_H
#define SENSORINFORMATION_H

#include <QtCore/QDataStream>
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

public:
    enum SensorProperties {
        AmbientTemperature = 0,
        ObjectTemperature,
        AccelerometerX,
        AccelerometerY,
        AccelerometerZ,
        Altitude,
        Light,
        Humidity
    };
    explicit SensorInformation(QObject *parent = nullptr);
    SensorInformation(const QByteArray &json, bool binary);

    double ambientTemperature() const;
    double objectTemperature() const;
    double accelerometerX() const;
    double accelerometerY() const;
    double accelerometerZ() const;
    double altitude() const;
    double light() const;
    double humidity() const;

    bool operator==(const SensorInformation &other) const;
signals:

    void ambientTemperatureChanged(double ambientTemperature);
    void objectTemperatureChanged(double objectTemperature);
    void accelerometerXChanged(double accelerometerX);
    void accelerometerYChanged(double accelerometerY);
    void accelerometerZChanged(double accelerometerZ);
    void altitudeChanged(double altitude);
    void lightChanged(double light);
    void humidityChanged(double humidity);

public slots:
    void setAmbientTemperature(double ambientTemperature);
    void setObjectTemperature(double objectTemperature);
    void setAccelerometerX(double accelerometerX);
    void setAccelerometerY(double accelerometerY);
    void setAccelerometerZ(double accelerometerZ);
    void setAltitude(double altitude);
    void setLight(double light);
    void setHumidity(double humidity);

    QByteArray exportToJson();
    QByteArray exportToJsonBinary();
private:
    friend QDataStream &operator<<(QDataStream &, const SensorInformation &);
    friend QDataStream &operator>>(QDataStream &, SensorInformation &);
    QString m_id;
    double m_ambientTemperature{10.};
    double m_objectTemperature{12.};
    double m_accelerometerX{0.};
    double m_accelerometerY{0.};
    double m_accelerometerZ{0.};
    double m_altitude{20.};
    double m_light{600.};
    double m_humidity{0.2};
};
QDataStream &operator<<(QDataStream &, const SensorInformation &);
QDataStream &operator>>(QDataStream &, SensorInformation &);

#endif // SENSORINFORMATION_H
