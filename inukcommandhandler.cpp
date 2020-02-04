#include "inukcommandhandler.h"
#include <QObject>
#include "qtjsonhandler.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.cmd")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)

InukCommandHandler::InukCommandHandler(QObject *parent) : QObject(parent)
{
    periodicTimer = new QTimer(this);
    connect(periodicTimer, &QTimer::timeout, this, &InukCommandHandler::periodicCallback);
   // periodicTimer->start(1000);

}

QString InukCommandHandler::parseRawMessage(QString msg)
{
    // "{\"type\":\"reboot\",\"reason\":4,\"code1\":0,\"stack\":0,\"version\":81230,\"blversion\":0}"

}

void InukCommandHandler::handleRawMessage(QString &msg)
{
    QtJsonHandler j;
    QJsonObject obj = j.stringToObject(msg);

    if (!obj.isEmpty()) {

        DEBUG << obj;
        if (obj["type"].toString() == "device_info") {
             ConnectionMessage msg;
             msg.read(obj);

             DEBUG << msg.print();

        } else {
            DEBUG << "type not supported";
        }

        emit messsageHandledJson(obj);

    }else {
        emit messsageHandledString(msg);
    }
}

void InukCommandHandler::periodicCallback()
{
    emit sendMessage("action this status get_device_info");
}


