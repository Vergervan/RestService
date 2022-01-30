#include "httprequest.h"

HttpRequest::HttpRequest(QString body)
{
    this->body = body;
    body = body.remove('\r');
    reqLines = body.split('\n');
    qDebug() << reqLines[reqLines.length()-1];
    QRegularExpressionMatch match;
    int offset = 0;
    bool hasMatch = false;
    foreach (const auto &pattern, requests)
    {
        match = pattern.match(body);
        if(match.hasMatch()){
            hasMatch = true;
            break;
        }
        ++offset;
    }
    if(!hasMatch){
        _error = true;
        return;
    }
    _type = static_cast<RequestType>(offset);
}

bool HttpRequest::hasRoute(QString routeName)
{
    int matchRoute = body.indexOf(routeName);
    if(matchRoute == -1) return false;
    int offset = matchRoute + routeName.length() + 1;
    int start = offset;
    int valLen = 0;
    while(!(body[offset] == ' '))
    {
        valLen++;
        offset++;
    }
    QString routeValue = body.mid(start, valLen);
    routeValues.insert( { routeName, routeValue });
    return true;
}

QString HttpRequest::getRouteValue(QString routeName)
{
    return routeValues[routeName];
}

QString HttpRequest::getBody()
{
    return reqLines[reqLines.length()-1];
}
