#ifndef AZURECONNECTION_H
#define AZURECONNECTION_H

#include <QtCore/QObject>
#include <QtCore/QString>

class AzureConnectionThread;

class AzureConnection : public QObject
{
    Q_OBJECT
public:
    AzureConnection();
    ~AzureConnection();

    void init(const QString &connectionString);
    void cleanup();

Q_SIGNALS:
    void messageReceived(const QByteArray &content);
    void messageSent();
    void messageError();

public Q_SLOTS:
    void publishMessage(const QByteArray &content);

private:
    AzureConnectionThread *m_thread;
};

#endif // AZURECONNECTION_H
