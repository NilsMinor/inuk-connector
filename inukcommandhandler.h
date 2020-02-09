#ifndef INUKCOMMANDHANDLER_H
#define INUKCOMMANDHANDLER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include "inukmqtt.h"

#include "messages/fruitymessages.h"


#define GATEWAY_ID      "this"
#define ALL_ID          "0"
#define STATUS          "status"
#define ACTION          "action"
#define DEVICE_INFO     "get_device_info"

class InukCommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit InukCommandHandler(QObject *parent = nullptr);
    QString createMessage(QStringList list);

private :
    QTimer * periodicTimer;
    void parseRawMessage (QString &msg);
    void parseRawMessage (QJsonObject & obj);


signals:
    void sendMessageToSerial(QString msg);

    void messsageHandledString(QString &msg);
    void messsageHandledJson(QJsonObject &json);
    void registerGateWayToipc (QString topic, QString msg);
    void nodeConnected (QString nodeId, bool isGateWay=false);
    void nodeDisconnected (QString nodeId, bool isGateWay=false);


public slots:
    void connected ();
    void handleRawMessage (QString &msg);
    void periodicCallback();
};

#endif // INUKCOMMANDHANDLER_H
