#include "loggingtransport.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QTimer>

LoggingTransport::LoggingTransport(QObject *parent)
    : QTcpSocket(parent)
{
    qDebug() << "Constructing Logging Client.";
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &LoggingTransport::printStatistics);
    m_timer->start();
    m_mutex = new QMutex();
}

qint64 LoggingTransport::writeData(const char *data, qint64 len)
{
    QMutexLocker l(m_mutex);
    m_dataSize += len;
    return QTcpSocket::writeData(data, len);
}

void LoggingTransport::printStatistics()
{
    QMutexLocker l(m_mutex);

    qDebug() << QDateTime::currentDateTime() << " Data sent:" << m_dataSize;
}

