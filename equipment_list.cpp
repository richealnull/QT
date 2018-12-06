#include "equipment_list.h"
#include "ui_equipment_list.h"
#include <QDebug>

equipment_list::equipment_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::equipment_list)
{
    ui->setupUi(this);
    setWindowTitle("设备数量");
    ui->tableWidget->setColumnCount(2);
    //设置不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"设备名称"<<"设备数量");
    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertRow(1);
    ui->tableWidget->insertRow(2);
    ui->tableWidget->insertRow(3);
    ui->tableWidget->insertRow(4);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("网关数量"));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("开关数量"));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("插座数量"));
    ui->tableWidget->setItem(3,0,new QTableWidgetItem("窗帘数量"));
    ui->tableWidget->setItem(4,0,new QTableWidgetItem("红外数量"));
}

equipment_list::~equipment_list()
{
    delete ui;
}

void equipment_list::on_pushButton_2_clicked()
{
    emit backToWidget();
    emit clearmap();
}

void equipment_list::on_pushButton_clicked()
{
    QTableWidgetItem * itemr = ui->tableWidget->currentItem();
    if(itemr==Q_NULLPTR)return;
    int i = itemr->row();
    qDebug()<<"这是第"<<i<<"行"<<endl;
    emit senddeviceinfo(i);
}

void equipment_list::deviceNumSet(int wg_num,int light_num,int socket_num,int curtain_num,int infrared_num)
{
    QString wg = QString::number(wg_num, 10);
    QString light = QString::number(light_num, 10);
    QString socket = QString::number(socket_num, 10);
    QString curtain = QString::number(curtain_num, 10);
    QString infrared = QString::number(infrared_num, 10);
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(wg));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(light));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(socket));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem(curtain));
    ui->tableWidget->setItem(4,1,new QTableWidgetItem(infrared));
}
