#include "headers/RestfulServer.h"
#include "headers/widget.h"

RestfulServer::~RestfulServer()
{
    delete handler;
}

void RestfulServer::incomingConnection(qintptr socketDscr){
    qDebug("New connection");
    QTcpSocket* client = new QTcpSocket;
    client->setSocketDescriptor(socketDscr);
    QThread* clientThread = new QThread;
    MessageReader* reader = new MessageReader(client, this->handler);
    connect(reader, SIGNAL(finishedRead()), qobject_cast<Widget*>(parent()), SLOT(refreshTableData()));
    reader->moveToThread(clientThread);

    connect(client, SIGNAL(readyRead()), reader, SLOT(read()));
    connect(client, SIGNAL(disconnected()), clientThread, SLOT(quit()));
    connect(client, SIGNAL(disconnected()), reader, SLOT(deleteLater()));
    connect(clientThread, SIGNAL(finished()), clientThread, SLOT(deleteLater()));

    clientThread->start();
}
