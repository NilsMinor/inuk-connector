#include "inukcommandhandler.h"
#include <QObject>
#include "qtjsonhandler.h"

InukCommandHandler::InukCommandHandler(QObject *parent) : QObject(parent)
{
    periodicTimer = new QTimer(this);
    connect(periodicTimer, &QTimer::timeout, this, &InukCommandHandler::periodicCallback);
    periodicTimer->start(1000);

     mqtt = new InukMQTT();
}

QString InukCommandHandler::parseRawMessage(QString msg)
{
    // "{\"type\":\"reboot\",\"reason\":4,\"code1\":0,\"stack\":0,\"version\":81230,\"blversion\":0}"

}

void InukCommandHandler::handleRawMessage(QString msg)
{
    //qDebug() << "handle msg : " << msg;
    QtJsonHandler j;
    QJsonObject obj = j.stringToObject(msg);

    if (!obj.isEmpty()) {
        qDebug() << obj;
        mqtt->sendString(msg);
    }else {
     //    qDebug() << "handle msg : " << msg;
    }
}

void InukCommandHandler::periodicCallback()
{
    emit sendMessage("get_config this adv");
}


