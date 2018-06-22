#include "brokerconnection.h"
#include "AzureConnection.h"

#include <QtCore/QCoreApplication>

const QString connectionString = QLatin1String("");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AzureConnection azure;
    azure.init(connectionString);

    BrokerConnection brokerConnection;
    brokerConnection.setAzure(&azure);
    brokerConnection.init();

    return a.exec();
}
