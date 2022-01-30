#include "httprequest.h"

//Parsing a body in the constructor
HttpRequest::HttpRequest(QString body)
{
    this->body = body;
    body = body.remove('\r');
    reqLines = body.split('\n');
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
    while(!(body[offset] == ' ')) //Calculating a string length of the value
    {
        valLen++;
        offset++;
    }
    QString routeValue = body.mid(start, valLen); //Substring a body to get value
    routeValues.insert( { routeName, routeValue }); //Adding route and it's value to map
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
