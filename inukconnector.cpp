#include "inukconnector.h"

InukConnector::InukConnector(QObject *parent) : QObject(parent)
{
    this->settingsHandler = new ConnectionSetting();
    serialHandler = new QSerialPort(this);
    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, &QTimer::timeout, this, &InukConnector::scannConnection);
    connect(serialHandler, &QSerialPort::errorOccurred, this, &InukConnector::handleError);
    connect(serialHandler, &QSerialPort::readyRead, this, &InukConnector::readData);

    this->reconnectTimer->start(RECONNECTION_INTERVALL);
}

InukConnector::~InukConnector()
{
    this->closeSerialPort();
    delete serialHandler;
}

void InukConnector::scannConnection()
{
    if (settingsHandler) {
         qDebug() << "Try to connect to : " << settingsHandler->getSettings().name;
         if(!serialHandler->isOpen()) {
             this->openSerialPort(settingsHandler->getSettings());
         }
         else {

         }
    }
}

void InukConnector::openSerialPort(ConnectionSetting::Settings s)
{
    if (s.name != "") {
        serialHandler->setPortName(s.name);
        serialHandler->setBaudRate(s.baudRate);
        serialHandler->setDataBits(s.dataBits);
        serialHandler->setParity(s.parity);
        serialHandler->setStopBits(s.stopBits);
        serialHandler->setFlowControl(s.flowControl);
        if (serialHandler->open(QIODevice::ReadWrite)) {
            this->reconnectTimer->stop();
            qDebug() << "connected to : " << s.name;
        } else {
            qDebug() << "error can't connect to " << s.name;
        }
    }

}

void InukConnector::closeSerialPort()
{
    if (serialHandler->isOpen())
        serialHandler->close();

    this->reconnectTimer->start(RECONNECTION_INTERVALL);
}

void InukConnector::writeData(const QByteArray &data)
{
    qDebug() << "uart-tx: " << data;
    serialHandler->write(data);
}

void InukConnector::readData()
{
    const QByteArray data = serialHandler->readAll();
    QString msg = QString(data);
    msg.replace("\r\n", "");    // remove line-feed
//    msg.replace("\"", "@");      // remove char
//    msg.replace("@", QString('A'));      // remove char

    cmdHandler.handleRawMessage(msg);
}

void InukConnector::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << "error handler: " << error;
        closeSerialPort();
    }
}

