#include "httpresponse.h"

void HttpResponse::sendResponse(const QTcpSocket* client, ResponseCode code, MessageType type, QString responseMessage)
{
    QTcpSocket dClient;
    dClient.setSocketDescriptor(client->socketDescriptor());
    std::stringstream ss;
    ss << "HTTP/1.1 " << std::to_string((int)code) << "\n";
    ss << "Content-type: ";
    switch(type)
    {
    case Json:
        ss << "application/json";
        break;
    case Html:
        ss << "text/html";
        break;
    }
    ss << "\n";

    ss << "\n" << responseMessage.toStdString();
    qDebug() << ss.str().c_str();
    dClient.write(ss.str().c_str());
    dClient.flush();
}
