#include "statusmessage.h"

StatusMessage::StatusMessage()
{
    this->keys << "blversion" << "code1" << "reason" << "stack" << "type" << "version";

}

void StatusMessage::read(const QJsonObject &jsonObj)
{
    this->jsonObj = jsonObj;
    this->blversion = jsonObj["blversion"].toInt();
    this->code1 = jsonObj["code1"].toInt();
    this->reason = jsonObj["reason"].toInt();
    this->stack = jsonObj["stack"].toInt();
    this->type = jsonObj["type"].toString();
    this->version = jsonObj["version"].toInt();
}

void StatusMessage::print()
{
    qDebug () << jsonObj;
}

bool StatusMessage::test(const QJsonObject &jsonObj)
{
    return jsonObj.keys() == this->keys;
}
