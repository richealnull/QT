#include "addserver.h"
#include "ui_addserver.h"

addServer::addServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addServer)
{
    ui->setupUi(this);
}

addServer::~addServer()
{
    delete ui;
}

void addServer::on_Buttonconn_clicked()
{
    emit server_to_Tabwidget_signal();
}
