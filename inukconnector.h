#ifndef INUKCONNECTOR_H
#define INUKCONNECTOR_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QTimer>
#include <QApplication>
#include <QProcess>


#include "inukserial.h"
#include "inukcommandhandler.h"

class InukConnector : public QObject
{
    Q_OBJECT
public:
    explicit InukConnector(QObject *parent = nullptr);
    ~InukConnector();

    static int const EXIT_CODE_REBOOT = -110;

private:

    InukCommandHandler *    cmd;
    InukSerial *            serial;
    InukMQTT *              mqtt;

    static void cbTest (QString msg);

    static void restart (QString msg);
signals:

public slots:
    void printMessage (QString &msg);
    void printJSON (QJsonObject &json);



    // serial
    void serialStarted( );
    void serialConnected(QString portName);
    void serialDisconnected(QString portName);

    // mqtt
    void mqttStarted ();
    void mqttConnected (QString hostName);
    void mqttError (QString error);
    void mqttDisconnected(QString hostanem);
};

#endif // INUKCONNECTOR_H
