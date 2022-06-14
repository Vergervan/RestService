#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QApplication>
#include <QStyle>
#include <QtSql>
#include "headers/restfulserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void refreshServerStatus();
    ~Widget();
public slots:
    void refreshTableData();
private slots:
    void on_turnServerButton_clicked();

private:
    RestfulServer* server;
    QSqlDatabase db;
    bool isWorking = false;
    Ui::Widget *ui;
};
#endif // WIDGET_H
