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
    const std::vector<QRegularExpression> requests { QRegularExpression("GET*"),
                                                     QRegularExpression("POST*"),
                                                     QRegularExpression("PUT*"),
                                                     QRegularExpression("DELETE*") };
public:
    enum RequestType{
        Get, Post, Put, Delete
    };

    HttpRequest(QString);
    inline RequestType getRequestType() { return _type; }
    inline bool hasError() { return _error; }
    bool hasRoute(QString);
    QString getRouteValue(QString routeName);
    QString getBody();
private:
    bool _error = false;
    QString body;
    std::map<QString, QString> routeValues;
    RequestType _type;
    QStringList reqLines;
};

#endif // HTTPREQUEST_H
