#include "supermanage.h"
#include "ui_supermanage.h"
#include <QSqlDatabase>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QSqlQuery>

using namespace std;
supermanage::supermanage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::supermanage)
{
    ui->setupUi(this);
    setWindowTitle("超级管理员");
    //设置无右键菜单
    ui->pwd_line->setContextMenuPolicy(Qt::NoContextMenu);
    //设置密码提示
    ui->pwd_line->setPlaceholderText(QString("输入密码"));
    //设置密码隐藏
    ui->pwd_line->setEchoMode(QLineEdit::Password);

    connect(&set_ui,&setManage::setManage_backSingle,
            [=]()
            {
                    this->show();
                    set_ui.hide();
            }
    );

    connect(&set_ui,&setManage::toLogin_backSingle,
            [=]()
            {
                    set_ui.hide();
                    emit superManage_backSingle();
            }
    );

    connect(&set_time,&addtime::timeback_super,
            [=]()
            {
                    this->show();
                    set_time.hide();
            }
    );
}

supermanage::~supermanage()
{
    delete ui;
}

void supermanage::on_Button_back_clicked()
{
    emit superManage_backSingle();
}

void supermanage::on_Button_ok_clicked()
{
    if(ui->pwd_line->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "密码不能为空");
    }
    else{
        QSqlDatabase d = QSqlDatabase::addDatabase("QMYSQL");
        d.setHostName("127.0.0.1");
        d.setDatabaseName("test");
        d.setPort(3306);
        d.setUserName("root");
        d.setPassword("huge123");
        QString pwd;
        bool ret = d.open();
        if(ret){
            qDebug()<<"Hi mysql!连接"<<endl;
            QSqlQuery query("select * from supermaster");
            while(query.next())
            {
                pwd = query.value(0).toString();
            }
        }
        else{
                qDebug()<<"失败"<<endl;
                qDebug()<<QSqlDatabase::drivers()<<endl;
                QMessageBox::about(NULL, "错误", "数据库连接失败");
            }

    if(pwd != ui->pwd_line->text())
    {

        QMessageBox::about(NULL, "错误", "密码不正确");
    }
    else{
        qDebug()<<"验证成功"<<endl;
        ui->pwd_line->clear();
        this->hide();
        set_ui.show();
    }
    d.close();
}
}

void supermanage::on_pushButton_clicked()
{
    if(ui->pwd_line->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "密码不能为空");
    }
    else{
        QSqlDatabase d = QSqlDatabase::addDatabase("QMYSQL");
        d.setHostName("127.0.0.1");
        d.setDatabaseName("test");
        d.setPort(3306);
        d.setUserName("root");
        d.setPassword("huge123");
        QString pwd;
        bool ret = d.open();
        if(ret){
            qDebug()<<"Hi mysql!连接"<<endl;
            QSqlQuery query("select * from supermaster");
            while(query.next())
            {
                pwd = query.value(0).toString();
            }
        }
        else{
                qDebug()<<"失败"<<endl;
                qDebug()<<QSqlDatabase::drivers()<<endl;
                QMessageBox::about(NULL, "错误", "数据库连接失败");
            }

    if(pwd != ui->pwd_line->text())
    {

        QMessageBox::about(NULL, "错误", "密码不正确");
    }
    else{
        qDebug()<<"验证成功"<<endl;
        ui->pwd_line->clear();
        this->hide();
        set_time.show();
    }
    d.close();
}
}
