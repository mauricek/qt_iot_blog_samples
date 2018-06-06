#include "sensor.h"
#include "sensorSupport.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const DDS_DomainId_t domainId = 0;
    DDSDomainParticipant *participant = nullptr;

    participant = DDSDomainParticipantFactory::get_instance()->create_participant(domainId,
                                                                                  DDS_PARTICIPANT_QOS_DEFAULT,
                                                                                  NULL,
                                                                                  DDS_STATUS_MASK_NONE);
    if (!participant) {
        qDebug() << "Could not create participant.";
        return -1;
    }

    DDSPublisher *publisher = participant->create_publisher(DDS_PUBLISHER_QOS_DEFAULT,
                                              NULL,
                                              DDS_STATUS_MASK_NONE);
    if (!publisher) {
        qDebug() << "Could not create publisher.";
        return -2;
    }

    const char *typeName = sensor_informationTypeSupport::get_type_name();
    DDS_ReturnCode_t ret = sensor_informationTypeSupport::register_type(participant, typeName);
    if (ret != DDS_RETCODE_OK) {
        qDebug() << "Could not register type.";
        return -3;
    }

    DDSTopic *topic = participant->create_topic("Sensor Information",
                                                typeName,
                                                DDS_TOPIC_QOS_DEFAULT,
                                                NULL,
                                                DDS_STATUS_MASK_NONE);
    if (!topic) {
        qDebug() << "Could not create topic.";
        return -4;
    }

    DDSDataWriter *writer = publisher->create_datawriter(topic,
                                                         DDS_DATAWRITER_QOS_DEFAULT,
                                                         NULL,
                                                         DDS_STATUS_MASK_NONE);
    if (!writer) {
        qDebug() << "Could not create writer.";
        return -5;
    }

    sensor_informationDataWriter *sensorWriter = sensor_informationDataWriter::narrow(writer);
    if (!sensorWriter) {
        qDebug() << "Could not narrow to sensor_informationDataWriter.";
        return -6;
    }

    // Create real data
    sensor_information *sensorInformation = sensor_informationTypeSupport::create_data();
    sensorInformation->ID = qstrdup("Foobidoo");
    sensorInformation->ambientTemperature = 12.;
    // As sensors have a unique ID, register each sensor to reuse data
    DDS_InstanceHandle_t sensorHandle = sensorWriter->register_instance(*sensorInformation);

    // Send data twice within a one second interval
    ret = sensorWriter->write(*sensorInformation, sensorHandle);
    if (ret != DDS_RETCODE_OK) {
        qDebug() << "Could not write data.";
    }
    DDS_Duration_t send_period{1,0};
    NDDSUtility::sleep(send_period);
    sensorInformation->ambientTemperature = 14.;

    ret = sensorWriter->write(*sensorInformation, sensorHandle);
    if (ret != DDS_RETCODE_OK) {
        qDebug() << "Could not write data.";
    }

    qDebug() << "Publishing done.";

    return a.exec();
}
