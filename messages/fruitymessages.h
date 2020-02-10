#ifndef FRUITYMESSAGES_H
#define FRUITYMESSAGES_H

#include <QString>
#include "meshconnectmessage.h"
#include "connectionmessage.h"
#include "logmessage.h"


#define MSG_DEVICE_INFO             ("device_info")
#define MSG_MESH_CONNECT            ("mesh_connect")
#define MSG_MESH_DISCONNECT         ("mesh_disconnect")
#define MSG_MESH_LOG                ("log")
#define MSG_MESH_ERROR              ("error")
#define MSG_CLUSTER_DISCONNECT      ("cluster_disconnect")

#define MSG_TYPE_IS(json,e)         (json["type"].toString() == QString(e))
#define MSG_TYPE(json)              (json["type"].toString())
#define GET_NODE_ID(json)           (json["nodeId"].toInt())

#endif // FRUITYMESSAGES_H
