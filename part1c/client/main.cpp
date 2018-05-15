#include "sensorinformation.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SensorInformation s;
    s.start();

    return a.exec();
}
