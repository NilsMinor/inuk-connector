#ifndef INUKMQTT_H
#define INUKMQTT_H

/* Author : Nils Minor
 *
 *
 * The inuk mqtt class is used to handle client communication to an mqtt server.
 * Therefore it implemnts the typical mqtt initialitation and communication functions.
 *
 * The strength of this module is the topic registration. It is used to call upper class functions
 * in a non-static way.
 *
 * To subscribe to an topic, the registerTopic function can be used in in conjunction with
 * a callback function, it is important to set the parent using >> new mqtt(this) <<.
 * The callback must by of type >> CallbackDataT(QObject*,QString&) << The callbacks a registered
 * in a map and handled by a mqtt callback whenever a message on any topic is received.
 *
 * Usage:
 *  InukConnector-Class:
 *
 *      friend void cb_function (QObject * Sender, QString msg);
 *
 *      void cb_function(QObject *Sender, QString msg)
 *      {
 *          DEBUG << "send message via uart " << msg;
 *          InukConnector * con = reinterpret_cast<InukConnector*>(Sender);
 *          con->serial->sendMessage(msg);
 *      }
 *
 * Registration:
 *      registerGatewayTopic ("/test", cb_function);
 *
 *
 */

#include <QObject>
#include "qmqtt.h"
#include <QTimer>
#include <QDebug>
#include <QLoggingCategory>

const QString HOST_NAME = QStringLiteral("test.mosquitto.org");
const quint16 PORT = 1883;

const QString MAIN_TOPIC                    = "inuit";
const QString GW_NAME                       = "/0001";
const QString GW_STATUS_TOPIC               = "/status";
const QString GW_SUB_TOPIC                  = "/gateway";
const QString NODE_SUB_TOPIC                = "/nodes";

// AT = ACTION_TOPIC
const QString AT_RESTART                    = "/restart";
const QString AT_UART_TX                    = "/uart-tx";
const QString AT_UART_RX                    = "/uart-rx";
const QString AT_MESH_CMD                   = "/mesh-cmd";

const QString ST_STATUS                     = "/status";
// MAIN TOPICS
const QString GW_TOPIC                      = MAIN_TOPIC + GW_NAME + GW_SUB_TOPIC ;
const QString NODES_TOPIC                   = MAIN_TOPIC + GW_NAME + NODE_SUB_TOPIC;

#define SEND_DEBUG_OVER_MQTT

// callback function declaration
typedef void CallbackDataT(QObject*,QString&);

class InukMQTT : public QObject
{
    Q_OBJECT
public:
    explicit InukMQTT(QObject *parent = nullptr);
    ~InukMQTT();

    void publish(QString topic, QString msg);
    void startConnecting(qint16 timeout = 10000);

    void registerGatewayTopic (QString topic, CallbackDataT *callback);
    void registerNodeTopic (QString topic, CallbackDataT *callback);

    void sendDebugMsg(QString msg);
    QObject * _parent;


private :
    void registerTopic (QString topic, CallbackDataT *callback);
    void handleRegisteredTopics (QString topic, QString payload);

    QMap<QString, CallbackDataT*> mqttCallbacks;
    QMQTT::Client *client;
    QTimer *reconnectTimer;

signals:
    void started ();
    void errorOccured (QString error);
    void connected (QString hostName);
    void disconnected (QString hostName);
    void receivedMessage (QString &msg);

public slots:

    void publishGateway (QString topic, QString& msg);
    void publishNode (QString topic, QString &msg);

private slots:
    void conectToHost();
    void onConnected();
    void onError(const QMQTT::ClientError error);
    void onSubscribed(const QString& topic);
    void onReceived(const QMQTT::Message& message);

};

#endif // INUKMQTT_H
