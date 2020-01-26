#ifndef INUKMQTT_H
#define INUKMQTT_H

#include <QObject>
#include "qmqtt.h"
#include <QTimer>
#include <QDebug>
#include <QLoggingCategory>

const QString HOST_NAME = QStringLiteral("test.mosquitto.org");
const quint16 PORT = 1883;

const QString MAIN_TOPIC            = "inuit";
const QString GW_NAME               = "0001";
const QString GW_STATUS_TOPIC       = "status";
const QString NODE_TOPIC            = "nodes";

class InukMQTT : public QObject
{
    Q_OBJECT
public:
    explicit InukMQTT(QObject *parent = nullptr);
    ~InukMQTT();

    void publish(QString topic, QString msg);
    void startConnecting(qint16 timeout = 1000);

private :
    QMQTT::Client *client;
    QTimer *reconnectTimer;
    void conectToHost ();
signals:
    void started ();
    void errorOccured (QString error);
    void connected (QString hostName);
    void disconnected (QString hostName);
    void receivedMessage (QString &msg);

public slots:

    void sendMessage();
    void sendString(QString obj);

private slots:
    void onConnected();
    void onError(const QMQTT::ClientError error);
    void onSubscribed(const QString& topic);
    void onReceived(const QMQTT::Message& message);

};

#endif // INUKMQTT_H
