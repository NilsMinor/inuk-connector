#ifndef QTJSONHANDLER_H
#define QTJSONHANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

class QtJsonHandler : public QObject
{
    Q_OBJECT
public:
    explicit QtJsonHandler(QObject *parent = nullptr);
    QJsonObject stringToObject(const QString& in);

signals:

public slots:
};

#endif // QTJSONHANDLER_H
