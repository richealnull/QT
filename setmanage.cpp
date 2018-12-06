#include "setmanage.h"
#include "ui_setmanage.h"
#include <QSqlDatabase>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>

setManage::setManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setManage)
{
    ui->setupUi(this);
    setWindowTitle("添加管理员");

    //设置无右键菜单
    ui->line_pwd->setContextMenuPolicy(Qt::NoContextMenu);
    //设置密码提示
    ui->line_pwd->setPlaceholderText(QString("输入密码"));
    //设置密码隐藏
    ui->line_pwd->setEchoMode(QLineEdit::Password);
    //设置最大长度16位
    ui->line_pwd->setMaxLength(16);
}

setManage::~setManage()
{
    delete ui;
}

void setManage::on_pushButton_clicked()
{
    emit setManage_backSingle();
}

void setManage::on_pushButton_2_clicked()
{
    if(ui->line_user->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "账号不能为空");
    }
    else if(ui->line_pwd->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "密码不能为空");
    }
    else if(ui->timeEdit->text() =="0:00"){
        QMessageBox::about(NULL, "错误", "时间不能为空");
    }
    else{
    QSqlDatabase d;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d=QSqlDatabase::database("qt_sql_default_connection");
    else
    d= QSqlDatabase::addDatabase("QMYSQL");

    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int a = 0,b = 0,c = 0,e = 0;
    if(ui->check_server->isChecked()){a=1;}
    if(ui->check_inquire->isChecked()){b=1;}
    if(ui->check_info->isChecked()){c=1;}
    if(ui->check_update->isChecked()){e=1;}

    QString user = ui->line_user->text();
    QString pwd = ui->line_pwd->text();
    QString time = ui->timeEdit->time().toString("hh:mm:ss");
    qDebug()<<a<<b<<c<<e<<user<<pwd<<time<<endl;

    if(ret)
    {
    QSqlQuery query;
    QString temp = QString::fromUtf8("select count(user) from general_user where user = '%1'").arg(user);
    int isoka;
    isoka = query.exec(temp);
    if(isoka>0){
    QMessageBox::about(NULL, "错误", "该用户已存在");
    }
    else{
        QString temp1 = QObject::tr("INSERT INTO general_user (user, server_permis,inform_permis,inquire_permis,update_permis,permis_time,passwor)  VALUES('%1','%2','%3','%4','%5','%6','%7')").arg(user).arg(a).arg(c).arg(b).arg(e).arg(time).arg(pwd);
        bool isok;
        isok = query.exec(temp1);
        if(isok){
        qDebug()<<"成功了"<<endl;
        qDebug()<<QSqlDatabase::drivers()<<endl;
        emit toLogin_backSingle();
        }
        else{qDebug()<<"插入失败"<<endl;
             qDebug()<<query.lastError()<<endl;
        }
    }
    }
    else{
        qDebug()<<"失败"<<endl;
        qDebug()<<QSqlDatabase::drivers()<<endl;
        QMessageBox::about(NULL, "错误", "数据库连接失败");
    }
    d.close();
    }
}
