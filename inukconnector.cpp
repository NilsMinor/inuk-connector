#include "inukconnector.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.con")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)

//
// DEFINE FRIEND CALLBACKS
//

void cb_restartApplication(QObject *Sender, QString &msg)
{
    Q_UNUSED(Sender);
    DEBUG << "restart application with message " << msg;
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void cb_sendUartMessage(QObject *Sender, QString &msg)
{
    DEBUG << "send message via uart " << msg;
    InukConnector * con = reinterpret_cast<InukConnector*>(Sender);
    con->serial->sendMessage(msg);
}

//
// END OF FRIEND CALLBACKS
//

InukConnector::InukConnector(QObject *parent) : QObject(parent)
{
    // init serial connector
    serial = new InukSerial();

    connect(serial, &InukSerial::started, this, &InukConnector::serialStarted);
    connect(serial, &InukSerial::connected, this, &InukConnector::serialConnected);
    connect(serial, &InukSerial::disconnected, this, &InukConnector::serialDisconnected);


    // init mqtt connector
    mqtt = new InukMQTT(this);
    connect(mqtt, &InukMQTT::started, this, &InukConnector::mqttStarted);
    connect(mqtt, &InukMQTT::connected, this, &InukConnector::mqttConnected);
    connect(mqtt, &InukMQTT::disconnected, this, &InukConnector::mqttDisconnected);
    connect(mqtt, &InukMQTT::errorOccured, this, &InukConnector::mqttError);


    cmd = new InukCommandHandler();

    connect(serial, &InukSerial::receivedData, cmd, &InukCommandHandler::handleRawMessage);
    connect(cmd, &InukCommandHandler::messsageHandledString, this, &InukConnector::printMessage);
    connect(cmd, &InukCommandHandler::messsageHandledJson, this, &InukConnector::printJSON);

    connect(this,   &InukConnector::sendViaSerial, serial, &InukSerial::sendMessage);

    mqtt->startConnecting();

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
    mqtt->publish(ST_STATUS, "serial-scanning");
}
void InukConnector::serialConnected(QString portName) {
    DEBUG << "serial connected to " << portName;
    mqtt->publish(ST_STATUS, "serial-connected");

}
void InukConnector::serialDisconnected(QString portName) {
    DEBUG << "serial disconnected from " << portName;
    mqtt->publish(ST_STATUS, "serial-disconnected");
}

//
// MQTT
//

void InukConnector::mqttStarted () {
    DEBUG << "mqtt started";
}
void InukConnector::mqttConnected (QString hostName) {
     DEBUG << "mqtt connected to " << hostName;
     serial->startScanning();

     mqtt->registerGatewayTopic(AT_RESTART, cb_restartApplication);
     //mqtt->registerGatewayTopic(AT_UART_TX, cb_sendUartMessage);

     mqtt->registerGatewayTopic(AT_UART_TX, cb_sendUartMessage);

//     connect(cmd, SIGNAL(messsageHandledString(QString&)), mqtt, SLOT(publishGateway("/uart-rx",QString&)));

}
void InukConnector::mqttError (QString error) {
     DEBUG << "mqtt error " << error;
}
void InukConnector::mqttDisconnected(QString hostName) {
    DEBUG << "mqtt disconnected from " << hostName;
}

void InukConnector::printMessage(QString &msg)
{
    mqtt->publishGateway(AT_UART_RX, msg);
    DEBUG << "Message is : " << msg;
}
void InukConnector::printJSON(QJsonObject &json)
{
    DEBUG << "JSON is : " << json;
}








