#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QTcpSocket>
#include <sstream>
#include <map>
#include <QString>
#include <string>
#include <iostream>

class HttpResponse
{
public:

enum ResponseCode
{
    Ok = 200,
    BadRequest = 400
};

enum MessageType{
    Json, Html
};

    static void sendResponse(const QTcpSocket*, ResponseCode, MessageType = Html, QString = QString());
private:
    HttpResponse();
};

#endif // HTTPRESPONSE_H
