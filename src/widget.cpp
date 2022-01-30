#include "headers/widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setStyle(QApplication::style());
    server = new RestfulServer(this);
    ui->dataTable->setColumnCount(2);
    ui->dataTable->setHorizontalHeaderLabels({"ID", "Value"});
    refreshServerStatus();
}

Widget::~Widget()
{
    delete ui;
    delete server;
}

void Widget::on_turnServerButton_clicked()
{
    if(isWorking){
        server->close();
        isWorking = false;
    }else{
        if(!server->listen(QHostAddress("127.0.0.1"), 1234)){
            QMessageBox::warning(this, "Error", "Cannot start a server");
            isWorking = false;
        }else{
            isWorking = true;
        }
    }
    refreshServerStatus();
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
    auto table = server->getTable();
    ui->dataTable->setRowCount(table.size());
    int counter = 0;
    for(auto it = table.begin(); it != table.end(); ++it, ++counter)
    {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(it.key()));
        QTableWidgetItem* valueItem = new QTableWidgetItem(it.value());
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        valueItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->dataTable->setItem(counter, 0, idItem);
        ui->dataTable->setItem(counter, 1, valueItem);
    }
}



