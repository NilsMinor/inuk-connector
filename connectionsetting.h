#ifndef CONNECTIONSETTING_H
#define CONNECTIONSETTING_H

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QDir>
#define CONNECTION_FILE_PATH "config/connection.json"

class ConnectionSetting
{
public:
    ConnectionSetting();
    struct Settings {
        QString name;
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
    void loadFromJson ();
    void saveToJson (QString path);
};

#endif // CONNECTIONSETTING_H
