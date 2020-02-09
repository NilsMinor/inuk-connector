#ifndef MESHCONNECTMESSAGE_H
#define MESHCONNECTMESSAGE_H

// mesh conection message
#include "message.h"

class MeshConnectMessage
{
public:
    MeshConnectMessage();
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    QStringList print ();

    QString         m_type;
    int             m_partnerId;
};

#endif // MESHCONNECTMESSAGE_H
