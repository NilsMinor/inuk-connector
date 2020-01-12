#ifndef CONNECTIONSETTING_H
#define CONNECTIONSETTING_H

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDataStream>

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
    void loadFromJson (QString path);
    void saveToJson (QString path);
    friend QDataStream &operator<<(QDataStream& out, const ConnectionSetting::Settings & s);
    friend QDataStream &operator>>(QDataStream& in, ConnectionSetting::Settings & s);

};

#endif // CONNECTIONSETTING_H
