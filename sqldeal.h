#ifndef SQLDEAL_H
#define SQLDEAL_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QHostAddress>
#include <QThread>
#include <QtEndian>
#include <QDateTime>
#include <string>
#include <string.h>
#include <iostream>
#include <QTextCodec>
#include <QMessageBox>
#include <time.h>
#include <QVariant>
/*
#include <mutex>
using namespace std;

class sigclass{

public:
    static QSqlDatabase* GetInstance();
    sigclass();   //构造函数(被保护)
private:
    static mutex m_mutex; //锁
    static QSqlDatabase *sql;

};*/

class sqldeal : public QObject
{
    Q_OBJECT
public:
    explicit sqldeal(QObject *parent = 0);
signals:
    //tab2初始化绘制信号
    void painttable2(int,QString,QString,int,int,int,int,double,bool);
    //发送服务器连接数据
    void sendconnserver(QString,QString,int);
    //发送服务器重连数据
    void sendreconnect(QString,QString,int);
    //服务器列表更新
    void sendupdate_tab2(int,QString,QString,int,int,int,int,double,bool);
    //服务器tab1 条形图更新
    void connsignal(int,QString,QString,int,int,int);
    //通知发送
    void sendInfoToSock(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int,QString,QString);
    //通知定时发送
    void infoSendToThis(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //通知列表初始化发送
    void initInfoSendToThis(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //通知列表更新
    void updateInfoSendToThis(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int);
    //发送wglist
    void sendwglist(QString,QString,int);
    //删除tablewidget界面更新
    void sendDeleteToThis(int);
    //给sock发网关更新信息
    void sendWgUpdateToSock(QString,QString,bool,QString,int);
    //给网关发充值信息
    void sendToWgAddDays(int,QByteArray,QString);
public slots:
    //查询tab2成员
    void recvtab2_member();
    //服务器添加
    void addServer(QString,QString,int);
    //服务器连接
    void connserver();
    //服务器重连
    void reconnect(QString);
    //服务器列表更改
    void changeserver(QString,int,int,int,double);
    //服务器列表更新
    void updatetab2();
    //更新时间
    void changeTime(QString,int);
    //this通知处理
    void addInfo(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,QString,QString);
    //通知消息查询
    void infotimer();
    //通知列表初始化
    void dealInfoInit();
    //通知列表更新
    void dealInfoUpdate();
    //查看通知人员
    void dealgwidslist(QString);
    //删除服务器列表行
    void dealDeleteTablewidget(QString,int);
    //更改服务器状态
    void changeServerState(QString,bool);
    //发送id存数据库
    void SaveId(int,QString);
    //发送更新文件给网关
    void saveUpdateFile(QString,QString,bool,QString,int);
    //网关在线处理
    void gwOnline(QByteArray,short,QString);
    //网关充值处理
    void dealWgPay(int,QString,QString);
    //网关时间直接修改处理
    //void dealWgChange(int,QString);
    void test();

};

#endif // SQLDEAL_H
