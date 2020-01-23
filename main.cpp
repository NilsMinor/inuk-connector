#include <QCoreApplication>
#include <QDebug>
#include "inukconnector.h"
#include <inukmqtt.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InukConnector * inuk_conector = new InukConnector();

    return a.exec();
}
