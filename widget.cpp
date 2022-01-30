#include "widget.h"
#include "ui_widget.h"

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



