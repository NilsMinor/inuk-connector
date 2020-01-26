#ifndef INUKCOMMANDHANDLER_H
#define INUKCOMMANDHANDLER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include "inukmqtt.h"


#define GATEWAY_ID      "this"
#define ALL_ID          "0"

class InukCommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit InukCommandHandler(QObject *parent = nullptr);

private :
    QString parseRawMessage (QString msg);
    QTimer * periodicTimer;

signals:
    void sendMessage(const QByteArray &data);
    void messsageHandledString(QString &msg);
    void messsageHandledJson(QJsonObject &json);

public slots:
    void handleRawMessage (QString &msg);
    void periodicCallback();
};

#endif // INUKCOMMANDHANDLER_H
