#include "inukconnector.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.con")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)

InukConnector::InukConnector(QObject *parent) : QObject(parent)
{
    // init serial connector
    serial = new InukSerial();

    connect(serial, &InukSerial::started, this, &InukConnector::serialStarted);
    connect(serial, &InukSerial::connected, this, &InukConnector::serialConnected);
    connect(serial, &InukSerial::disconnected, this, &InukConnector::serialDisconnected);

    // init mqtt connector
    mqtt = new InukMQTT();
    connect(mqtt, &InukMQTT::started, this, &InukConnector::mqttStarted);
    connect(mqtt, &InukMQTT::connected, this, &InukConnector::mqttConnected);
    connect(mqtt, &InukMQTT::disconnected, this, &InukConnector::mqttDisconnected);
    connect(mqtt, &InukMQTT::errorOccured, this, &InukConnector::mqttError);

    cmd = new InukCommandHandler();

    connect(serial, &InukSerial::receivedData, cmd, &InukCommandHandler::handleRawMessage);
    connect(cmd, &InukCommandHandler::messsageHandledString, this, &InukConnector::printMessage);
    connect(cmd, &InukCommandHandler::messsageHandledJson, this, &InukConnector::printJSON);

    mqtt->startConnecting();
    serial->startScanning();

    DEBUG << "initialization done";
}

InukConnector::~InukConnector()
{

}

//
// SERIAL
//
void InukConnector::serialStarted( ) {
    DEBUG << "serial started scanning";
    mqtt->publish("status", "serial-scanning");
}
void InukConnector::serialConnected(QString portName) {
    DEBUG << "serial connected to " << portName;
    mqtt->publish("status", "serial-connected");
}
void InukConnector::serialDisconnected(QString portName) {
    DEBUG << "serial disconnected from " << portName;
    mqtt->publish("status", "serial-disconnected");
}

//
// MQTT
//

void InukConnector::mqttStarted () {
    DEBUG << "mqtt started";
}
void InukConnector::mqttConnected (QString hostName) {
     DEBUG << "mqtt connected to " << hostName;
}
void InukConnector::mqttError (QString error) {
     DEBUG << "mqtt error " << error;
}
void InukConnector::mqttDisconnected(QString hostName) {
    DEBUG << "mqtt disconnected from " << hostName;
}

void InukConnector::printMessage(QString &msg)
{
    DEBUG << "Message is : " << msg;
}
void InukConnector::printJSON(QJsonObject &json)
{
    DEBUG << "JSON is : " << json;
}

void InukConnector::restart()
{
    DEBUG << "restart application";
    // qApp->exit(EXIT_CODE_REBOOT);
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}


