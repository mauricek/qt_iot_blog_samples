QT -= gui
QT += mqtt network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    brokerconnection.cpp \
    AzureConnection.cpp \
    sensorinformation.cpp

HEADERS += \
    brokerconnection.h \
    AzureConnection.h \
    sensorinformation.h

win32 {
    # Those reflect the nuget packages received via VS2017. Install them via
    # nuget install <item>. NuGet will put them below your current directory.
    # Note, that the packages from NuGet seem to be static builds, if that
    # has any legal implication to the project using this part
    AZURE_PATH = C:/dev/azure/Microsoft.Azure.IoTHub.IoTHubClient.1.2.11
    AZURE_UTIL_PATH = C:/dev/azure/Microsoft.Azure.C.SharedUtility.1.1.11
    AZURE_AMQP_PATH = C:/dev/azure/Microsoft.Azure.IoTHub.AmqpTransport.1.2.11
    AZURE_HTTP_PATH = C:/dev/azure/Microsoft.Azure.IoTHub.HttpTransport.1.2.11
    AZURE_UAMQP_PATH = C:/dev/azure/Microsoft.Azure.uamqp.1.2.11

    !exists($$AZURE_PATH): error("Need to specify AZURE_PATH to find Azure libraries")

    CONFIG(debug, debug|release) {
        AZURE_BUILD = Debug
    } else {
        AZURE_BUILD = Release
    }

#    deployFiles.files += $${AZURE_PATH}/iothub_client/$${AZURE_BUILD}/iothub_client_dll.dll
#    INSTALLS += deployFiles

    LIBS += $${AZURE_PATH}/build/native/x64/$${AZURE_BUILD}/iothub_client.lib
    LIBS += $${AZURE_PATH}/build/native/x64/$${AZURE_BUILD}/parson.lib
    LIBS += $${AZURE_PATH}/build/native/x64/$${AZURE_BUILD}/prov_auth_client.lib
    LIBS += $${AZURE_PATH}/build/native/x64/$${AZURE_BUILD}/hsm_security_client.lib
    LIBS += $${AZURE_PATH}/build/native/x64/$${AZURE_BUILD}/uhttp.lib
    LIBS += $${AZURE_UTIL_PATH}/build/native/x64/$${AZURE_BUILD}/aziotsharedutil.lib
    LIBS += $${AZURE_AMQP_PATH}/build/native/x64/$${AZURE_BUILD}/iothub_client_amqp_transport.lib
    LIBS += $${AZURE_HTTP_PATH}/build/native/x64/$${AZURE_BUILD}/iothub_client_http_transport.lib
    LIBS += $${AZURE_UAMQP_PATH}/build/native/x64/$${AZURE_BUILD}/uamqp.lib
    LIBS += ws2_32.lib Secur32.lib Advapi32.lib Ncrypt.lib Crypt32.lib Rpcrt4.lib Winhttp.lib

    INCLUDEPATH += $${AZURE_PATH}/build/native/include
    INCLUDEPATH += $${AZURE_UTIL_PATH}/build/native/include/azure_c_shared_utility
    INCLUDEPATH += $${AZURE_UTIL_PATH}/build/native/include
    INCLUDEPATH += $${AZURE_AMQP_PATH}/build/native/include
}

linux {
    # This project assumes the C-SDK to be preinstalled on a system
    # For Ubuntu do:
    # sudo add-apt-repository ppa:aziotsdklinux/ppa-azureiot
    # sudo apt-get update
    # sudo apt-get install azure-io-sdk-c-dev

    # Building instructions for compiling manually
    # git clone --recursive https://github.com/Azure/azure-iot-sdk-c.git
    # create build directory, in there
    # cmake -Dskip_samples=ON -DCMAKE_INSTALL_PREFIX=<installdir> ..

    AZURE_INSTALL_PATH= #C:/Utils/arm/azure_arm_build
    isEmpty(AZURE_INSTALL_PATH) {
        message("Assuming Azure to be preinstalled")
        AZURE_INSTALL_PATH=/usr
    } else {
        message("Using Azure from: $${AZURE_INSTALL_PATH}")
    }

    LIBS+= -L$${AZURE_INSTALL_PATH}/lib
    INCLUDEPATH += $${AZURE_INSTALL_PATH}/include

    INCLUDEPATH += $${AZURE_INSTALL_PATH}/include/azureiot
    INCLUDEPATH += $${AZURE_INSTALL_PATH}/include/azureiot/inc

    LIBS += -liothub_client -liothub_client_amqp_transport -laziotsharedutil -luamqp
    # Dependencies
    LIBS += -lcurl -lssl -lcrypto -luuid
}

