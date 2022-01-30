#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new RestfulServer(this);
}

Widget::~Widget()
{
    delete ui;
    delete server;
}

void Widget::on_turnServerButton_clicked()
{
    if(!server->listen(QHostAddress("127.0.0.1"), 1234)){
        qDebug("Unable to start");
    }else{
        qDebug("Successfully listening");
    }
}

