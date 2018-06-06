QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

# Specify path to RTI installation
RTIDDS_PREFIX = C:\\rti_dds\\rti_connext_dds-5.3.1
RTIDDS_VERSION = i86Win32VS2017

RTIDDS_IDL = ../common/sensor.idl

include(../common/dds.pri)

SOURCES += \
        main.cpp
