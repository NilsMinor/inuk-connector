#ifndef INUKCONNECTOR_H
#define INUKCONNECTOR_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QTimer>
#include "connectionsetting.h"
#include "inukcommandhandler.h"

#define RECONNECTION_INTERVALL 1000

class InukConnector : public QObject
{
    Q_OBJECT
public:
    explicit InukConnector(QObject *parent = nullptr);
    ~InukConnector();

private:
    ConnectionSetting *settingsHandler;
    QSerialPort *serialHandler = nullptr;
    InukCommandHandler *cmdHandler;
    QTimer *reconnectTimer;

    void openSerialPort(ConnectionSetting::Settings s);
    void closeSerialPort();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

signals:

public slots:
    void writeData(const QByteArray &data);
    void scannConnection ();
};

#endif // INUKCONNECTOR_H
