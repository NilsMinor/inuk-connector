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
    periodicTimer->start(1000);


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
        emit messsageHandledJson(obj);

    }else {
        emit messsageHandledString(msg);
    }
}

void InukCommandHandler::periodicCallback()
{
    emit sendMessage("get_config this adv");
}


