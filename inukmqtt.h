#ifndef INUKMQTT_H
#define INUKMQTT_H

#include <QObject>
#include "qmqtt.h"
#include <QTimer>
#include <QDebug>

const QString EXAMPLE_HOST = QStringLiteral("test.mosquitto.org");
const quint16 EXAMPLE_PORT = 1883;
const QString EXAMPLE_TOPIC = "/inuk/testtopic";

class InukMQTT : public QObject
{

    Q_OBJECT
public:
    explicit InukMQTT(QObject *parent = nullptr);
    ~InukMQTT();

    QMQTT::Client *client;
    QTimer *timer;
    void init (void);
    quint16 number = 0;

signals:

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
