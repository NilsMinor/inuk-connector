#ifndef CONNECTIONSETTING_H
#define CONNECTIONSETTING_H

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "qtjsonhandler.h"


#define CONNECTION_FILE_PATH "config/connection.json"
#define LOCAL_CONFIG_FILE "/Users/nilsminor/Documents/inuit/inuk-connector/inuk-connector/config/connection.json"

class ConnectionSetting
{
public:
    ConnectionSetting();
    struct Settings {
        QString portName;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
        bool localEchoEnabled;
    };
    ConnectionSetting::Settings getSettings ();
private :
    Settings settings;
    void loadFromJson (bool local);
    Settings jsonToSettings (QJsonObject json);
    void saveToJson (QString path);
};

#endif // CONNECTIONSETTING_H
