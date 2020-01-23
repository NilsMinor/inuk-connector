#ifndef INUKCOMMANDHANDLER_H
#define INUKCOMMANDHANDLER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include "inukmqtt.h"

class InukCommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit InukCommandHandler(QObject *parent = nullptr);

private :
    QString parseRawMessage (QString msg);
    QTimer * periodicTimer;
    InukMQTT * mqtt;

signals:
    void sendMessage(const QByteArray &data);

public slots:
    void handleRawMessage (QString msg);
    void periodicCallback();
};

#endif // INUKCOMMANDHANDLER_H
