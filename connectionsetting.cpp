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

void ConnectionSetting::loadFromJson( )
{
    QFile file;
    QString path = QDir::currentPath() + CONNECTION_FILE_PATH;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.isOpen()) {
        QString val = file.readAll();
        file.close();

        qDebug() << val;
    } else {
        qDebug() << "error - no valid setting in file " << path;
    }

}

void ConnectionSetting::saveToJson(QString path)
{
    Q_UNUSED(path);

}
