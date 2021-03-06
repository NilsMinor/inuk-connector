QT -= gui
QT += core serialport widgets qmqtt network

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += sdk_no_version_check

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        connectionsetting.cpp \
        inukcommandhandler.cpp \
        inukconnector.cpp \
        inukmqtt.cpp \
        inukserial.cpp \
        main.cpp \
        messages/connectionmessage.cpp \
        messages/logmessage.cpp \
        messages/meshconnectmessage.cpp \
        messages/message.cpp \
        qtjsonhandler.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    connectionsetting.h \
    inukcommandhandler.h \
    inukconnector.h \
    inukmqtt.h \
    inukserial.h \
    messages/connectionmessage.h \
    messages/fruitymessages.h \
    messages/logmessage.h \
    messages/meshconnectmessage.h \
    messages/message.h \
    qtjsonhandler.h

DISTFILES += \
    README.md \
    config/connection.json
