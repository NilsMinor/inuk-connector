#include "connectionmessage.h"

ConnectionMessage::ConnectionMessage()
{

}

ConnectionMessage::~ConnectionMessage()
{

}

void ConnectionMessage::read(const QJsonObject &json)
{
    m_type              = JSON_TO_STRING(json,"type");
    m_nodeId            = JSON_TO_INT(json, "nodeId");
    m_module            = JSON_TO_INT(json, "module");
    m_dBmRX             = JSON_TO_INT(json, "dBmRX");
    m_dBmTX             = JSON_TO_INT(json, "dBmTX");
    m_calibratedTX      = JSON_TO_INT(json, "calibratedTX");
    m_deviceType        = JSON_TO_INT(json, "deviceType");
    m_manufacturerId    = JSON_TO_INT(json, "manufacturerId");
    m_networkId         = JSON_TO_INT(json, "networkId");
    m_nodeVersion       = JSON_TO_INT(json, "nodeVersion");
    m_chipId            = JSON_TO_STRING(json,"m_chipId");
    m_serialNumber      = JSON_TO_STRING(json,"serialNumber");
    m_accessAddress     = JSON_TO_STRING(json,"accessAddress");
    m_blVersion         = JSON_TO_INT(json, "blVersion");

    // TODO implement m_groupIds array
    // QVector<int>    m_groupIds();
}

void ConnectionMessage::write(QJsonObject &json) const
{
    Q_UNUSED(json);

}

QStringList ConnectionMessage::print()
{
    QStringList list ;
    list.append("m_type :" + m_type);
    list.append("m_nodeId :" + QString::number(m_nodeId));
    list.append("m_module :" + QString::number(m_module));
    list.append("m_dBmRX :" + QString::number(m_dBmRX));
    list.append("m_dBmTX :" + QString::number(m_dBmTX));
    list.append("m_calibratedTX :" + QString::number(m_calibratedTX));
    list.append("m_deviceType :" + QString::number(m_deviceType));
    list.append("m_manufacturerId :" + QString::number(m_manufacturerId));
    list.append("m_networkId :" + QString::number(m_networkId));
    list.append("m_nodeVersion :" + QString::number(m_nodeVersion));
    list.append("m_chipId :" + m_chipId);
    list.append("m_serialNumber :" + m_serialNumber);
    list.append("m_accessAddress :" + m_accessAddress);
    list.append("m_blVersion :" + QString::number(m_blVersion));

    return list;
}
