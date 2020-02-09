#include "logmessage.h"

LogMessage::LogMessage()
{

}

void LogMessage::read(const QJsonObject &json)
{
    m_type          = JSON_TO_STRING(json,"type");
    m_file          = JSON_TO_STRING(json, "file");
    m_line          = JSON_TO_STRING(json, "line");
    m_message       = JSON_TO_STRING(json, "message");
    m_tag           = JSON_TO_STRING(json, "tag");
}

void LogMessage::write(QJsonObject &json) const
{
    Q_UNUSED(json);
}

QStringList LogMessage::print()
{
    QStringList list ;
    list.append("m_type :" + m_type);
    list.append("m_file :" + m_file);
    list.append("m_line :" + m_line);
    list.append("m_message :" + m_message);
    list.append("m_tag :" + m_tag);
    return list;
}
