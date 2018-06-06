# rtiddsgen -language c++ -d C:\dev\qt_iot_communication_blog\part3\foo ..\sensor_publisher\sensor.idl

ddsgen.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.cxx # Additionally created files get their own rule
ddsgen.variable_out = GENERATED_SOURCES
ddsgen.input = RTIDDS_IDL
ddsgen.commands = $${RTIDDS_PREFIX}\\bin\\rtiddsgen -language c++ -d $${OUT_PWD} ${QMAKE_FILE_NAME}

ddsheadergen.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.h
ddsheadergen.variable_out = GENERATED_FILES
ddsheadergen.input = RTIDDS_IDL
ddsheadergen.depends = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.cxx
ddsheadergen.commands = echo "Additional Header: ${QMAKE_FILE_NAME}"

ddsplugingen.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}Plugin.cxx
ddsplugingen.variable_out = GENERATED_SOURCES
ddsplugingen.input = RTIDDS_IDL
ddsplugingen.depends = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.cxx # Depend on the output of rtiddsgen
ddsplugingen.commands = echo "Additional Source(Plugin): ${QMAKE_FILE_NAME}"

ddspluginheadergen.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}Plugin.h
ddspluginheadergen.variable_out = GENERATED_FILES
ddspluginheadergen.input = RTIDDS_IDL
ddspluginheadergen.depends = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.cxx  # Depend on the output of rtiddsgen
ddspluginheadergen.commands = echo "Additional Header(Plugin): ${QMAKE_FILE_NAME}"

ddssupportgen.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}Support.cxx
ddssupportgen.variable_out = GENERATED_SOURCES
ddssupportgen.input = RTIDDS_IDL
ddssupportgen.depends = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.cxx # Depend on the output of rtiddsgen
ddssupportgen.commands = echo "Additional Source(Support): ${QMAKE_FILE_NAME}"

ddssupportheadergen.output = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}Support.h
ddssupportheadergen.variable_out = GENERATED_FILES
ddssupportheadergen.input = RTIDDS_IDL
ddssupportheadergen.depends = $${OUT_PWD}/${QMAKE_FILE_IN_BASE}.cxx # Depend on the output of rtiddsgen
ddssupportheadergen.commands = echo "Additional Header(Support): ${QMAKE_FILE_NAME}"

QMAKE_EXTRA_COMPILERS += ddsgen ddsheadergen ddsplugingen ddspluginheadergen ddssupportgen ddssupportheadergen

RTIDDS_INCLUDES = $${RTIDDS_PREFIX}/include \
                  $${RTIDDS_PREFIX}/include/ndds

INCLUDEPATH += $${RTIDDS_INCLUDES}
DEPENDPATH += $${RTIDDS_INCLUDES}
LIBS += -L$${RTIDDS_PREFIX}/lib/$${RTIDDS_VERSION} nddscd.lib nddscored.lib
