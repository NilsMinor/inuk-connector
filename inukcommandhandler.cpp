#include "inukcommandhandler.h"
#include <QObject>
InukCommandHandler::InukCommandHandler(QObject *parent) : QObject(parent)
{

}

QString InukCommandHandler::parseRawMessage(QString msg)
{
    // "{\"type\":\"reboot\",\"reason\":4,\"code1\":0,\"stack\":0,\"version\":81230,\"blversion\":0}"

}

void InukCommandHandler::handleRawMessage(QString msg)
{
    //qDebug() << "handle msg : " << msg;
    QJsonObject obj = ObjectFromString(msg);
    QVariantMap map = obj.toVariantMap();
    if (!obj.isEmpty()) {
        qDebug() << map;
    }else {
        //qDebug() << "handle msg : " << msg;
    }
}

QJsonObject InukCommandHandler::ObjectFromString(const QString& in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        // qDebug() << "Invalid JSON...\n" << in << endl;
    }

    return obj;
}

