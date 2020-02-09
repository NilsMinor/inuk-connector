#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include "message.h"

// mesh log message
class LogMessage
{
public:
    LogMessage();
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    QStringList print ();

    QString         m_type;
    QString         m_file;
    QString         m_line;
    QString         m_message;
    QString         m_tag;
};

#endif // LOGMESSAGE_H
