#include "wg_updata.h"
#include "ui_wg_updata.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

wg_updata::wg_updata(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wg_updata)
{
    ui->setupUi(this);
    setWindowTitle("更新管理");
    //初始化tab3 comboBox
     ui->comboBox_servers->insertItem(0,"服务器列表");
     ui->comboBox_servers->setEditable(false); //设置不可编辑
     ui->comboBox_servers->setEditable(false);
     ui->comboBox_servers->setEditable(false);
}

wg_updata::~wg_updata()
{
    delete ui;
}

void wg_updata::on_Button_back_clicked()
{
    emit wgupdata_to_Tabwidget_signal();
}

void wg_updata::on_Button_switch_bag_clicked()
{
    /*QString file_full;
    QFileInfo fi;

    file_full = QFileDialog::getOpenFileName(this);

    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    ui->label_filename->setText(file_name);*/
}
void wg_updata::init_combox(QString name)
{
    qDebug()<<"combox设置"<<ui->comboBox_servers->count()+1<<endl;
    qDebug()<<name<<endl;
    for(int i=0;i<ui->comboBox_servers->count();i++)
    {
        if (name ==ui->comboBox_servers->itemText(i))
        {
            qDebug()<<"combox删除"<<endl;
                ui->comboBox_servers->removeItem(i);
        }
    }
    ui->comboBox_servers->insertItem(ui->comboBox_servers->count()+1,name);
}

void wg_updata::on_Button_find_clicked()
{
    emit showUpList();
    this->hide();
}
void wg_updata::showWgUpdata()
{
    this->show();
}

void wg_updata::on_Button_updata_clicked()
{
    if(ui->radioButton->isChecked()==false&&ui->line_wg->text()==NULL&&ui->comboBox_servers->currentText()=="服务器列表"){
         QMessageBox::about(NULL, "warring", "请选择一个发送对象");
    }
    //if(file_name == NULL || file_path == NULL){
      //   QMessageBox::about(NULL, "warring", "请选择发送文件");
   // }
    if(ui->comboBox_servers->count()<2){
         QMessageBox::about(NULL, "warring", "没有服务器在线");
    }
    else{
        emit sendFileToSql(ui->line_wg->text(),ui->comboBox_servers->currentText(),ui->radioButton->isChecked(),ui->line_address->text());
    }
    qDebug()<<ui->comboBox_servers->currentText()<<endl;
    qDebug()<<ui->comboBox_servers->count()<<endl;
    qDebug()<<ui->radioButton->isChecked()<<endl;
    qDebug()<<ui->label_wg->text()<<endl;
    //qDebug()<<file_name<<endl;
    //qDebug()<<file_path<<endl;
}
