#ifndef RESTFULSERVER_H
#define RESTFULSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QThread>
#include "messagehandler.h"
#include "messagereader.h"

class RestfulServer : public QTcpServer{

public:
    RestfulServer(QObject* parent = nullptr) : QTcpServer(parent)
    {
        handler = new MessageHandler;
    }
    ~RestfulServer();
protected:
    void incomingConnection(qintptr socket) override;
private:
    MessageHandler* handler;
};

#endif // RESTFULSERVER_H
