#ifndef INUKMQTT_H
#define INUKMQTT_H

/* Author : Nils Minor
 *
 *
 * The inuk mqtt class is used to handle client communication to an mqtt server.
 * Therefore it implemnts the typical mqtt initialitation and communication functions.
 *
 * To subscribe to an topic, the registerTopic function can be used in in conjunction with
 * a callbakc function. The callback must by of type void (*cbFunction)(QString);
 * The callback will be triggered when a message on the specific topic is received.
 *
 * Example:
 *      obj::cbTest(QString msg);
 *      obj->registerTopic("test", cbTest);
 *
 */

#include <QObject>
#include "qmqtt.h"
#include <QTimer>
#include <QDebug>
#include <QLoggingCategory>

const QString HOST_NAME = QStringLiteral("test.mosquitto.org");
const quint16 PORT = 1883;

const QString MAIN_TOPIC            = "inuit";
const QString GW_NAME               = "/0001";
const QString GW_STATUS_TOPIC       = "/status";
const QString GW_SUB_TOPIC          = "/gateway";
const QString NODE_SUB_TOPIC        = "/nodes";

const QString GW_TOPIC              = MAIN_TOPIC + GW_NAME + GW_SUB_TOPIC ;
const QString NODES_TOPIC           = MAIN_TOPIC + GW_NAME + NODE_SUB_TOPIC ;

// callback function declaration
typedef void (*cbFunction)(QString);

class InukMQTT : public QObject
{
    Q_OBJECT
public:
    explicit InukMQTT(QObject *parent = nullptr);
    ~InukMQTT();

    void publish(QString topic, QString msg);
    void startConnecting(qint16 timeout = 10000);

    void registerGatewayTopic (QString topic,void (*func)(QString msg));
    void registerNodeTopic (QString topic,void (*func)(QString msg));



private :
    void registerTopic (QString topic,void (*func)(QString msg));
    void handleRegisteredTopics (QString topic, QString payload);

    QMap<QString, cbFunction> mqttCallbacks;
    QMQTT::Client *client;
    QTimer *reconnectTimer;

signals:
    void started ();
    void errorOccured (QString error);
    void connected (QString hostName);
    void disconnected (QString hostName);
    void receivedMessage (QString &msg);

public slots:
    void sendString(QString obj);

private slots:
    void conectToHost();
    void onConnected();
    void onError(const QMQTT::ClientError error);
    void onSubscribed(const QString& topic);
    void onReceived(const QMQTT::Message& message);

};

#endif // INUKMQTT_H
