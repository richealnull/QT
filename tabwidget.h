#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QList>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QChartView>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCore>
#include <QStandardItemModel>
#include <QString>
#include <QtCharts>
#include <QtCharts/QBarSet>
#include <QThread>
#include <QTimer>
#include <QValidator>

#include "tab3_widget.h"
#include "findinfo.h"
#include "wg_demand.h"
#include "changepwd.h"
#include "quit.h"
#include "wg_updata.h"
//#include "widgetthread.h"
#include "widgetsocket.h"
#include "sqldeal.h"
#include "sendlist.h"
#include "equipment_list.h"
#include "device_show.h"
#include "setreferrer.h"
#include "addreferees.h"
#include "changetime.h"
#include "update_list.h"
#include "update_wglist.h"
#include "parse.h"

using namespace QtCharts;
namespace Ui {
class TabWidget;
}

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = 0);
    void time(QString time,int general_id);

    int user_id;
    int adminTime;      //管理员剩余时间

    ~TabWidget();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_Button_gw_updata_clicked();

    void on_Button_robot_updata_clicked();

    void on_Button_count_updata_clicked();

    void on_pushButton_7_clicked();

    void on_Button_up_clicked();

    void on_Button_down_clicked();

    void on_pushButton_delete_clicked();

public slots:
    //连接初始化
    void threadconn(int userid,QString ip,QString username,int port,int app_count,int wg_count);
    //列表初始化
    void painttab2(int,QString,QString,int,int,int,int,double,bool);
    //列表更新
    void updatetab2(int,QString,QString,int,int,int,int,double,bool);
    void on_Button_restart_clicked();
    //添加服务器
    void dealSendAddToSql(QString,QString,int);
    //处理sql来的服务器连接数据
    void dealSendConnServer(QString,QString,int);
    //处理重连  来自sock
    void dealSockReconnect(QString);
    //处理重连  来自sql
    void dealSqlReconnect(QString,QString,int);
    //处理服务器更行  来自sock
    void dealSockChangeServer(QString,int,int,int,double);
    //处理combox 初始化
    void dealInitcombox(QString);

    //处理tab列表更新 来自time线程
    //void dealtab2_update_to_sql(int,int);
    //处理时间更新
    //void dealchangeTime(QString,int);
    //处理通知消息发送
    void dealsendbufFromTab3(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);


    //处理添加服务器失败
    void dealAddServerFail();
    //处理通知消息定时发送
    void dealInfoFromSql(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //通知消息初始化
    void dealInitInfoFromSql(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //通知发送
    void dealInfoToSock(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int,QString,QString);
    //通知列表更新
    void dealUpdateToInfo(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int);
    //查看gwids列表信号处理
    void dealgwidslist(QString);
    //gwids发送给gwids列表
    void dealgwidsFromSql(QString,QString,int);
    //设备列表显示
    void equiListShow();
    //设备列表返回查询网关
    void equiListToSelectWg();
    //详细设备列表显示
    void dealdeviceFromEqui(int);
    //推荐人页面显示
    void dealSetRefeShow();
    //更新网关列表
    void showUpWgList();
    //删除tablewidget更新
    void dealDealTableWidget(int);
    //界面时间更新
    void timeone();
    //服务器列表刷新
    void timeten();
    //网关查询
    void timerFive();
    //传给数据库网关更新文件消息
    void sendToSqlWgUpdate(QString,QString,bool,QString);
    //进度条生成
    void dealProgressBar(int);
    //进度条改变
    void dealProgressBarChange(int);
    //停止网关查询定时器
    void stopTimeFive();



signals:
    //void starttime(QString time,int general_id); //启动定时器信号
    void startsock(); //启动套接字线程信号
    void sendpaint_tab2(); //初始化绘制table2列表
    void sendupdate_tab2(); //更新table2
    void sendrestart(QString);  //重启服务器
    void senAddServer(QString,QString,int); //增加服务器
    void sendclosesock(); //断开服务器连接
    void sendAddToSql(QString,QString,int);//传给sql 处理添加
    void sendConnToSock(QString,QString,int); //传给sock 处理连接
    void sendReconnectToSql(QString); //传给sql 处理重连
    void sendReconnectToSock(QString,QString,int); //传给sock 重连
    void sendChangeServerToSql(QString,int,int,int,double); //传给sql 更改server表
    //void sendUpdateToSql(int,int); //更新tab2操作 给sql
    void sendChangeTimeToSql(QString,int);//更新时间 数据库
    void sendInfoToTab3(QString,bool,QString,QString,QString); //this 发送选项到tab3
    //通知消息发送到sock
    void sendbufToSock(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int,QString,QString);
    void sendbufToSql(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,QString,QString);
    //通知消息遍历发送到sock
    void sendInfoToSock(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //消息查询初始化数据库查询
    void sendInfoInitToSql();
    void sendInfoInitToInfo(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //更新Info表信号发送
    void sendInfoUpdate();
    //更新表数据发送
    void sendUpdateToInfo(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int);
    //查看gwids发给数据库
    void sendgwidsToSql(QString);
    //wgids列表显示
    void sendgwidsToList(QString,QString,int);
    //清空列表
    void sendclearlist();
    //网关查询发送
    void sendSelectWgToSock(QString);
    //网关查询定时器
    //void selectWgTime(QString);
    //设备详细列表设置
    void sendDeviceInfoToSock(int);

    //删除tablewidget行
    void senddeleteToSql(QString,int);
   //tab2刷新和界面数据刷新
    void sendTab2UpdateToSql();
    //通知消息初始化
    void sendInitInfo();
    //发送给userid给数据库网关更新信息
    void sendUseridToSql(QString,QString,bool,QString,int);
    void sendtest();
private:
    Ui::TabWidget *ui;
    //通知子界面
    tab3_widget tab3;
    //查询通知子界面
    findInfo find;
    //查询网关子界面
    wg_demand find_wg;
    //修改密码子界面
    changepwd change;
    //退出子界面
    quit q;
    //网关更新子界面
    wg_updata updata_wg;
    //通知成员列表
    sendlist wglist;
    //设备列表
    equipment_list equiList;
    //设备详细信息
    device_show device;
    //推荐人页面
    setreferrer setrefe;
    //添加推荐人页面
    addreferees addrefe;
    //修改时间页面
    changeTime changetime;
    //升级界面
    update_list wg_updatelist;
    //升级网关列表
    update_wglist upwglist;
    //设置网关截止时间页面
    //setstoptime wg_setstoptime;

    //接收数据库更新信息
    void dataReceived(int app_count,int wg_count);
    int maxSize;   //最大数量
    int maxX;
    int maxY;
    int nCurScroller = 0; //翻页时滑动条位置
    int pageValue = 10;  //一页显示条数
    int gwSelect = 0;
    QString gwid;  //查询网关id
    QList<int> ports;
    QList<int> users;
    QChart *chart;
    QChartView *chartView;
    QSplineSeries *ipSplineSeries;
    QSplineSeries *userSplineSeries;
    QScatterSeries *userScatterSeries;
    QScatterSeries *ipScatterSeries;

    //widgetthread *widgetT;  //线程定时器动态条形图刷新
    //QThread *thread;
    widgetsocket *widgetS; //线程网络通讯层
    QThread *threads;
    sqldeal *sql;   //线程数据库处理层
    QThread *sqlthread;
    //parse *par;   //数据解析
    //QThread *parthread;

    QTimer *timer;      //三秒 sock心跳
    QTimer *timerinfo;  //一个小时  通知
    QTimer *timeOne;    //一秒    管理员时间减少 桌面时间更新
    QTimer *timeTen;    //十秒    服务器列表更新
    QTimer *timerfive; //五秒

    QProgressDialog *progressDialog;

};

#endif // TABWIDGET_H
