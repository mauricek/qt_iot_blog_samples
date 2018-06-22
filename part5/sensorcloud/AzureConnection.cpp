#include "AzureConnection.h"

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>

#include <stdio.h>
#include <stdlib.h>

#include "azure_c_shared_utility/platform.h"
#include "iothub_client.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "iothubtransportamqp.h"

typedef struct EVENT_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;
} EVENT_INSTANCE;

class AzureConnectionThread;

class AzureWorker
{
public:
    explicit AzureWorker(AzureConnectionThread *thread);

    bool init(const QString &connectionString);
    bool publishMessage(const QByteArray &inMessage);
    bool update();
    bool release();

    void messageReceived(const QByteArray &message);

private:
    IOTHUB_CLIENT_LL_HANDLE m_iotClientHandle{nullptr};
    size_t m_messageId{0};
    AzureConnectionThread *m_connectionThread;
};

class AzureConnectionThread : public QThread
{
    Q_OBJECT
public:
    explicit AzureConnectionThread(const QString &connectionString);
    ~AzureConnectionThread();

    void stop();
    void publishMessage(const QByteArray &message);

Q_SIGNALS:
    void messageReceived(QByteArray message);
    void messageSent();
    void messageError();

protected:
    void run() override;

private:
    bool isRunning();
    QString m_connectionString;
    QMutex m_mutex;
    bool m_running{true};
    QQueue<QByteArray> m_messages;
};

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    Q_UNUSED(result)
    EVENT_INSTANCE *eventInstance = (EVENT_INSTANCE *)userContextCallback;
    switch (result) {
    case IOTHUB_CLIENT_CONFIRMATION_OK:
        qDebug() << "ID: " << eventInstance->messageTrackingId << " confirmed";
        break;
    case IOTHUB_CLIENT_CONFIRMATION_BECAUSE_DESTROY:
        qDebug() << "ID: " << eventInstance->messageTrackingId << " not confirmed due to destroy";
        break;
    case IOTHUB_CLIENT_CONFIRMATION_MESSAGE_TIMEOUT:
        qDebug() << "ID: " << eventInstance->messageTrackingId << " not confirmed due to timeout";
        break;
    case IOTHUB_CLIENT_CONFIRMATION_ERROR:
        qDebug() << "ID: " << eventInstance->messageTrackingId << " not confirmed due to confirmation error";
        break;
    default:
        qDebug() << "ID: " << eventInstance->messageTrackingId << " Unknown confirmation";
        break;
    }
    IoTHubMessage_Destroy(eventInstance->messageHandle);
    delete eventInstance;
}

static IOTHUBMESSAGE_DISPOSITION_RESULT ReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    AzureWorker *worker = (AzureWorker *)userContextCallback;

    IOTHUBMESSAGE_CONTENT_TYPE contentType = IoTHubMessage_GetContentType(message);
    if (contentType == IOTHUBMESSAGE_BYTEARRAY)
    {
        const unsigned char *buffer = NULL;
        size_t size = 0;
        if (IoTHubMessage_GetByteArray(message, &buffer, &size) == IOTHUB_MESSAGE_OK)
            worker->messageReceived(QByteArray((const char *)buffer, size));
    }
    return IOTHUBMESSAGE_ACCEPTED;
}

AzureWorker::AzureWorker(AzureConnectionThread *thread)
    : m_connectionThread(thread)
{
}

bool AzureWorker::init(const QString &connectionString)
{
    if (platform_init() != 0)
        return false;

    m_iotClientHandle = IoTHubClient_LL_CreateFromConnectionString(qPrintable(connectionString), AMQP_Protocol);
    if (m_iotClientHandle == NULL)
        return false;

    IOTHUB_CLIENT_RESULT receiveEvent = IoTHubClient_LL_SetMessageCallback(m_iotClientHandle, ReceiveMessageCallback, this);
    if (receiveEvent != IOTHUB_CLIENT_OK)
        return false;

    return true;
}

bool AzureWorker::publishMessage(const QByteArray &inMessage)
{
    EVENT_INSTANCE* message = new EVENT_INSTANCE;
    message->messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)inMessage.constData(), inMessage.size());
    if (message->messageHandle == NULL)
        return false;
    message->messageTrackingId = m_messageId++;

    IOTHUB_CLIENT_RESULT sendEvent = IoTHubClient_LL_SendEventAsync(m_iotClientHandle, message->messageHandle, SendConfirmationCallback, message);
    if (sendEvent != IOTHUB_CLIENT_OK)
    {
        IoTHubMessage_Destroy(message->messageHandle);
        delete message;
        return false;
    }
    return true;
}

bool AzureWorker::update()
{
    IoTHubClient_LL_DoWork(m_iotClientHandle);
    return true;
}

bool AzureWorker::release()
{
    IoTHubClient_LL_Destroy(m_iotClientHandle);
    platform_deinit();
    return true;
}

void AzureWorker::messageReceived(const QByteArray &message)
{
    if (m_connectionThread)
        emit m_connectionThread->messageReceived(message);
}

AzureConnectionThread::AzureConnectionThread(const QString &connectionString)
    : m_connectionString(connectionString)
{
}

AzureConnectionThread::~AzureConnectionThread()
{
}

void AzureConnectionThread::stop()
{
    QMutexLocker lock(&m_mutex);
    m_running = false;
}

bool AzureConnectionThread::isRunning()
{
    QMutexLocker lock(&m_mutex);
    return m_running;
}

void AzureConnectionThread::publishMessage(const QByteArray &message)
{
    QMutexLocker lock(&m_mutex);
    if (m_messages.size() < 20)
        m_messages.enqueue(message);
}

void AzureConnectionThread::run()
{
    AzureWorker worker(this);
    if (!worker.init(m_connectionString))
        return;
    unsigned long sleepTime = 1;
    while (isRunning())
    {
        m_mutex.lock();
        if (m_messages.size() > 0)
        {
            if (worker.publishMessage(m_messages.dequeue()))
                emit messageSent();
            else
                emit messageError();
        }
        m_mutex.unlock();
        worker.update();
        QThread::msleep(sleepTime);
    }
    worker.release();
}

AzureConnection::AzureConnection()
    : m_thread(nullptr)
{
}

AzureConnection::~AzureConnection()
{
}

void AzureConnection::init(const QString &connectionString)
{
    if (m_thread) {
        qWarning() << "Already initialized.";
        return;
    }

    m_thread = new AzureConnectionThread(connectionString);
    QObject::connect(m_thread, &AzureConnectionThread::messageReceived,
                     this, &AzureConnection::messageReceived, Qt::QueuedConnection);
    QObject::connect(m_thread, &AzureConnectionThread::messageSent,
                     this, &AzureConnection::messageSent, Qt::QueuedConnection);
    QObject::connect(m_thread, &AzureConnectionThread::messageError,
                     this, &AzureConnection::messageError, Qt::QueuedConnection);
    m_thread->start();
}

void AzureConnection::cleanup()
{
    if (m_thread) {
        m_thread->stop();
        m_thread->wait();
        delete m_thread;
    }
    m_thread = nullptr;
}

void AzureConnection::publishMessage(const QByteArray &content)
{
    if (m_thread)
        m_thread->publishMessage(content);
}

#include "AzureConnection.moc"
