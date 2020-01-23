#ifndef STATUSMESSAGE_H
#define STATUSMESSAGE_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include "messagetype.h"

class StatusMessage : MessageType
{
public:
    explicit StatusMessage( );
    void read(const QJsonObject &jsonObj);
    void print(void);
    bool test(const QJsonObject &jsonObj);

private:
    QJsonObject jsonObj;
    int blversion;
    int code1;
    int reason;
    int stack;
    QString type;
    int version;

    QStringList keys;

signals:

public slots:
};

#endif // STATUSMESSAGE_H
