#ifndef RESTFULSERVER_H
#define RESTFULSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QThread>
#include "headers/messagehandler.h"
#include "headers/messagereader.h"

class RestfulServer : public QTcpServer{

public:
    RestfulServer(QObject* parent = nullptr) : QTcpServer(parent)
    {
        handler = new MessageHandler;
    }
    inline QMap<int, QString> getTable() { return handler->getTable(); }
    ~RestfulServer();
protected:
    void incomingConnection(qintptr socket) override;
private:
    MessageHandler* handler;
};

#endif // RESTFULSERVER_H
