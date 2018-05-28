#include "sensorinformation.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

SensorInformation::SensorInformation(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid().toString().mid(1, 8);
}

SensorInformation::SensorInformation(const QByteArray &json, bool binary)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    m_id = obj["SensorID"].toString();
    m_ambientTemperature = obj["AmbientTemperature"].toDouble();
    m_objectTemperature = obj["ObjectTemperature"].toDouble();
    m_accelerometerX = obj["AccelerometerX"].toDouble();
    m_accelerometerY = obj["AccelerometerY"].toDouble();
    m_accelerometerZ = obj["AccelerometerZ"].toDouble();
    m_altitude = obj["Altitude"].toDouble();
    m_light = obj["Light"].toDouble();
    m_humidity = obj["Humidity"].toDouble();
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

bool SensorInformation::operator==(const SensorInformation &other) const
{
    return m_id == other.m_id
            && m_ambientTemperature == other.m_ambientTemperature
            && m_objectTemperature == other.m_objectTemperature
            && m_accelerometerX == other.m_accelerometerX
            && m_accelerometerY == other.m_accelerometerY
            && m_accelerometerZ == other.m_accelerometerZ
            && m_altitude == other.m_altitude
            && m_light == other.m_light
            && m_humidity == other.m_humidity;
}

void SensorInformation::setAmbientTemperature(double ambientTemperature)
{
    if (qFuzzyCompare(m_ambientTemperature, ambientTemperature))
        return;

    m_ambientTemperature = ambientTemperature;
    emit ambientTemperatureChanged(m_ambientTemperature);
}

void SensorInformation::setObjectTemperature(double objectTemperature)
{
    if (qFuzzyCompare(m_objectTemperature, objectTemperature))
        return;

    m_objectTemperature = objectTemperature;
    emit objectTemperatureChanged(m_objectTemperature);
}

void SensorInformation::setAccelerometerX(double accelerometerX)
{
    if (qFuzzyCompare(m_accelerometerX, accelerometerX))
        return;

    m_accelerometerX = accelerometerX;
    emit accelerometerXChanged(m_accelerometerX);
}

void SensorInformation::setAccelerometerY(double accelerometerY)
{
    if (qFuzzyCompare(m_accelerometerY, accelerometerY))
        return;

    m_accelerometerY = accelerometerY;
    emit accelerometerYChanged(m_accelerometerY);
}

void SensorInformation::setAccelerometerZ(double accelerometerZ)
{
    if (qFuzzyCompare(m_accelerometerZ, accelerometerZ))
        return;

    m_accelerometerZ = accelerometerZ;
    emit accelerometerZChanged(m_accelerometerZ);
}

void SensorInformation::setAltitude(double altitude)
{
    if (qFuzzyCompare(m_altitude, altitude))
        return;

    m_altitude = altitude;
    emit altitudeChanged(m_altitude);
}

void SensorInformation::setLight(double light)
{
    if (qFuzzyCompare(m_light, light))
        return;

    m_light = light;
    emit lightChanged(m_light);
}

void SensorInformation::setHumidity(double humidity)
{
    if (qFuzzyCompare(m_humidity, humidity))
        return;

    m_humidity = humidity;
    emit humidityChanged(m_humidity);
}

QByteArray SensorInformation::exportToJson()
{
    QJsonObject jobject;
    jobject["SensorID"] = m_id;
    jobject["AmbientTemperature"] = m_ambientTemperature;
    jobject["ObjectTemperature"] = m_objectTemperature;
    jobject["AccelerometerX"] = m_accelerometerX;
    jobject["AccelerometerY"] = m_accelerometerY;
    jobject["AccelerometerZ"] = m_accelerometerZ;
    jobject["Altitude"] = m_altitude;
    jobject["Light"] = m_light;
    jobject["Humidity"] = m_humidity;
    QJsonDocument doc( jobject );

    return doc.toJson(QJsonDocument::Compact);
    //return doc.toJson();
}

QByteArray SensorInformation::exportToJsonBinary()
{
    QJsonObject jobject;
    jobject["SensorID"] = m_id;
    jobject["AmbientTemperature"] = m_ambientTemperature;
    jobject["ObjectTemperature"] = m_objectTemperature;
    jobject["AccelerometerX"] = m_accelerometerX;
    jobject["AccelerometerY"] = m_accelerometerY;
    jobject["AccelerometerZ"] = m_accelerometerZ;
    jobject["Altitude"] = m_altitude;
    jobject["Light"] = m_light;
    jobject["Humidity"] = m_humidity;
    QJsonDocument doc( jobject );

    return doc.toBinaryData();
}

QDataStream &operator<<(QDataStream &out, const SensorInformation &item)
{
    QDataStream::FloatingPointPrecision prev = out.floatingPointPrecision();
    out.setFloatingPointPrecision(QDataStream::DoublePrecision);
    out << item.m_id
        << item.m_ambientTemperature
        << item.m_objectTemperature
        << item.m_accelerometerX
        << item.m_accelerometerY
        << item.m_accelerometerZ
        << item.m_altitude
        << item.m_light
        << item.m_humidity;
    out.setFloatingPointPrecision(prev);
    return out;
}

QDataStream &operator>>(QDataStream &in, SensorInformation &item)
{
    QDataStream::FloatingPointPrecision prev = in.floatingPointPrecision();
    in.setFloatingPointPrecision(QDataStream::DoublePrecision);

    in >> item.m_id
            >> item.m_ambientTemperature
            >> item.m_objectTemperature
            >> item.m_accelerometerX
            >> item.m_accelerometerY
            >> item.m_accelerometerZ
            >> item.m_altitude
            >> item.m_light
            >> item.m_humidity;

    in.setFloatingPointPrecision(prev);
    return in;
}
