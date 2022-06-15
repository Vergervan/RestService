#include "headers/widget.h"
#include "ui_widget.h"
#include <QCryptographicHash>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setStyle(QApplication::style());
    server = new RestfulServer(this);
    refreshServerStatus();
}

Widget::~Widget()
{
    delete ui;
    delete server;
}

void Widget::on_turnServerButton_clicked()
{
    if(isWorking)
    {
        server->close();
        db.close();
        QSqlDatabase::removeDatabase("server");
        isWorking = false;
    }
    else
    {
        isWorking = server->listen(QHostAddress("127.0.0.1"), 1234);
        if(!isWorking)
        {
            QMessageBox::warning(this, "Error", "Cannot start a server");
        }

        this->db = QSqlDatabase::addDatabase("QSQLITE", "server");
        db.setDatabaseName("api_history.db");
        if(!db.open())
        {
            QMessageBox::warning(this, "Error", "Cannot open a database");
            server->close();
            isWorking = false;
        }
        server->setDatabase(db);
        QSqlQuery query(db);
        query.exec("CREATE TABLE IF NOT EXISTS data_values (id INTEGER PRIMARY KEY, value TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS operations (id INTEGER PRIMARY KEY, time TEXT DEFAULT CURRENT_TIMESTAMP, operationText TEXT)");
    }
    refreshServerStatus();
    refreshTableData();
}

void Widget::refreshServerStatus()
{
    if(isWorking)
    {
        ui->stateLabel->setProperty("state", "on");
        ui->stateLabel->setText("On");
        ui->turnServerButton->setText("Stop");
    }
    else
    {
        ui->stateLabel->setProperty("state", "off");
        ui->stateLabel->setText("Off");
        ui->turnServerButton->setText("Start");
    }
    ui->stateLabel->style()->unpolish(ui->stateLabel);
    ui->stateLabel->style()->polish(ui->stateLabel);
}

void Widget::refreshTableData()
{
    ui->dataTable->clear();
    ui->dataTable->setRowCount(0);
    QSqlRecord record = db.record("data_values");
    QStringList labels;
    for(int i = 0; i < record.count(); ++i)
    {
        labels.push_back(record.fieldName(i));
    }
    ui->dataTable->setColumnCount(record.count());
    ui->dataTable->setHorizontalHeaderLabels(labels);
    QSqlQuery query(db);
    query.exec("SELECT * FROM data_values");
    int counter = 0;
    while(query.next())
    {
        ui->dataTable->insertRow(counter);
        QTableWidgetItem* idItem = new QTableWidgetItem(query.value(0).toString());
        QTableWidgetItem* valueItem = new QTableWidgetItem(query.value(1).toString());
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        valueItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->dataTable->setItem(counter, 0, idItem);
        ui->dataTable->setItem(counter, 1, valueItem);
        counter++;
    }
}



