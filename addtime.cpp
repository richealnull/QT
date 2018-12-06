#include "addtime.h"
#include "ui_addtime.h"

addtime::addtime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addtime)
{
    ui->setupUi(this);
    ui->lineEdit_2->setValidator(new QIntValidator(0, 24*365, this));
     setWindowTitle("设置时间");
}


addtime::~addtime()
{
    delete ui;
}

void addtime::on_pushButton_2_clicked()
{
    emit timeback_super();
}

void addtime::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "用户名不能为空");
    }
    else if(ui->timeEdit->time().isNull()&&ui->lineEdit_2->text().isEmpty()){
         QMessageBox::about(NULL, "错误", "时间不能为空");
    }
    else{
        QSqlDatabase d = QSqlDatabase::addDatabase("QMYSQL");
        d.setHostName("127.0.0.1");
        d.setDatabaseName("test");
        d.setPort(3306);
        d.setUserName("root");
        d.setPassword("huge123");
        bool ret = d.open();
        QString time;
        QString time1 = ui->timeEdit->time().toString("hh:mm:ss");
        QString time2 = ui->lineEdit_2->text();
        time2 +=":00:00";
        if(ui->lineEdit_2->text().isEmpty()){
            time = time1;
        }
        else{
            time = time2;
        }
        if(ret){
            QSqlQuery query;
            QString temp = QString::fromUtf8("select count(user) from general_user where user = '%1'").arg(ui->lineEdit->text());
            int isoka;
            isoka = query.exec(temp);
            if(isoka>0){
                //增加时间
                QString temp2 = QString::fromUtf8("update general_user set permis_time = '%1' where user ='%2'").arg(time).arg(ui->lineEdit->text());
                bool isok;
                isok = query.exec(temp2);
                if(isok){
                    QMessageBox::about(NULL, "成功", "时间更改成功");
                    this->hide();
                    emit timeback_super();
                }
                else{
                    QMessageBox::about(NULL, "失败", "时间更改失败");
                }
            }
            else{
                QMessageBox::about(NULL, "警告", "用户不存在");
            }
}
        d.close();
}
}
