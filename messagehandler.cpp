#include "messagehandler.h"

void MessageHandler::process(QTcpSocket* client, const QString message)
{
    HttpRequest request(message); //Parsing a message into the HttpRequest

    if(request.hasRoute("api"))
    {
        return processApi(client, request);
    }
    else if(request.hasRoute("test"))
    {
        HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Html, getTableBody()); //Send a table to the browser
    }
    else
    {
        HttpResponse::sendResponse(client, HttpResponse::BadRequest); //Bad request 'cause cannot find a route
    }
}

void MessageHandler::processApi(QTcpSocket* client, HttpRequest& request)
{
    QString routeValue = request.getRouteValue("api");
    HttpRequest::RequestType type = request.getRequestType();
    if(type == HttpRequest::Post) //A condition for the POST request
    {
        int id = addNewValueInTable(request.getBody());
        writeInJournal(type, id);
        return HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Json, QString::number(id));
    }
    bool ok = false;
    int intRouteValue = routeValue.toInt(&ok); //Converting a value from the request to Integer
    if(!ok || !table.contains(intRouteValue))
    {
        return HttpResponse::sendResponse(client, HttpResponse::BadRequest);
    }
    writeInJournal(type, intRouteValue);
    switch(type)
    {
        case HttpRequest::Get:
            return HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Json, table[intRouteValue]); //Sending a value of some ID from the table
            break;
        case HttpRequest::Put:
            table[intRouteValue] = request.getBody(); //Setting a value from the request to ID
            return HttpResponse::sendResponse(client, HttpResponse::Ok);
            break;
        case HttpRequest::Delete:
            table.remove(intRouteValue); //Remove an ID from the table
            return HttpResponse::sendResponse(client, HttpResponse::Ok);
            break;
    }
}

void MessageHandler::writeInJournal(HttpRequest::RequestType type, int id)
{
    QString pattern = QString("Item with id %1 has been ").arg(id);
    switch (type)
    {
        case HttpRequest::Post:
            journal.push_back(pattern + "added");
            break;
        case HttpRequest::Put:
            journal.push_back(pattern + "changed");
            break;
        case HttpRequest::Delete:
            journal.push_back(pattern + "deleted");
            break;
        default:
            break;
    }
}

//Making a table HTML code
QString MessageHandler::getTableBody()
{
    std::stringstream ss;
    ss << "<html><body><table border=\"1\" style=\"border-collapse: collapse; border: 1px solid black;\"><tr><th>ID</th><th>Value</th></tr>";
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
