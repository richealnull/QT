#include "login.h"
#include "ui_login.h"

#include <windows.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QThread>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("登录");
    myT = new workthread;
    //创建子线程
    thread = new QThread(this);
    //把自定义的线程加入子线程
    myT->moveToThread(thread);

    //设置无右键菜单
    ui->passworldline->setContextMenuPolicy(Qt::NoContextMenu);
    //设置密码提示
    ui->passworldline->setPlaceholderText(QString("输入密码"));
    //设置密码隐藏
    ui->passworldline->setEchoMode(QLineEdit::Password);
    //设置最大长度16位
    ui->passworldline->setMaxLength(16);
    //发送登录验证给线程
    connect(this,&Login::startlogin,myT,&workthread::login);
    //线程处理登录验证
    connect(myT,&workthread::loginsignal,this,&Login::loginslote);
    //绑定密码框回车
    connect(ui->passworldline, SIGNAL(returnPressed()), ui->ButtonLogin, SIGNAL(clicked()), Qt::UniqueConnection);
    //超级管理员设置显示
    connect(&super,&supermanage::superManage_backSingle,
            [=]()
            {
                    this->show();
                    super.hide();
            }
    );
    //关闭界面关闭线程,关闭socket
    connect(this,&Login::destroyed,this,
            [=](){
                thread->quit();
                thread->wait();
            }
            );
     qDebug() <<"主线程号:"<< QThread::currentThread()<<endl;
     //线程启动
     thread->start();
}

Login::~Login()
{
    delete ui;
}
//登录验证
void Login::loginslote(QString user,QString pwd,int server,int info,int inquire,int update,QString time,int general_id)
{
    if(user == ui->userline->text() && pwd == ui->passworldline->text())
    {
        qDebug()<<"验证成功"<<endl;

        if(server == 0)
        {
            tab.removeTab(1);
            if(info == 0)
            {
                tab.removeTab(1);
                if(inquire == 0)
                {
                    tab.removeTab(1);
                    if(update == 0)
                    {
                        tab.removeTab(1);
                    }
                }
                else if(update == 0)
                {
                    tab.removeTab(2);
                }
            }
            else if(inquire == 0)
            {
                tab.removeTab(2);
                if(update == 0)
                {
                    tab.removeTab(2);
                }
            }
            else if(update == 0)
            {
                tab.removeTab(3);
            }
        }
        else if(info == 0)
        {
            tab.removeTab(2);
            if(inquire == 0)
            {
               tab.removeTab(2);
               if(update == 0)
               {
                   tab.removeTab(2);
               }
            }
            else if(update == 0)
            {
                tab.removeTab(3);
            }
        }
        else if(inquire == 0)
        {
            tab.removeTab(3);
            if(update == 0)
            {
                tab.removeTab(3);
            }
        }
        else if(update == 0)
        {
            tab.removeTab(4);
        }
        tab.show();
        qDebug()<<"tabwidget显示"<<endl;
        tab.setCurrentIndex(0);
        thread->quit();
        thread->wait();
        qDebug()<<"登录线程关闭"<<endl;
        this->hide();
        tab.time(time,general_id);

    }
    else{
        QMessageBox::about(NULL, "错误", "账号密码错误");
        qDebug()<<time<<endl;
    }
}


void Login::on_ButtonManage_clicked()
{
    ui->passworldline->clear();
    ui->userline->clear();
    super.show();
    this->hide();
}

void Login::on_ButtonLogin_clicked()
{
    //发送信号，线程获取数据库数据 验证登录
    emit startlogin(ui->userline->text(),ui->passworldline->text());
}
