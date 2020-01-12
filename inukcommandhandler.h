#ifndef INUKCOMMANDHANDLER_H
#define INUKCOMMANDHANDLER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

class InukCommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit InukCommandHandler(QObject *parent = nullptr);

private :
    QString parseRawMessage (QString msg);
    QJsonObject ObjectFromString(const QString& in);

signals:

public slots:
    void handleRawMessage (QString msg);
};

#endif // INUKCOMMANDHANDLER_H
