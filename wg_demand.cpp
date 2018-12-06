#include "wg_demand.h"
#include "ui_wg_demand.h"
#include <QDebug>
#include <QScrollBar>

wg_demand::wg_demand(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wg_demand)
{
    ui->setupUi(this);
    setWindowTitle("网关管理");
    ui->Button_adduser->setEnabled(false);
    ui->Button_chatrecord->setEnabled(false);
    ui->Button_equipment->setEnabled(false);
    ui->Button_gateway->setEnabled(false);
    ui->Button_gatewaydate->setEnabled(false);
    ui->Button_handlelog->setEnabled(false);
}

wg_demand::~wg_demand()
{
    delete ui;
}

void wg_demand::on_Button_back_clicked()
{
    emit findwg_to_Tabwidget_signal();
    this->hide();
}

void wg_demand::dealWgData(QString id)
{
    qDebug()<<"权限开始"<<endl;
    gwid = id;
    ui->Button_adduser->setEnabled(true);
    ui->Button_chatrecord->setEnabled(true);
    ui->Button_equipment->setEnabled(true);
    ui->Button_gateway->setEnabled(true);
    ui->Button_gatewaydate->setEnabled(true);
    ui->Button_handlelog->setEnabled(true);
}

void wg_demand::on_Button_equipment_clicked()
{
    emit equiListShow();
}

void wg_demand::on_Button_recommended_clicked()
{
   emit setrefeShow();
}

void wg_demand::showself()
{
    this->show();
}

void wg_demand::on_Button_gatewaydate_clicked()
{
    this->hide();
    emit sendgwidtochange(gwid);
    emit changeTimeShow();
}

/*void wg_demand::on_pushButton_clicked()
{
    this->hide();
    emit sendgwidtoset(gwid);
    emit setstoptimeshow();

}*/
