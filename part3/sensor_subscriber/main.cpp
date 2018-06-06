#include "sensor.h"
#include "sensorSupport.h"

#include <QCoreApplication>
#include <QDebug>

QDebug operator<<(QDebug d, const sensor_information &sensor)
{
    d << "Sensor(ID:" << sensor.ID
      << " Temp:" << sensor.ambientTemperature << "/" << sensor.objectTemperature
      << " Accel:" << sensor.accelerometerX << "/" << sensor.accelerometerY << "/" << sensor.accelerometerZ
      << " Humidity:" << sensor.humidity
      << " Light:" << sensor.light
      << " Alt:" << sensor.altitude
      << ")";
    return d;
}

class ReaderListener : public DDSDataReaderListener {
  public:
    ReaderListener() : DDSDataReaderListener()
    {
        qDebug() << Q_FUNC_INFO;
    }
    void on_requested_deadline_missed(DDSDataReader *, const DDS_RequestedDeadlineMissedStatus &) override
    {
        qDebug() << Q_FUNC_INFO;
    }

    void on_requested_incompatible_qos(DDSDataReader *, const DDS_RequestedIncompatibleQosStatus &) override
    {
        qDebug() << Q_FUNC_INFO;
    }

    void on_sample_rejected(DDSDataReader *, const DDS_SampleRejectedStatus &) override
    {
        qDebug() << Q_FUNC_INFO;
    }

    void on_liveliness_changed(DDSDataReader *, const DDS_LivelinessChangedStatus &status) override
    {
        // Liveliness only reports availability, not the initial state of a sensor
        // Follow up changes are reported to on_data_available
        qDebug() << Q_FUNC_INFO << status.alive_count;
    }

    void on_sample_lost(DDSDataReader *, const DDS_SampleLostStatus &) override
    {
        qDebug() << Q_FUNC_INFO;
    }

    void on_subscription_matched(DDSDataReader *, const DDS_SubscriptionMatchedStatus &) override
    {
        qDebug() << Q_FUNC_INFO;
    }

    void on_data_available(DDSDataReader* reader) override;
};

void ReaderListener::on_data_available(DDSDataReader* reader)
{
    sensor_informationDataReader *sensorReader = sensor_informationDataReader::narrow(reader);

    if (!sensorReader) {
        qDebug() << "Could not acquire sensorReader";
        return;
    }

    sensor_informationSeq data;
    DDS_SampleInfoSeq info;
    DDS_ReturnCode_t ret = sensorReader->take(
        data, info, DDS_LENGTH_UNLIMITED,
        DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (ret == DDS_RETCODE_NO_DATA) {
        qDebug() << "No data, continue...";
        return;
    } else if (ret != DDS_RETCODE_OK) {
        qDebug() << "Could not receive data:" << ret;
        return;
    }

    for (int i = 0; i < data.length(); ++i) {
        if (info[i].valid_data)
            qDebug() << data[i];
        else
            qDebug() << "Received Metadata on:" << i;
    }

    ret = sensorReader->return_loan(data, info);
    if (ret != DDS_RETCODE_OK)
        qDebug() << "Could not return loan.";
}

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

    DDSSubscriber *subscriber = participant->create_subscriber(DDS_SUBSCRIBER_QOS_DEFAULT,
                                                               NULL,
                                                               DDS_STATUS_MASK_NONE);
    if (!subscriber) {
        qDebug() << "Could not create subscriber.";
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

    ReaderListener *listener = new ReaderListener();
    if (!listener) {
        qDebug() << "Could not create listener.";
        return -5;
    }

    DDSDataReader *reader = subscriber->create_datareader(topic,
                                                          DDS_DATAREADER_QOS_DEFAULT,
                                                          listener,
                                                          DDS_LIVELINESS_CHANGED_STATUS |
                                                          DDS_DATA_AVAILABLE_STATUS);
    if (!reader) {
        qDebug() << "Could not create reader.";
        return -6;
    }

    qDebug() << "Subscribed, now waiting...";

    return a.exec();
}
