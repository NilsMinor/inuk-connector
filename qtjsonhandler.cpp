#include "qtjsonhandler.h"

#include <QLoggingCategory>
#define LOGGING_CAT QLoggingCategory("inuk.json")
#define DEBUG       qDebug(LOGGING_CAT)
#define WARN        qWarning(LOGGING_CAT)


QtJsonHandler::QtJsonHandler(QObject *parent) : QObject(parent)
{

}

QJsonObject QtJsonHandler::stringToObject(const QString& in)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject()) {
            obj = doc.object();
        }
        else {
            DEBUG << "Document is not an object" << endl;
        }
    }
    else {
        // qDebug() << "Invalid JSON...\n" << in << endl;
    }
    return obj;
}
