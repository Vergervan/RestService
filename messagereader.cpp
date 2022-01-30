#include "messagereader.h"

MessageReader::MessageReader(QTcpSocket* client, MessageHandler* handler)
{
    this->client = client;
    this->handler = handler;
    qDebug("Reader was created");
}

MessageReader::~MessageReader()
{
    qDebug("Reader removed");
}

void MessageReader::read()
{
    qDebug("Start read");
    QByteArray total_data, data_buffer;
    while(true)
    {
        data_buffer = client->read(1024);
        if(data_buffer.isEmpty()) {
            break;
        }
        total_data.append(data_buffer);
    }
    QString message_content(total_data);

    qDebug() << message_content;
    handler->process(client, message_content);
    emit finishedRead();
}
