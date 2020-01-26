#include "inukmqtt.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.mqtt")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)

InukMQTT::InukMQTT(QObject *parent) : QObject(parent)
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

    startConnecting ();

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

    client->subscribe( '/' + MAIN_TOPIC + '/' + GW_NAME + '/' + "action");
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
}



void InukMQTT::sendMessage()
{
//    if (client->isConnectedToHost()) {
//        QMQTT::Message message(number, EXAMPLE_TOPIC, QString("Number is %1").arg(number).toUtf8());
//        client->publish(message);
//        number++;
//    } else {
//        client->connectToHost();
//        DEBUG << "MQTT not connected to host";
//    }
}

void InukMQTT::sendString(QString obj)
{
    // QMQTT::Message message(1, EXAMPLE_TOPIC, QString(obj).toUtf8());
    // client->publish(message);
}

void InukMQTT::publish(QString topic, QString msg)
{
    QString _topic = '/' + MAIN_TOPIC + '/' + GW_NAME + '/' + topic;
    DEBUG << "send [" << _topic << "]: " << msg;
    QMQTT::Message message(1, _topic, QString(msg).toUtf8());
    client->publish(message);
}
