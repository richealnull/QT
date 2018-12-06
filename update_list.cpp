#include "update_list.h"
#include "ui_update_list.h"
#include <QScrollBar>
#include <QDebug>

update_list::update_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::update_list)
{
    ui->setupUi(this);
    setWindowTitle("发送列表");
    ui->tableWidget->setColumnCount(4);   //设置列数
    //设置不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0); //设置当前滑动条位置
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"userid"<<"升级版本"<<"发送人数"<<"升级id");
}

update_list::~update_list()
{
    delete ui;
}

void update_list::on_pushButton_2_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value();

       if(nCurScroller>0)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(nCurScroller-pageValue);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(maxValue);
}

void update_list::on_pushButton_3_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

       if(nCurScroller<maxValue)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue+nCurScroller);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}

void update_list::on_pushButton_clicked()
{
    emit backToWgUpdata();
    this->hide();
}
void update_list::showUpList()
{
    this->show();
}

void update_list::on_pushButton_4_clicked()
{
    emit showUpWgList();
}
