#include "connectionsetting.h"

ConnectionSetting::ConnectionSetting()
{
 this->settings = {

      "/dev/tty.SLAB_USBtoUART",         // port name
        QSerialPort::Baud115200,        // baudrate
        QSerialPort::DataBits::Data8,   // data bits
        QSerialPort::Parity::NoParity,  // parity
        QSerialPort::StopBits::OneStop, // stop bits
        QSerialPort::FlowControl::HardwareControl, // flow control
        false // local echo
    };

}

ConnectionSetting::Settings ConnectionSetting::getSettings()
{
    return this->settings;
}

void ConnectionSetting::loadFromJson(QString path)
{

}

void ConnectionSetting::saveToJson(QString path)
{


}

QDataStream & operator>>(QDataStream &in, ConnectionSetting::Settings &s) {
    in >> s;
    return in;
}

QDataStream & operator << (QDataStream& out, ConnectionSetting::Settings & s) {
    out << s;
    return out;
}
