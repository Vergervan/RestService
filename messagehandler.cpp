#include "messagehandler.h"

void MessageHandler::process(QTcpSocket* client, const QString message)
{
    HttpRequest request(message);

    if(request.hasRoute("api"))
    {
        return processApi(client, request);
    }
    else if(request.hasRoute("test"))
    {
        HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Html, getTableBody());
    }
    else
    {
        HttpResponse::sendResponse(client, HttpResponse::BadRequest);
    }
}

void MessageHandler::processApi(QTcpSocket* client, HttpRequest& request)
{
    QString routeValue = request.getRouteValue("api");
    HttpRequest::RequestType type = request.getRequestType();
    if(type == HttpRequest::Post)
    {
        int id = addNewValueInTable(request.getBody());
        writeInJournal(type, id);
        return HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Json, QString::number(id));
    }
    bool ok = false;
    int intRouteValue = routeValue.toInt(&ok);
    if(!ok || !table.contains(intRouteValue))
    {
        return HttpResponse::sendResponse(client, HttpResponse::BadRequest);
    }
    writeInJournal(type, intRouteValue);
    qDebug() << "Processing a request";
    switch(type)
    {
        case HttpRequest::Get:
            return HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Json, table[intRouteValue]);
            break;
        case HttpRequest::Put:
            table[intRouteValue] = request.getBody();
            return HttpResponse::sendResponse(client, HttpResponse::Ok);
            break;
        case HttpRequest::Delete:
            table.remove(intRouteValue);
            return HttpResponse::sendResponse(client, HttpResponse::Ok);
            break;
    }
    qDebug() << "Stop processing";
}

void MessageHandler::writeInJournal(HttpRequest::RequestType type, int id)
{
    QString pattern = QString("Item with id %1 has been ").arg(id);
    switch (type)
    {
        case HttpRequest::Post:
            journal.emplace_back(pattern + "added");
            break;
        case HttpRequest::Put:
            journal.emplace_back(pattern + "changed");
            break;
        case HttpRequest::Delete:
            journal.emplace_back(pattern + "deleted");
            break;
        default:
            break;
    }
}

QString MessageHandler::getTableBody()
{
    std::stringstream ss;
    ss << "<html><body><table><tr><th>ID</th><th>Value</th></tr>";
    for(auto it = table.begin(); it != table.end(); it++)
    {
        ss << QString("<tr><td>%1</td><td>%2</td></tr>").arg(QString::number(it.key()), it.value()).toStdString();
    }
    ss << "</table><br>";
    foreach (auto record, journal)
    {
        ss << record.toStdString() << "<br>";
    }
    ss << "</body></html>";
    return QString(ss.str().c_str());
}

int MessageHandler::addNewValueInTable(QString value)
{
    int pos = 0;
    while(table.contains(pos))
    {
        ++pos;
    }
    table.insert(pos, value);
    return pos;
}
