#ifndef LOGGINGTRANSPORT_H
#define LOGGINGTRANSPORT_H

#include <QtNetwork/QTcpSocket>

class QMutex;
class QTimer;

class LoggingTransport : public QTcpSocket
{
public:
    LoggingTransport(QObject *parent = nullptr);

protected:
    qint64 writeData(const char *data, qint64 len) override;
private:
    void printStatistics();
    QTimer *m_timer;
    QMutex *m_mutex;
    int m_dataSize{0};
};

#endif // LOGGINGTRANSPORT_H
