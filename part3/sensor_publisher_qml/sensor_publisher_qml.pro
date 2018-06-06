QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

# Specify path to RTI installation
RTIDDS_PREFIX = C:\\rti_dds\\rti_connext_dds-5.3.1
RTIDDS_VERSION = i86Win32VS2017

RTIDDS_IDL = ../common/sensor.idl

include(../common/dds.pri)

SOURCES += \
        main.cpp \
        sensorinformation.cpp

HEADERS += \
        sensorinformation.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
