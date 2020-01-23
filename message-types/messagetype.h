#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <QObject>

class MessageType : public QObject
{
    Q_OBJECT
public:
    explicit MessageType(QObject *parent = nullptr);
    virtual ~MessageType() {}

    virtual void read(const QJsonObject &jsonObj) = 0;
    virtual void print(void) = 0;
    virtual bool test(const QJsonObject &jsonObj) = 0;
signals:

public slots:
};

#endif // MESSAGETYPE_H
