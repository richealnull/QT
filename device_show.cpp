#include "device_show.h"
#include "ui_device_show.h"
#include <QScrollBar>
#include <QDebug>

device_show::device_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::device_show)
{
    ui->setupUi(this);
    qRegisterMetaType<QMap<QString,unsigned char*>>("QMap<QString,unsigned char*>");

    setWindowTitle("设备详情");
    ui->tableWidget->setColumnCount(3);   //设置列数
    //设置不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0); //设置当前滑动条位置
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"设备名称"<<"软件版本"<<"硬件版本");
}

device_show::~device_show()
{
    delete ui;
}

void device_show::on_pushButton_2_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value();

       if(nCurScroller>0)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(nCurScroller-pageValue);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(maxValue);
}

void device_show::on_pushButton_3_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

       if(nCurScroller<maxValue)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue+nCurScroller);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}

void device_show::on_pushButton_clicked()
{
    this->hide();
}
void device_show::deviceSet(QMap<QString,unsigned char*> list)
{
    qDebug()<<"设置device"<<list.size()<<endl;
    int rows = list.size();
    int addrow = 0;
    QMap<QString,unsigned char*>::iterator it = list.begin();
    for(int i =0;i<rows;i++)
    {
        if(addrow < ui->tableWidget->rowCount() && it != list.end())
        {
            QString name = it.key();
            unsigned char* value = it.value();
            unsigned char Software_version = (*(unsigned char*)(value));
            unsigned char Hardware_version = (*(unsigned char*)(value+1));
            ui->tableWidget->setItem(addrow,0,new QTableWidgetItem(name));
            ui->tableWidget->setItem(addrow,1,new QTableWidgetItem(QString(Software_version)));
            ui->tableWidget->setItem(addrow,2,new QTableWidgetItem(QString(Hardware_version)));
            addrow++;
            it++;
        }
        else if(ui->tableWidget->rowCount() <= addrow && it != list.end())
        {
            QString name = it.key();
            unsigned char* value = it.value();
            unsigned char Software_version = (*(unsigned char*)(value));
            unsigned char Hardware_version = (*(unsigned char*)(value+1));
            ui->tableWidget->insertRow(addrow);//增加一行
            ui->tableWidget->setItem(addrow,0,new QTableWidgetItem(name));
            ui->tableWidget->setItem(addrow,1,new QTableWidgetItem(QString(Software_version)));
            ui->tableWidget->setItem(addrow,2,new QTableWidgetItem(QString(Hardware_version)));
            addrow++;
            it++;
        }
    }
}
