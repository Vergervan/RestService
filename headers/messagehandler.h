#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QMap>
#include <sstream>
#include "headers/httprequest.h"
#include "headers/httpresponse.h"

class MessageHandler : public QObject
{
    Q_OBJECT

public:
    void process(QTcpSocket*, QString);
    void processApi(QTcpSocket*, HttpRequest&);
    int addNewValueInTable(QString value);
    void writeInJournal(HttpRequest::RequestType, int);
    QString getTableBody();
    inline QMap<int, QString> getTable() { return table; }
private:
    QMap<int, QString> table;
    QStringList journal;
};

#endif // MESSAGEHANDLER_H
