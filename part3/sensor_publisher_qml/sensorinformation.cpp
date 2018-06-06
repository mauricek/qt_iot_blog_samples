#include "sensorinformation.h"
#include "sensorSupport.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>

SensorInformation::SensorInformation(QObject *parent) : QObject(parent)
{
    init();
}

void SensorInformation::init()
{
    const DDS_DomainId_t domainId = 0;
    DDSDomainParticipant *participant = nullptr;

    participant = DDSDomainParticipantFactory::get_instance()->create_participant(domainId,
                                                                                  DDS_PARTICIPANT_QOS_DEFAULT,
                                                                                  NULL,
                                                                                  DDS_STATUS_MASK_NONE);
    if (!participant)
        qFatal("Could not create participant.");

    DDSPublisher *publisher = participant->create_publisher(DDS_PUBLISHER_QOS_DEFAULT,
                                              NULL,
                                              DDS_STATUS_MASK_NONE);
    if (!publisher)
        qFatal("Could not create publisher.");

    const char *typeName = sensor_informationTypeSupport::get_type_name();
    DDS_ReturnCode_t ret = sensor_informationTypeSupport::register_type(participant, typeName);
    if (ret != DDS_RETCODE_OK)
        qFatal("Could not register type.");

    DDSTopic *topic = participant->create_topic("Sensor Information",
                                                typeName,
                                                DDS_TOPIC_QOS_DEFAULT,
                                                NULL,
                                                DDS_STATUS_MASK_NONE);
    if (!topic)
        qFatal("Could not create topic.");

    DDSDataWriter *writer = publisher->create_datawriter(topic,
                                                         DDS_DATAWRITER_QOS_DEFAULT,
                                                         NULL,
                                                         DDS_STATUS_MASK_NONE);
    if (!writer)
        qFatal("Could not create writer.");

    m_sensorWriter = sensor_informationDataWriter::narrow(writer);
    if (!m_sensorWriter)
        qFatal("Could not narrow to sensor_informationDataWriter.");

    // Create real data
    m_info = sensor_informationTypeSupport::create_data();
    m_id = QUuid::createUuid().toString().mid(1, 8);
    m_info->ID = qstrdup(qPrintable(m_id));

    // As sensors have a unique ID, register each sensor to reuse data
    m_handle = m_sensorWriter->register_instance(*m_info);
}

void SensorInformation::sync()
{
    DDS_ReturnCode_t ret = m_sensorWriter->write(*m_info, m_handle);
    if (ret != DDS_RETCODE_OK) {
        qDebug() << "Could not write data.";
    }

}

double SensorInformation::ambientTemperature() const
{
    return m_info->ambientTemperature;
}

double SensorInformation::objectTemperature() const
{
    return m_info->objectTemperature;
}

double SensorInformation::accelerometerX() const
{
    return m_info->accelerometerX;
}

double SensorInformation::accelerometerY() const
{
    return m_info->accelerometerY;
}

double SensorInformation::accelerometerZ() const
{
    return m_info->accelerometerZ;
}

double SensorInformation::altitude() const
{
    return m_info->altitude;
}

double SensorInformation::light() const
{
    return m_info->light;
}

double SensorInformation::humidity() const
{
    return m_info->humidity;
}

QString SensorInformation::identifier() const
{
    return m_id;
}

void SensorInformation::setAmbientTemperature(double ambientTemperature)
{
    if (qFuzzyCompare(m_info->ambientTemperature, ambientTemperature))
        return;

    m_info->ambientTemperature = ambientTemperature;
    emit ambientTemperatureChanged(ambientTemperature);
    sync();
}

void SensorInformation::setObjectTemperature(double objectTemperature)
{
    if (qFuzzyCompare(m_info->objectTemperature, objectTemperature))
        return;

    m_info->objectTemperature = objectTemperature;
    emit objectTemperatureChanged(objectTemperature);
    sync();
}

void SensorInformation::setAccelerometerX(double accelerometerX)
{
    if (qFuzzyCompare(m_info->accelerometerX, accelerometerX))
        return;

    m_info->accelerometerX = accelerometerX;
    emit accelerometerXChanged(accelerometerX);
    sync();
}

void SensorInformation::setAccelerometerY(double accelerometerY)
{
    if (qFuzzyCompare(m_info->accelerometerY, accelerometerY))
        return;

    m_info->accelerometerY = accelerometerY;
    emit accelerometerYChanged(accelerometerY);
    sync();
}

void SensorInformation::setAccelerometerZ(double accelerometerZ)
{
    if (qFuzzyCompare(m_info->accelerometerZ, accelerometerZ))
        return;

    m_info->accelerometerZ = accelerometerZ;
    emit accelerometerZChanged(accelerometerZ);
    sync();
}

void SensorInformation::setAltitude(double altitude)
{
    if (qFuzzyCompare(m_info->altitude, altitude))
        return;

    m_info->altitude = altitude;
    emit altitudeChanged(altitude);
    sync();
}

void SensorInformation::setLight(double light)
{
    if (qFuzzyCompare(m_info->light, light))
        return;

    m_info->light = light;
    emit lightChanged(light);
    sync();
}

void SensorInformation::setHumidity(double humidity)
{
    if (qFuzzyCompare(m_info->humidity, humidity))
        return;

    m_info->humidity = humidity;
    emit humidityChanged(humidity);
    sync();
}

void SensorInformation::setIdentifier(const QString &id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit identifierChanged(m_id);
    sync();
}
