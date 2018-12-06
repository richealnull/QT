#ifndef WIDGETSOCKET_H
#define WIDGETSOCKET_H

#include <QMetaType>
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QHostAddress>
#include <QThread>
#include <QTimer>
#include <QVector>

class widgetsocket : public QObject
{
    Q_OBJECT
public:
    explicit widgetsocket(QObject *parent = 0); 
    QMap<QString,QTcpSocket*> _sock;               //通讯容器
    QVector<QString> _ids;                          //app——id容器
    QVector<QString> wg_ids;                        //wg_id容器
    QMap<QString,unsigned char*> wg_list;          //网关
    QMap<QString,unsigned char*> light_list;       //开关
    QMap<QString,unsigned char*> socket_list;      //插座
    QMap<QString,unsigned char*> curtain_list;     //窗帘
    QMap<QString,unsigned char*> infrared_list;    //红外 
    void onpacket(char*,int);  //数据解析
    int packagedate(unsigned char**);  //通知封装
    int packbegin(unsigned char**,int); //封装开始发送数据
    int packend(unsigned char**,int); //封装发送数据结束
    void dealAllServerIds(QString);    //处理所有服务器id获取信息发送

    void dealAllServerUpdate();          //处理更新网关id获取信息发送
    void wgFilebegin(unsigned char**);   //网关文件发送开始封装
    void wgFileend(unsigned char**);      //网关文件发送结束封装

    char sysname[13] = {0}; //注册id
    char imgfile[9] ={0};   //生成的图片名
    char imgfile2[9] = {0};
    char imgfile3[9] = {0};
    QString img1,img2,img3; //图片数据
    int imagenum = 0; //图片数量
    int filepacknum = 0; //包总数量
    int servernum;//服务器在线数量
    int idnum; //id数量
    QString sendobj,sendtype,sendtitle,sendedit,sendlink,sendlinkplace;//发送对象，类型，标题，内容，连接，链接地址
    int sendgwsid;
    //网关更新参数
    int servernum2 = 0; //服务器在线数量
    QString file_path; //文件地址
    int update_id = 0; //更新id
    int update_filepacknum = 0; //更新文件包数量
    int wgidnum = 0; //网关数量
    char updatename[9] = {0};



signals:
    void sendAddServer(QString,QString,int);  //发送服务器添加信号
    void sendReconnect(QString);    //发送服务器重连信号
    void sendChangeServer(QString,int,int,int,double); //发送服务器数据修改信号
    void sendInitcombox(QString); //初始化服务器列表选择
    void sendAddServerFail();  //服务器添加失败
    void sendWgPermission(QString);  //网关回发消息
    void sendParse(char*,int); //发送解析信号
    void sendStat(QString,bool);  //发送服务器状态
    void sendDeviceNums(int,int,int,int,int); //发送设备数量
    void sendDeviceInfo(QMap<QString,unsigned char*>);//发送设备详情
    void sendSaveIdToSql(int,QString);  //存发送到数据库
    void sendProgressBar(int);  //界面生成进度条信号
    void sendProgressBarChange(int); //界面进度条更改
    void stopTimeFive();//停止网关查询定时器
    void sendTimeToSql_UI(QByteArray,short,QString); //网关在线确认

public slots:
    void heartbeat();  //处理心跳
    void dealrestart(QString);  //处理重启
    void dealAddServer(QString,QString,int); //服务器添加链接
    void closesock();  //关闭连接
    void dealbuffer();  //数据处理
    void Createsocket(QString,QString,int);//创建连接
    void reconnection(QString,QString,int); //重连
    //void sendDisconn();  //服务器断开处理
    void sendInfoToServer(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int,QString,QString);
    //处理定时通知发送
    void dealInfoTimer(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    //处理网关查询信号
    void dealSelectWg(QString);
    //创建网关查询定时器
    //void dealSelectWgTime(QString);
    //设备数量信号处理
    void deviceNums();
    //设备详情信号处理
    void dealDeviceInfo(int);
    //清除map
    void dealClearMap();
    void initdate();        //通知消息初始化
    //更新网关初始化
    void initWgUpdate();
    //处理网关更新文件
    void dealWgUpdate(QString,QString,bool,QString,int);
    //发送截止时间给网关
    void sendStopTimeToGw(int,QByteArray,QString);

private:

};

#endif // WIDGETSOCKET_H
