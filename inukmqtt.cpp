#include "inukmqtt.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.mqtt")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)

InukMQTT::InukMQTT(QObject *parent) : _parent(parent)
{
    client = new QMQTT::Client;
    client->setHostName(HOST_NAME);
    client->setPort(1883);

    connect(client, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(client, SIGNAL(subscribed(QString)), this, SLOT(onSubscribed(QString)));
    connect(client, SIGNAL(received(QMQTT::Message)), this, SLOT(onReceived(QMQTT::Message)));
    connect(client, SIGNAL(error(QMQTT::ClientError)), this, SLOT(onError(QMQTT::ClientError)));

    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, &QTimer::timeout, this, &InukMQTT::conectToHost);
}

InukMQTT::~InukMQTT()
{
    if(client->isConnectedToHost()) {
        client->disconnectFromHost();
    }
}

void InukMQTT::startConnecting(qint16 timeout)
{
    emit started();
    DEBUG << "start timer with timeout : " << timeout;
    this->reconnectTimer->start(timeout);
}

void InukMQTT::registerGatewayTopic(QString topic, CallbackDataT *callback)
{
    QString m ="subscribed";
    QString top = "/" + topic;
    this->publishGateway(top, m);
    this->registerTopic(GW_TOPIC + top, callback);
}

void InukMQTT::registerNodeTopic(QString topic, CallbackDataT *callback, QString initMsg)
{
    if (!initMsg.isEmpty()) {
        this->publishNode(topic, initMsg);
    }

    this->registerTopic(NODES_TOPIC + "/"+ topic, callback);
}

void InukMQTT::unregisterNodeTopic(QString topic, QString msg)
{

    if (!msg.isEmpty()) {
        this->publishNode(topic, msg);
    }
    client->unsubscribe(topic);
}

void InukMQTT::sendDebugMsg(QString msg)
{
#if defined (SEND_DEBUG_OVER_MQTT)
    if (client->isConnectedToHost()) {
        this->publish(GW_TOPIC + "/debug", msg);
    }
#endif
}

void InukMQTT::registerTopic(QString topic,  CallbackDataT *callback)
{
    if (!callback) {
        WARN << "no callback function set for topic " << topic;
        return;
    }
    else {
        client->subscribe(topic);
        mqttCallbacks.insert(topic, callback);
    }
}

void InukMQTT::handleRegisteredTopics(QString topic, QString payload)
{
    CallbackDataT * _callback = mqttCallbacks.value(topic);
    if (_parent == nullptr) {
        WARN << "no parent was set";
        return;
    }
    if (_callback != nullptr) {
        DEBUG << "call registers callback for [" << topic << "] with payload " << payload;
        _callback (_parent, topic, payload);
    } else {
        WARN << "no callback for  " << topic << " in map: " << mqttCallbacks;
    }
}

void InukMQTT::conectToHost()
{
    if (!client->isConnectedToHost()) {
        DEBUG << "try to connect to " << client->hostName();
        client->connectToHost();
    }
}

void InukMQTT::onConnected()
{
    reconnectTimer->stop();
    DEBUG << "MQTT is now connected to host " << client->hostName();
    emit connected(client->hostName());
}

void InukMQTT::onError(const QMQTT::ClientError error)
{
    QString err = "MQTT error " + QString(error);
    DEBUG << err;
    emit errorOccured(err);
}

void InukMQTT::onSubscribed(const QString &topic)
{
    DEBUG << "subscribed to topic " << topic;
}

void InukMQTT::onReceived(const QMQTT::Message &message)
{
    DEBUG << "received: [" << message.topic() << "]" << QString::fromUtf8(message.payload()) ;
    handleRegisteredTopics(message.topic(), QString::fromUtf8(message.payload()));
}

void InukMQTT::publishGateway(QString topic, QString &msg)
{
    QMQTT::Message message(0, GW_TOPIC + topic, QString(msg).toUtf8());
    client->publish(message);
}

void InukMQTT::publishNode(QString topic, QString msg)
{
    QMQTT::Message message(0, NODES_TOPIC + "/" + topic, QString(msg).toUtf8());
    client->publish(message);
}

void InukMQTT::publish(QString topic, QString msg)
{
    QString _topic = GW_TOPIC + topic;
    DEBUG << "send [" << _topic << "]: " << msg;
    QMQTT::Message message(1, _topic, QString(msg).toUtf8());
    client->publish(message);
}
