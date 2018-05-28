#include "sensorinformationproto.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QTimer>
#include <QtCore/QUuid>

SensorInformationProto::SensorInformationProto(QObject *parent) : QObject(parent)
{
    m_protoInfo.set_id(QUuid::createUuid().toString().mid(1, 8).toStdString());
    m_protoInfo.set_ambienttemperature(10.);
    m_protoInfo.set_objecttemperature(12.);
    m_protoInfo.set_accelerometerx(0.);
    m_protoInfo.set_accelerometery(0.);
    m_protoInfo.set_accelerometerz(0.);
    m_protoInfo.set_altitude(20.);
    m_protoInfo.set_light(600.);
    m_protoInfo.set_humidity(0.2);
}

SensorInformationProto::SensorInformationProto(const std::string &pr)
{
    m_protoInfo.ParseFromString(pr);
}

double SensorInformationProto::ambientTemperature() const
{
    return m_protoInfo.ambienttemperature();
}

double SensorInformationProto::objectTemperature() const
{
    return m_protoInfo.objecttemperature();
}

double SensorInformationProto::accelerometerX() const
{
    return m_protoInfo.accelerometerx();
}

double SensorInformationProto::accelerometerY() const
{
    return m_protoInfo.accelerometery();
}

double SensorInformationProto::accelerometerZ() const
{
    return m_protoInfo.accelerometerz();
}

double SensorInformationProto::altitude() const
{
    return m_protoInfo.altitude();
}

double SensorInformationProto::light() const
{
    return m_protoInfo.light();
}

double SensorInformationProto::humidity() const
{
    return m_protoInfo.humidity();
}

std::string SensorInformationProto::serialize() const
{
    std::string res;
    m_protoInfo.SerializeToString(&res);
    return res;
}

bool SensorInformationProto::operator==(const SensorInformationProto &other) const
{
    return m_protoInfo.id() == other.m_protoInfo.id()
            && m_protoInfo.ambienttemperature() == other.m_protoInfo.ambienttemperature()
            && m_protoInfo.objecttemperature() == other.m_protoInfo.objecttemperature()
            && m_protoInfo.accelerometerx() == other.m_protoInfo.accelerometerx()
            && m_protoInfo.accelerometery() == other.m_protoInfo.accelerometery()
            && m_protoInfo.accelerometerz() == other.m_protoInfo.accelerometerz()
            && m_protoInfo.altitude() == other.m_protoInfo.altitude()
            && m_protoInfo.light() == other.m_protoInfo.light()
            && m_protoInfo.humidity() == other.m_protoInfo.humidity();
}

void SensorInformationProto::setAmbientTemperature(double ambientTemperature)
{
    if (qFuzzyCompare(m_protoInfo.ambienttemperature(), ambientTemperature))
        return;

    m_protoInfo.set_ambienttemperature(ambientTemperature);
    emit ambientTemperatureChanged(ambientTemperature);
}

void SensorInformationProto::setObjectTemperature(double objectTemperature)
{
    if (qFuzzyCompare(m_protoInfo.objecttemperature(), objectTemperature))
        return;

    m_protoInfo.set_objecttemperature(objectTemperature);
    emit objectTemperatureChanged(objectTemperature);
}

void SensorInformationProto::setAccelerometerX(double accelerometerX)
{
    if (qFuzzyCompare(m_protoInfo.accelerometerx(), accelerometerX))
        return;

    m_protoInfo.set_accelerometerx(accelerometerX);
    emit accelerometerXChanged(accelerometerX);
}

void SensorInformationProto::setAccelerometerY(double accelerometerY)
{
    if (qFuzzyCompare(m_protoInfo.accelerometery(), accelerometerY))
        return;

    m_protoInfo.set_accelerometery(accelerometerY);
    emit accelerometerYChanged(accelerometerY);
}

void SensorInformationProto::setAccelerometerZ(double accelerometerZ)
{
    if (qFuzzyCompare(m_protoInfo.accelerometerz(), accelerometerZ))
        return;

    m_protoInfo.set_accelerometerz(accelerometerZ);
    emit accelerometerZChanged(accelerometerZ);
}

void SensorInformationProto::setAltitude(double altitude)
{
    if (qFuzzyCompare(m_protoInfo.altitude(), altitude))
        return;

    m_protoInfo.set_altitude(altitude);
    emit altitudeChanged(altitude);
}

void SensorInformationProto::setLight(double light)
{
    if (qFuzzyCompare(m_protoInfo.light(), light))
        return;

    m_protoInfo.set_light(light);
    emit lightChanged(light);
}

void SensorInformationProto::setHumidity(double humidity)
{
    if (qFuzzyCompare(m_protoInfo.humidity(), humidity))
        return;

    m_protoInfo.set_humidity(humidity);
    emit humidityChanged(humidity);
}
