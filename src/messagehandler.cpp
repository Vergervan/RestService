#include "headers/messagehandler.h"

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
    QSqlQuery query(db);

    QString routeValue = request.getRouteValue("api");
    HttpRequest::RequestType type = request.getRequestType();
    if(type == HttpRequest::Post) //A condition for the POST request
    {
        int id = addNewValueInTable(request.getBody());
        if(id == -1)
        {
            return HttpResponse::sendResponse(client, HttpResponse::BadRequest);
        }
        writeInJournal(type, id);
        return HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Json, QString::number(id));
    }
    bool ok = false;
    int intRouteValue = routeValue.toInt(&ok); //Converting a value from the request to Integer
    query.prepare("SELECT value FROM data_values WHERE id = ?");
    query.addBindValue(intRouteValue);
    if(!ok || !query.exec() || !query.first())
    {
        return HttpResponse::sendResponse(client, HttpResponse::BadRequest);
    }
    writeInJournal(type, intRouteValue);
    switch(type)
    {
        case HttpRequest::Get:
            return HttpResponse::sendResponse(client, HttpResponse::Ok, HttpResponse::Json, query.value(0).toString()); //Sending a value of some ID from the table

        case HttpRequest::Put:
            query.prepare("UPDATE data_values SET value = ? WHERE id = ?");
            query.addBindValue(request.getBody());
            query.addBindValue(intRouteValue);
            if(query.exec())
                return HttpResponse::sendResponse(client, HttpResponse::Ok);

        case HttpRequest::Delete:
            query.prepare("DELETE FROM data_values WHERE id = ?"); //Remove an ID from the table
            query.addBindValue(intRouteValue);
            if(query.exec())
                return HttpResponse::sendResponse(client, HttpResponse::Ok);
    }
}

void MessageHandler::writeInJournal(HttpRequest::RequestType type, int id)
{
    QString pattern = QString("Item with id %1 has been ").arg(id);
    switch (type)
    {
        case HttpRequest::Post:
            pattern.append("added");
            break;
        case HttpRequest::Put:
            pattern.append("changed");
            break;
        case HttpRequest::Delete:
            pattern.append("deleted");
            break;
    }
    if(type != HttpRequest::Get)
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO operations(operationText) VALUES (?)");
        query.addBindValue(pattern);
        query.exec();
    }
}
//Making a table HTML code
QString MessageHandler::getTableBody()
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM data_values");
    std::stringstream ss;
    ss << "<html><body><table border=\"1\" style=\"border-collapse: collapse; border: 1px solid black;\"><tr><th>ID</th><th>Value</th></tr>";
    while(query.next())
    {
        ss << QString("<tr><td>%1</td><td>%2</td></tr>").arg(query.value(0).toString(), query.value(1).toString()).toStdString();
    }
    ss << "</table><br>Operations journal:<br>";
    query.exec("SELECT time, operationText FROM operations");
    while(query.next())
    {
        ss << query.value(0).toString().toStdString() << " - " << query.value(1).toString().toStdString() << "<br>";
    }
    ss << "</body></html>";
    return QString(ss.str().c_str());
}

int MessageHandler::addNewValueInTable(QString value)
{
    QSqlQuery query(this->db);
    query.prepare("INSERT INTO data_values VALUES (null, ?)");
    query.addBindValue(value);
    if(!query.exec()){
        query.lastError().databaseText();
        return -1;
    }
    return query.lastInsertId().toInt();
}
