QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QT += network testlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


PROTO_FILE = sensor.proto
protoc.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.pb.cc
protoc.variable_out = GENERATED_SOURCES
protoc.input = PROTO_FILE

win32 {
    PROTO_PATH = C:/Utils/protobuf_rel_sh
    DEFINES += PROTOBUF_USE_DLLS

    protoc.commands = $${PROTO_PATH}/bin/protoc -I=$$relative_path($${PWD}, $${OUT_PWD}) --cpp_out=. ${QMAKE_FILE_NAME}
    INCLUDEPATH += $${PROTO_PATH}/include
    DEPENDPATH += $${PROTO_PATH}/include
    LIBS += -L$${PROTO_PATH}/lib -llibprotobuf
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += protobuf

    protoc.commands = protoc -I=$$relative_path($${PWD}, $${OUT_PWD}) --cpp_out=. ${QMAKE_FILE_NAME}
}
QMAKE_EXTRA_COMPILERS += protoc

SOURCES += \
        main.cpp \
    sensorinformation.cpp \
    sensorinformationproto.cpp \
    loggingtransport.cpp

HEADERS += \
    sensorinformation.h \
    sensorinformationproto.h \
    loggingtransport.h
