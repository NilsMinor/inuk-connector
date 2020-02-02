#ifndef INUKSERIAL_H
#define INUKSERIAL_H

#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include "connectionsetting.h"

/* InukSerial is used to connect to an nrf51/52 mesh module via usb/uart.
 * After initialization, it starts to scan the ports and tries to
 * connect to the module with settings from serial.txt
 *
 * TODO structure of serial.txt
 *
 */

#define RECONNECTION_INTERVALL  1000
#define LF_TOKEN                "\r\n"

class InukSerial : public QObject
{
    Q_OBJECT
public:
    explicit InukSerial(QObject *parent = nullptr);
        ~InukSerial();
    void startScanning (qint16 timeout = RECONNECTION_INTERVALL);


signals:

    void started();
    void connected (QString portName);
    void disconnected (QString portName);
    void receivedData (QString & msg);

public slots:
    void sendMessage (QString msg);


private:
    ConnectionSetting *settingsHandler;
    QTimer *reconnectTimer;
    QSerialPort *serial = nullptr;
    QString sBuffer;

    void openSerialPort(ConnectionSetting::Settings s);
    void closeSerialPort();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

private slots :
    void scannConnection ();
    void writeData(const QByteArray &data);



};

#endif // INUKSERIAL_H
