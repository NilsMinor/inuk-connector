#include "inukserial.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.serial")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)


InukSerial::InukSerial(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    settingsHandler = new ConnectionSetting();
    reconnectTimer = new QTimer(this);

    connect(reconnectTimer, &QTimer::timeout, this, &InukSerial::scannConnection);
    connect(serial, &QSerialPort::errorOccurred, this, &InukSerial::handleError);
    connect(serial, &QSerialPort::readyRead, this, &InukSerial::readData);
    // connect(this, &InukCommandHandler::sendMessage, this, &InukConnector::writeData);

    startScanning(RECONNECTION_INTERVALL);
}

InukSerial::~InukSerial()
{
    this->closeSerialPort();
    delete serial;
    delete settingsHandler;
}

void InukSerial::startScanning(qint16 timeout)
{
    emit started();
    DEBUG << "start timer with timeout : " << timeout;
    this->reconnectTimer->start(timeout);
}

void InukSerial::scannConnection()
{
    if (settingsHandler) {
         DEBUG << "try to connect to : " << settingsHandler->getSettings().portName;
         if(!serial->isOpen()) {
             this->openSerialPort(settingsHandler->getSettings());
         }
         else {

         }
    }
}

void InukSerial::openSerialPort(ConnectionSetting::Settings s)
{
    if (s.portName != "") {
        serial->setPortName(s.portName);
        serial->setBaudRate(s.baudRate);
        serial->setDataBits(s.dataBits);
        serial->setParity(s.parity);
        serial->setStopBits(s.stopBits);
        serial->setFlowControl(s.flowControl);
        if (serial->open(QIODevice::ReadWrite)) {
            this->reconnectTimer->stop();
            DEBUG << "connected to : " << s.portName;
            emit connected(serial->portName());
        } else {
            DEBUG << "error can't connect to " << s.portName;
        }
    }

}

void InukSerial::closeSerialPort()
{
    if (serial->isOpen()) {
        DEBUG << "close serial port";
        serial->close();
        emit disconnected(serial->portName());
    }

    startScanning(RECONNECTION_INTERVALL);
}

void InukSerial::writeData(const QByteArray &data)
{
    if (serial->isOpen()) {
        DEBUG << "send via serial port " << data;
        QString msg = QString(data) + "\r\n";
        serial->write(data+ "\r");
    }
    else {
        WARN << "serial connection is not open";
    }
}

void InukSerial::readData()
{
    if (serial->isOpen()) {
        const QByteArray data = serial->readAll();
        DEBUG << "received via serial port " << data;
        QString msg = QString(data);
        msg.replace("\r\n", "");    // remove line-feed
        emit receivedData(msg);
    }
    else {
        WARN << "serial connection is not open";
    }
}

void InukSerial::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        WARN << "error handler: " << error;
        closeSerialPort();
    }
}

