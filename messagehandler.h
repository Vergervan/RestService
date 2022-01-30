#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QMap>
#include <sstream>
#include "httprequest.h"
#include "httpresponse.h"

class MessageHandler : public QObject
{
    Q_OBJECT

public:
    void process(QTcpSocket*, QString);
    void processApi(QTcpSocket*, HttpRequest&);
    int addNewValueInTable(QString value);
    void writeInJournal(HttpRequest::RequestType, int);
    QString getTableBody();
private:
    QMap<int, QString> table;
    QStringList journal;
};

#endif // MESSAGEHANDLER_H
