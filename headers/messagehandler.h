#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <sstream>
#include <QDebug>
#include <QtSql>
#include <QtGlobal>
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
    inline void setDatabase(QSqlDatabase db) { this->db = db; }
private:
    QSqlDatabase db;
};

#endif // MESSAGEHANDLER_H
