#ifndef MESSAGE_H
#define MESSAGE_H

#define JSON_ERROR_STRING       "json_error";
#define JSON_ERROR_INT          -1000;

#define IS_JSON_NUMBER(json, v) (json.contains(v) && json[v].isDouble())
#define IS_JSON_STRING(json, v) (json.contains(v) && json[v].isString())

#define JSON_TO_STRING(json, v) ( IS_JSON_STRING(json, v) ? json[v].toString() : "json_error"  )
#define JSON_TO_INT(json, v) ( IS_JSON_NUMBER(json, v) ? json[v].toInt() : -1  )

#include <QObject>
#include <QJsonObject>

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = nullptr);
    QString type;

signals:

public slots:
};

#endif // MESSAGE_H



// Messages from mesh
// {"type":"mesh_disconnect","partnerId":2935}
// {"type":"cluster_disconnect","size":1}
// {"code":0,"module":3,"nodeId":12380,"requestHandle":0,"type":"set_pin_config_result"}
// {"code":0,"text":"OK","type":"error"}
// {"clusterId":3092983900,"clusterSize":2,"type":"update_joinme"}
