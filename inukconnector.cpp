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

void cb_sendUartMessage(QObject *Sender, QString &topic, QString &msg)
{
    Q_UNUSED(topic);
    DEBUG << "send message via uart " << msg;
    InukConnector * con = reinterpret_cast<InukConnector*>(Sender);

    con->serial->sendMessage(msg);
}

void cb_sendUartCommand(QObject *Sender, QString &topic, QString &msg)
{
    DEBUG << "send message via uart " << msg;
    InukConnector * con = reinterpret_cast<InukConnector*>(Sender);

    QString cmd = con->cmd->createMessage({ACTION, con->getNodeId(topic), STATUS, msg});
    DEBUG << "send command " << cmd;
    con->serial->sendMessage(cmd);
}


//
// END OF FRIEND CALLBACKS
//

InukConnector::InukConnector(QObject *parent) : QObject(parent)
{
    // init serial connector
    serial = new InukSerial();
    connect(serial, &InukSerial::started, this, &InukConnector::serialStarted);                     // serial port client started, trying to connect
    connect(serial, &InukSerial::connected, this, &InukConnector::serialConnected);                 // serial port client connected
    connect(serial, &InukSerial::disconnected, this, &InukConnector::serialDisconnected);           // serial port client disconnected

    // init mqtt connector
    mqtt = new InukMQTT(this);
    connect(mqtt, &InukMQTT::started, this, &InukConnector::mqttStarted);                           // mqtt client started, trying to connect to host
    connect(mqtt, &InukMQTT::connected, this, &InukConnector::mqttConnected);                       // mqtt client connected to hsot
    connect(mqtt, &InukMQTT::disconnected, this, &InukConnector::mqttDisconnected);                 // mqtt client disconnected from host
    connect(mqtt, &InukMQTT::errorOccured, this, &InukConnector::mqttError);                        // mqtt client error occured

    // command manager
    cmd = new InukCommandHandler();
    connect(serial, &InukSerial::connected, cmd, &InukCommandHandler::connected);                   // inform cmd handler that serial conection is established
    connect(serial, &InukSerial::receivedData, cmd, &InukCommandHandler::handleRawMessage);         // inform cmd handler that serial message was received
    connect(cmd, &InukCommandHandler::messsageHandledString, this, &InukConnector::printMessage);   // cmd handler has parsed a string message
    connect(cmd, &InukCommandHandler::messsageHandledJson, this, &InukConnector::printJSON);        // cmd handler has parsed a json message
    connect(cmd, &InukCommandHandler::sendMessageToSerial, serial, &InukSerial::sendMessage);       // cmd handler send message over serial port

    connect(cmd, &InukCommandHandler::nodeConnected, this, &InukConnector::connectNode);            // cmd handler recognized a node connection
    connect(cmd, &InukCommandHandler::nodeDisconnected, this, &InukConnector::disconnectNode);      // cmd handler recoginzed a node disconnection


    mqtt->startConnecting();

    DEBUG << "initialization done";
}

InukConnector::~InukConnector()
{

}

QString InukConnector::getNodeId(QString topic)
{
    // topic is build like >inuit/nodes/2935<
    QStringList pieces = topic.split( "/" );
    return pieces.value( pieces.length() - 1 );
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

     // mqtt->registerGatewayTopic(AT_RESTART, cb_restartApplication);
     // mqtt->registerGatewayTopic(AT_UART_TX, cb_sendUartMessage);
}
void InukConnector::mqttError (QString error) {
     DEBUG << "mqtt error " << error;
}
void InukConnector::mqttDisconnected(QString hostName) {
    DEBUG << "mqtt disconnected from " << hostName;
}

void InukConnector::connectNode(QString nodeId, bool isGateWay)
{
    // register gateway nodeId topic
    if (isGateWay) {
        DEBUG << "register node as gateway " << nodeId;
        mqtt->registerGatewayTopic(nodeId, cb_sendUartCommand);
    }
    else {
        connectedNodes += nodeId;
        mqtt->registerNodeTopic(nodeId, cb_sendUartCommand);
        DEBUG << "register node " << nodeId << " connectedNodes is" << connectedNodes;
    }
}

void InukConnector::disconnectNode(QString nodeId, bool isGateWay)
{
    if (isGateWay) {
        mqtt->unregisterNodeTopic(nodeId);
    }
    else {
        connectedNodes.removeOne(nodeId);
        mqtt->unregisterNodeTopic(nodeId);
        DEBUG << "unregsiter node " << nodeId << " connectedNodes is " << connectedNodes;
    }
}

//
// CMD HANDLER
//
void InukConnector::printMessage(QString &msg)
{
    mqtt->publishGateway(AT_UART_RX, msg);
    DEBUG << "Message is : " << msg;
}
void InukConnector::printJSON(QJsonObject &json)
{
    QJsonDocument doc(json);
    QString msg(doc.toJson(QJsonDocument::Compact));

    // check if the message contains a nodeId
    QString nodeId = QString::number(GET_NODE_ID(json));

    if (!nodeId.isEmpty()) {
        if (connectedNodes.contains(nodeId)) {
            mqtt->publishNode( '/' + nodeId + AT_UART_RX, msg);
        }
    }
    else {
        mqtt->publishGateway(AT_UART_RX, msg);
    }

    // DEBUG << "JSON is : " << json;
}








