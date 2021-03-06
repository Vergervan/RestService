#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QString>
#include <QRegularExpression>
#include <vector>
#include <QDebug>
#include <map>

class HttpRequest
{
private:
    const std::vector<QString> requests { "GET", "POST", "PUT", "DELETE" };
public:
    enum RequestType { Get, Post, Put, Delete };

    HttpRequest(QString);
    inline RequestType getRequestType() { return _type; }
    inline bool hasError() { return _error; }
    bool hasRoute(QString);
    QString getRouteValue(QString routeName);
    QString getBody();
private:
    bool _error = false;
    QString body; //Body of the request
    std::map<QString, QString> routeValues;
    RequestType _type; //Type of the request
    QStringList reqLines; //Every line of the request
};

#endif // HTTPREQUEST_H
