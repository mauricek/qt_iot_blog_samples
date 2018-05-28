#include "sensorinformation.h"
#include "sensorinformationproto.h"
#include <QCoreApplication>
#include <QDataStream>
#include <QIODevice>
#include <QRandomGenerator>
#include <QtTest>

inline double moveValue(double currentValue, double min, double max, double maxStep)
{
    static QRandomGenerator generator;
    double result = currentValue + generator.bounded(maxStep * 2.) - maxStep;
    if (result < min)
        return min;
    else if (result > max)
        return max;
    return result;
}

class tst_Serialize : public QObject
{
    Q_OBJECT

public:
    tst_Serialize(QObject *parent = nullptr);

public slots:
    void initTestCase();
    void cleanupTestCase();
private slots:
    void dataStream();
    void json_text();
    void json_binary();
    void protobuf();
    void dataStreamExport();
    void dataStreamImport();
    void json_binaryExport();
    void json_binaryImport();
private:
    QVector<SensorInformation *> m_sensors;
    QVector<SensorInformationProto *> m_protoSensors;
    QVector<QByteArray> m_dataStreamExports;
    QVector<QByteArray> m_jsonExports;
};

tst_Serialize::tst_Serialize(QObject *parent)
    : QObject(parent)
{
}

void tst_Serialize::initTestCase()
{
    const int sensorCount = 10000;

    for (int s = 0; s < sensorCount; ++s) {
        m_sensors.append(new SensorInformation());
        // Randomize content
        for (int i = 0; i < 100; ++i) {
            m_sensors[s]->setAmbientTemperature(moveValue(m_sensors[s]->ambientTemperature(), -10., 40., 0.2));
            m_sensors[s]->setObjectTemperature(moveValue(m_sensors[s]->objectTemperature(), -10., 40, 0.2));
            m_sensors[s]->setLight(moveValue(m_sensors[s]->light(), 0., 1000., 100.));
            m_sensors[s]->setHumidity(moveValue(m_sensors[s]->humidity(), 0., 100., 1.));
            m_sensors[s]->setAltitude(moveValue(m_sensors[s]->altitude(), 0., 5000., 1.));
            m_sensors[s]->setAccelerometerX(moveValue(m_sensors[s]->accelerometerX(), -10., 10., 1.));
            m_sensors[s]->setAccelerometerY(moveValue(m_sensors[s]->accelerometerY(), -10., 10., 1.));
            m_sensors[s]->setAccelerometerZ(moveValue(m_sensors[s]->accelerometerZ(), -10., 10., 1.));
        }
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    for (int s = 0; s < sensorCount; ++s) {
        m_protoSensors.append(new SensorInformationProto());
        // Randomize content
        for (int i = 0; i < 100; ++i) {
            m_sensors[s]->setAmbientTemperature(moveValue(m_sensors[s]->ambientTemperature(), -10., 40., 0.2));
            m_sensors[s]->setObjectTemperature(moveValue(m_sensors[s]->objectTemperature(), -10., 40, 0.2));
            m_sensors[s]->setLight(moveValue(m_sensors[s]->light(), 0., 1000., 100.));
            m_sensors[s]->setHumidity(moveValue(m_sensors[s]->humidity(), 0., 100., 1.));
            m_sensors[s]->setAltitude(moveValue(m_sensors[s]->altitude(), 0., 5000., 1.));
            m_sensors[s]->setAccelerometerX(moveValue(m_sensors[s]->accelerometerX(), -10., 10., 1.));
            m_sensors[s]->setAccelerometerY(moveValue(m_sensors[s]->accelerometerY(), -10., 10., 1.));
            m_sensors[s]->setAccelerometerZ(moveValue(m_sensors[s]->accelerometerZ(), -10., 10., 1.));
        }
    }
}

void tst_Serialize::cleanupTestCase()
{
    while (m_sensors.size())
        delete m_sensors.takeFirst();

    google::protobuf::ShutdownProtobufLibrary();
}

void tst_Serialize::dataStream()
{
    double store = 0.;
    quint64 avgSize = 0;
    QBENCHMARK {
        avgSize = 0;
        for (int i = 0; i < m_sensors.size(); ++i) {
            QByteArray content;
            QDataStream writeStream(&content, QIODevice::WriteOnly);
            writeStream << *m_sensors[i];

            avgSize += content.size();
            QDataStream readStream(&content, QIODevice::ReadOnly);
            SensorInformation sensorImport;
            readStream >> sensorImport;

            if (sensorImport == *m_sensors[i])
                store += sensorImport.accelerometerX();
            else
                qFatal("Sensor class broken");
        }
    }
    avgSize /= m_sensors.size();
    qDebug() << "Average Message Size is:" << avgSize << ":" << m_sensors.size();
}

void tst_Serialize::json_text()
{
    double store = 0.;
    quint64 avgSize = 0;
    QBENCHMARK {
        avgSize = 0;
        for (int i = 0; i < m_sensors.size(); ++i) {
            const QByteArray jcontent = m_sensors[i]->exportToJson();
            avgSize += jcontent.size();
            SensorInformation sensorJImport(jcontent, false);
            if (sensorJImport == *m_sensors[i])
                store += sensorJImport.accelerometerX();
            else
                qFatal("Sensor class broken");
        }
    }
    avgSize /= m_sensors.size();
    qDebug() << "Average Message Size is:" << avgSize << ":" << m_sensors.size();
}

void tst_Serialize::json_binary()
{
    double store = 0.;
    quint64 avgSize = 0;
    QBENCHMARK {
        avgSize = 0;
        for (int i = 0; i < m_sensors.size(); ++i) {
            const QByteArray jcontent = m_sensors[i]->exportToJsonBinary();
            avgSize += jcontent.size();
            SensorInformation sensorJImport(jcontent, true);
            if (sensorJImport == *m_sensors[i])
                store += sensorJImport.accelerometerX();
            else
                qFatal("Sensor class broken");
        }
    }
    avgSize /= m_sensors.size();
    qDebug() << "Average Message Size is:" << avgSize << ":" << m_sensors.size();
}

void tst_Serialize::protobuf()
{
    double store = 0.;
    quint64 avgSize = 0;
    QBENCHMARK {
        avgSize = 0;
        for (int i = 0; i < m_protoSensors.size(); ++i) {
            const std::string pcontent = m_protoSensors[i]->serialize();
            avgSize += pcontent.size();
            SensorInformationProto sensorPImport(pcontent);
            if (sensorPImport == *m_protoSensors[i])
                store += sensorPImport.accelerometerX();
            else
                qFatal("Sensor class broken");
        }
    }
    avgSize /= m_sensors.size();
    qDebug() << "Average Message Size is:" << avgSize << ":" << m_sensors.size();
}

void tst_Serialize::dataStreamExport()
{
    QBENCHMARK {
        for (int i = 0; i < m_sensors.size(); ++i) {
            QByteArray content;
            QDataStream writeStream(&content, QIODevice::WriteOnly);
            writeStream << *m_sensors[i];

            m_dataStreamExports.append(content);
        }
    }
}

void tst_Serialize::dataStreamImport()
{
    double store = 0.;
    QBENCHMARK {
        for (int i = 0; i < m_sensors.size(); ++i) {
            QDataStream readStream(&m_dataStreamExports[i], QIODevice::ReadOnly);
            SensorInformation sensorImport;
            readStream >> sensorImport;

            if (sensorImport == *m_sensors[i])
                store += sensorImport.accelerometerX();
            else
                qFatal("Sensor class broken");
        }
    }
}

void tst_Serialize::json_binaryExport()
{
    QBENCHMARK {
        for (int i = 0; i < m_sensors.size(); ++i) {
            m_jsonExports.append(m_sensors[i]->exportToJsonBinary());
        }
    }
}

void tst_Serialize::json_binaryImport()
{
    double store = 0.;
    QBENCHMARK {
        for (int i = 0; i < m_sensors.size(); ++i) {
            SensorInformation sensorImport(m_jsonExports[i], true);

            if (sensorImport == *m_sensors[i])
                store += sensorImport.accelerometerX();
            else
                qFatal("Sensor class broken");
        }
    }
}

QTEST_MAIN(tst_Serialize)
#include "main.moc"
