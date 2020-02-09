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

void InukCommandHandler::parseRawMessage(QString &msg)
{
    emit messsageHandledString(msg);
}

void InukCommandHandler::parseRawMessage(QJsonObject &obj)
{
    QStringList debug;

    if (MSG_TYPE_IS(obj, MSG_DEVICE_INFO)) {
        ConnectionMessage device_info;
        device_info.read(obj);
        debug = device_info.print();
        emit nodeConnected(QString::number(device_info.m_nodeId), true);
    }
    else if (MSG_TYPE_IS(obj, MSG_MESH_CONNECT)) {
        MeshConnectMessage meshConnect;
        meshConnect.read(obj);
        debug = meshConnect.print();
        emit nodeConnected(QString::number(meshConnect.m_partnerId));
    }
    else if (MSG_TYPE_IS(obj, MSG_MESH_DISCONNECT)) {
        MeshConnectMessage meshDisconnect;
        meshDisconnect.read(obj);
        debug = meshDisconnect.print();
        emit nodeDisconnected(QString::number(meshDisconnect.m_partnerId));
    }
    else if (MSG_TYPE_IS(obj, MSG_MESH_LOG)) {
        LogMessage log;
        log.read(obj);
    } else {
        WARN << "message type not supported :" << MSG_TYPE(obj);
    }

    if (!debug.isEmpty()) {
        DEBUG << "message handled" << debug;
    }

    emit messsageHandledJson(obj);
}

QString InukCommandHandler::createMessage(QStringList list)
{
    QString msg;
    for (int i = 0; i < list.size(); ++i) {
        if (i == list.size() -1)
            msg += list.at(i);
        else
            msg += list.at(i) + " ";
    }
    return msg;
}

void InukCommandHandler::connected()
{
    DEBUG << "serial and mqtt is now connected";

    // get information about gatway
    QString msg = this->createMessage({ACTION, GATEWAY_ID, STATUS, DEVICE_INFO});
    emit sendMessageToSerial(msg);
}

void InukCommandHandler::handleRawMessage(QString &msg)
{
    QtJsonHandler j;
    QJsonObject obj = j.stringToObject(msg);

    if (!obj.isEmpty()) {
        parseRawMessage(obj);

    }else {
        parseRawMessage(msg);
    }
}

void InukCommandHandler::periodicCallback()
{
    // emit sendMessage("action this status get_device_info");
}



