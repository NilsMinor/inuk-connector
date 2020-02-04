#ifndef CONNECTIONMESSAGE_H
#define CONNECTIONMESSAGE_H

// action this status get_connections
//{
//  "type": "connections",
//  "nodeId": 12380,
//  "module": 3,
//  "partners": [
//    0,
//    0,
//    0,
//    0
//  ],
//  "rssiValues": [
//    0,
//    0,
//    0,
//    0
//  ]
//}


#include <QObject>
#include "message.h"
#include <QJsonObject>
#define JSON_ERROR_STRING       "json_error";
#define JSON_ERROR_INT          -1000;

#define IS_JSON_NUMBER(json, v) (json.contains(v) && json[v].isDouble())
#define IS_JSON_STRING(json, v) (json.contains(v) && json[v].isString())

#define JSON_TO_STRING(json, v) ( IS_JSON_STRING(json, v) ? json[v].toString() : "json_error"  )
#define JSON_TO_INT(json, v) ( IS_JSON_NUMBER(json, v) ? json[v].toInt() : -1  )

class ConnectionMessage
{
public:
    ConnectionMessage();
    ~ConnectionMessage();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    QStringList print ();

private:
    QString         m_type;
    int             m_nodeId;
    int             m_module;
    int             m_dBmRX;
    int             m_dBmTX;
    int             m_calibratedTX;
    int             m_deviceType;
    int             m_manufacturerId;
    int             m_networkId;
    int             m_nodeVersion;
    QString         m_chipId;
    QString         m_serialNumber;
    QString         m_accessAddress;
    QVector<int>    m_groupIds();
    int             m_blVersion;

};

#endif // CONNECTIONMESSAGE_H
