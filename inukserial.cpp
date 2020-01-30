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

        // the serial port readAll function will receive data with variable length
        const QByteArray data = serial->readAll();
        QString msg = QString(data);

        // add message to buffer
        sBuffer.append(msg);

        // split the buffer by line feed
        // split the buffer by line feed token
        // fStr contains the message
        // sStr contains the remaining data of the message buffer
        if (sBuffer.contains(LF_TOKEN)) {
            QStringList list = sBuffer.split(LF_TOKEN);

            if (list.length() == 2) {
                QString fStr = list[0];
                QString sStr = list[1];

                // set remaining data to buffer content
                sBuffer = sStr;

                DEBUG << "serial received " << fStr;
                emit receivedData(fStr);
            }
        }
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

