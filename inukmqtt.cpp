#include "inukmqtt.h"

InukMQTT::InukMQTT(QObject *parent) : QObject(parent)
{
    QHostAddress host;
    host.setAddress(QStringLiteral("test.mosquitto.org"));

    client = new QMQTT::Client;
    //client->setClientId("clientId");
    //client->setUsername("name");
    //client->setPassword("password");
    client->setHostName(EXAMPLE_HOST);
    client->setPort(1883);
    client->connectToHost();
    //connect(client, SIGNAL(subscribed(QString,quint8)), this, SLOT(slotSubcribedSuccess(QString))); connect(client, SIGNAL(received(QMQTT::Message)), this, SLOT(slotShowMessage(QMQTT::Message)));
//    connect(sendButton, SIGNAL(clicked()), this, SLOT(slotSendButton()));
    connect(client, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(client, SIGNAL(subscribed(QString)), this, SLOT(onSubscribed(QString)));
    connect(client, SIGNAL(received(QMQTT::Message)), this, SLOT(onReceived(QMQTT::Message)));
    connect(client, SIGNAL(error(QMQTT::ClientError)), this, SLOT(onError(QMQTT::ClientError)));


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &InukMQTT::sendMessage);
    //this->timer->start(2000);
}

InukMQTT::~InukMQTT()
{
    if(client->isConnectedToHost()) {
        client->disconnectFromHost();
    }
}

void InukMQTT::onConnected()
{
    qDebug() << "MQTT is now connected to host " << client->hostName();
    client->subscribe("/inuk/rec");
}

void InukMQTT::onError(const QMQTT::ClientError error)
{
    qDebug() << "MQTT error " << error;

}

void InukMQTT::onSubscribed(const QString &topic)
{
    qDebug() << "MQTT subscribed to topic " << topic << endl;
}

void InukMQTT::onReceived(const QMQTT::Message &message)
{
    qDebug() << "MQTT publish received: \"" << QString::fromUtf8(message.payload()) << "\"" << endl;
}



void InukMQTT::sendMessage()
{
    if (client->isConnectedToHost()) {
        QMQTT::Message message(number, EXAMPLE_TOPIC, QString("Number is %1").arg(number).toUtf8());
        client->publish(message);
        number++;
    } else {
        client->connectToHost();
        qDebug() << "MQTT not connected to host";
    }
}



void InukMQTT::sendString(QString obj)
{
    QMQTT::Message message(1, EXAMPLE_TOPIC,
                                   QString(obj).toUtf8());
    client->publish(message);
}
