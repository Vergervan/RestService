#ifndef RESTFULSERVER_H
#define RESTFULSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QThread>
#include <QtSql>
#include "headers/messagehandler.h"
#include "headers/messagereader.h"

class RestfulServer : public QTcpServer{

public:
    inline RestfulServer(QObject* parent = nullptr) : QTcpServer(parent)
    {
        handler = new MessageHandler;
    }
    inline void setDatabase(QSqlDatabase db)
    {
        this->db = db;
        handler->setDatabase(db);
    }
    ~RestfulServer();
protected:
    void incomingConnection(qintptr socket) override;
private:
    MessageHandler* handler;
    QSqlDatabase db;
};

#endif // RESTFULSERVER_H
