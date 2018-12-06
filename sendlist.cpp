#include "sendlist.h"
#include "ui_sendlist.h"
#include <QScrollBar>
#include <QDebug>

sendlist::sendlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sendlist)
{
    ui->setupUi(this);
    setWindowTitle("发送列表");
    ui->tableWidget->setColumnCount(2);   //设置列数
    //设置不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0); //设置当前滑动条位置
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"编号"<<"id");
}

sendlist::~sendlist()
{
    delete ui;
}

void sendlist::on_pushButton_2_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value();

       if(nCurScroller>0)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(nCurScroller-pageValue);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(maxValue);
}

void sendlist::on_pushButton_3_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

       if(nCurScroller<maxValue)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue+nCurScroller);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}

void sendlist::showlist(QString gwids, QString wgname, int listnum)
{
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    qDebug()<<RowCont<<listnum<<endl;

    if(listnum<RowCont)
    {
        ui->tableWidget->setItem(listnum,0,new QTableWidgetItem(gwids));
        ui->tableWidget->setItem(listnum,1,new QTableWidgetItem(wgname));
    }
    else if(RowCont<=listnum)
    {
        ui->tableWidget->insertRow(listnum);//增加一行
        ui->tableWidget->setItem(listnum,0,new QTableWidgetItem(gwids));
        ui->tableWidget->setItem(listnum,1,new QTableWidgetItem(wgname));
    }
}

void sendlist::on_pushButton_clicked()
{
    this->hide();
}
void sendlist::dealclear()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
}
