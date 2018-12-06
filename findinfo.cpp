#include "findinfo.h"
#include "ui_findinfo.h"
#include <QScrollBar>
#include <QDebug>

findInfo::findInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::findInfo)
{
    ui->setupUi(this);
    setWindowTitle("通知消息列表");
    ui->tableWidget->setColumnCount(11);   //设置列数为8
    //设置不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0); //设置当前滑动条位置
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"编号"<<"标题"<<"内容"<<"link"<<"linkplace"<<"发送时间"<<"截止时间"<<"间隔时间"<<"发送数量"<<"发送对象"<<"发送类型");

}

findInfo::~findInfo()
{
    delete ui;
}

void findInfo::on_pushButton_clicked()
{
    emit findinfo_to_Tabwidget_signal();
}

void findInfo::on_Button_up_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value();

       if(nCurScroller>0)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(nCurScroller-pageValue);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(maxValue);
}

void findInfo::on_Button_down_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

       if(nCurScroller<maxValue)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue+nCurScroller);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}

void findInfo::initInfo(int userid,int num,bool radiobtn,QString timeend,QString title,QString content,QString link,QString linkplace,QString imgaddress,QString intervaltime,QString lasttime,QString wg_name,QString serverall,QString obj,QString type,QString timebegin,int gwids)
{
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行
    QString uid = QString::number(userid,10);
    QString uconnnum = QString::number(num,10);
    QString gwidss = QString::number(gwids,10);

    ui->tableWidget->setItem(RowCont,0,new QTableWidgetItem(gwidss));
    ui->tableWidget->setItem(RowCont,1,new QTableWidgetItem(title));
    ui->tableWidget->setItem(RowCont,2,new QTableWidgetItem(content));
    ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem(link));
    ui->tableWidget->setItem(RowCont,4,new QTableWidgetItem(linkplace));
    ui->tableWidget->setItem(RowCont,5,new QTableWidgetItem(timebegin));
    ui->tableWidget->setItem(RowCont,6,new QTableWidgetItem(timeend));
    ui->tableWidget->setItem(RowCont,7,new QTableWidgetItem(intervaltime));
    ui->tableWidget->setItem(RowCont,8,new QTableWidgetItem(uconnnum));
    ui->tableWidget->setItem(RowCont,9,new QTableWidgetItem(obj));
    ui->tableWidget->setItem(RowCont,10,new QTableWidgetItem(type));
}

void findInfo::updateInfo(int userid,int num,bool radiobtn,QString timeend,QString title,QString content,QString link,QString linkplace,QString imgaddress,QString intervaltime,QString lasttime,QString wg_name,QString serverall,QString obj,QString type,QString timebegin,int listnum,int gwids)
{
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    QString uconnnum = QString::number(num,10);
    QString gwidss = QString::number(gwids,10);
    if(listnum<RowCont)
    {
        ui->tableWidget->setItem(listnum,0,new QTableWidgetItem(gwidss));
        ui->tableWidget->setItem(listnum,1,new QTableWidgetItem(title));
        ui->tableWidget->setItem(listnum,2,new QTableWidgetItem(content));
        ui->tableWidget->setItem(listnum,3,new QTableWidgetItem(link));
        ui->tableWidget->setItem(listnum,4,new QTableWidgetItem(linkplace));
        ui->tableWidget->setItem(listnum,5,new QTableWidgetItem(timebegin));
        ui->tableWidget->setItem(listnum,6,new QTableWidgetItem(timeend));
        ui->tableWidget->setItem(listnum,7,new QTableWidgetItem(intervaltime));
        ui->tableWidget->setItem(listnum,8,new QTableWidgetItem(uconnnum));
        ui->tableWidget->setItem(listnum,9,new QTableWidgetItem(obj));
        ui->tableWidget->setItem(listnum,10,new QTableWidgetItem(type));
    }
    else if(RowCont<=listnum)
    {
        ui->tableWidget->insertRow(listnum);//增加一行
        ui->tableWidget->setItem(listnum,0,new QTableWidgetItem(gwidss));
        ui->tableWidget->setItem(listnum,1,new QTableWidgetItem(title));
        ui->tableWidget->setItem(listnum,2,new QTableWidgetItem(content));
        ui->tableWidget->setItem(listnum,3,new QTableWidgetItem(link));
        ui->tableWidget->setItem(listnum,4,new QTableWidgetItem(linkplace));
        ui->tableWidget->setItem(listnum,5,new QTableWidgetItem(timebegin));
        ui->tableWidget->setItem(listnum,6,new QTableWidgetItem(timeend));
        ui->tableWidget->setItem(listnum,7,new QTableWidgetItem(intervaltime));
        ui->tableWidget->setItem(listnum,8,new QTableWidgetItem(uconnnum));
        ui->tableWidget->setItem(listnum,9,new QTableWidgetItem(obj));
        ui->tableWidget->setItem(listnum,10,new QTableWidgetItem(type));
    }
}

void findInfo::on_Button_look_clicked()
{
    //发送信号给主界面调出列表界面，获取到鼠标点击行参数传过去
    QTableWidgetItem * itemr = ui->tableWidget->currentItem();
    if(itemr==Q_NULLPTR)return;
    int i = itemr->row();
    qDebug()<<"这是第"<<i<<"行"<<endl;
    QString serverName = ui->tableWidget->item(i, 0)->text();//获取某一格内容
    qDebug()<<serverName<<endl;
    emit sengwidslist(serverName);
}
