#include "connectionsetting.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.serial.settings")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)

ConnectionSetting::ConnectionSetting()
{
 this->settings = {

      "/dev/tty.SLAB_USBtoUART",        // port name
        QSerialPort::Baud115200,        // baudrate
        QSerialPort::DataBits::Data8,   // data bits
        QSerialPort::Parity::NoParity,  // parity
        QSerialPort::StopBits::OneStop, // stop bits
        QSerialPort::FlowControl::HardwareControl, // flow control
        false // local echo
    };
    // this->loadFromJson(true);
}

ConnectionSetting::Settings ConnectionSetting::getSettings()
{
    return this->settings;
}

void ConnectionSetting::loadFromJson(bool local)
{
    QFile file;
    QString path;
    if (local) {
         path = LOCAL_CONFIG_FILE;
    } else {
        path = QDir::currentPath() + CONNECTION_FILE_PATH;
    }

    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.isOpen()) {
        QString content = file.readAll();
        file.close();

        QtJsonHandler j;
        QJsonObject json =  j.stringToObject(content);
        DEBUG << json["portName"].toString();

    } else {
        WARN << "error - no valid setting in file " << path;
    }
}

ConnectionSetting::Settings ConnectionSetting::jsonToSettings(QJsonObject json)
{
    if (!json.isEmpty()) {
        this->settings.portName = json["portName"].toString();
        // TODO implement rest
        this->settings.localEchoEnabled = json["localEchoEnabled"].toBool();
    }
}

void ConnectionSetting::saveToJson(QString path)
{
    Q_UNUSED(path);

}
