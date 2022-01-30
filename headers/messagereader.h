#ifndef MESSAGEREADER_H
#define MESSAGEREADER_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include "headers/messagehandler.h"

class MessageReader : public QObject
{
    Q_OBJECT

public:
    MessageReader(QTcpSocket*, MessageHandler*);
    ~MessageReader();
private:
    QTcpSocket* client;
    MessageHandler* handler;
public slots:
    void read();
signals:
    void finishedRead();
};

#endif // MESSAGEREADER_H
