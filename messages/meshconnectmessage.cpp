#include "meshconnectmessage.h"

MeshConnectMessage::MeshConnectMessage()
{

}

void MeshConnectMessage::read(const QJsonObject &json)
{
    m_type              = JSON_TO_STRING(json,"type");
    m_partnerId         = JSON_TO_INT(json, "partnerId");
}

void MeshConnectMessage::write(QJsonObject &json) const
{
    Q_UNUSED(json);
}

QStringList MeshConnectMessage::print()
{
    QStringList list ;
    list.append("m_type :" + m_type);
    list.append("m_nodeId :" + QString::number(m_partnerId));
    return list;
}
