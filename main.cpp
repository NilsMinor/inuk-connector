#include <QCoreApplication>
#include <QDebug>
#include "inukconnector.h"
#include <inukmqtt.h>

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.main")
#define DEBUG qDebug(LOGGING_CAT)
#define WARN qWarning(LOGGING_CAT)

#define DEBUG_PATTERN   "[%{time process}] (%{category}) %{message} (%{file}:%{line})"
#define PROD_PATTERN    "[%{time process}] (%{category}) %{message}"

int main(int argc, char *argv[])
{
    int currentExitCode = 0;
     QCoreApplication a(argc, argv);
    do {

        qSetMessagePattern(PROD_PATTERN); // DEBUG_PATTERN | PROD_PATTERN

        QLoggingCategory::setFilterRules(QStringLiteral("inuk.*.debug=false\n"
                                                    "inuk.serial*.debug=true\n"
                                                    "inuk.mqtt.debug=false\n"
                                                    "inuk.cmd.debug=false\n"
                                                    "inuk.con.debug=false\n"
                                                    ));

        InukConnector * inuk_conector = new InukConnector();
        currentExitCode = a.exec();
     } while( currentExitCode == InukConnector::EXIT_CODE_REBOOT );


    return a.exec();
}
