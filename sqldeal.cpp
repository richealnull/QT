#include "sqldeal.h"
#include "des3.h"

/*QSqlDatabase *sigclass::sql=new QSqlDatabase;
mutex sigclass::m_mutex;

sigclass::sigclass()
{

}
QSqlDatabase *sigclass::GetInstance()
{

          std::lock_guard<std::mutex> lock(m_mutex);
          return sql;
}*/


   //QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
   //QTextCodec::setCodecForLocale(utf8);

sqldeal::sqldeal(QObject *parent) : QObject(parent)
{

}

void sqldeal::recvtab2_member()
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int userid = 0 ;
    QString ip;
    QString servername;
    QString dmname;
    int cpu = 0;
    int app_count =0;
    int wg_count =0;
    int port= 0;
    int id = 0;
    int conn_count = 0;
    bool state;
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from server_list");
        while(query.next())
        {
               userid = query.value(0).toInt();
               ip = query.value(1).toString();
               servername = query.value(2).toString();
               port = query.value(3).toInt();
               app_count =query.value(4).toInt();
               wg_count =query.value(5).toInt();
               cpu = query.value(6).toDouble();
               conn_count = query.value(7).toInt();
               state = query.value(8).toBool();
              qDebug()<<"初始化列表"<<userid<<ip<<servername<<port<<app_count<<wg_count<<dmname<<cpu<<id<<state<<endl;
              emit painttable2(id,ip,servername,port,conn_count,app_count,wg_count,cpu,state);
              id++;
        }
    }
    d.close();
}

void sqldeal::addServer(QString serverip,QString servername,int serverport)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        QString temp1 = QObject::tr("INSERT INTO server_list (ip,server_name,port)  VALUES('%1','%2','%3')").arg(serverip).arg(servername).arg(serverport);
        bool isok;
        isok = query.exec(temp1);
        if(isok){
        qDebug()<<"成功了"<<endl;
        }
        else{
            qDebug()<<"添加服务器失败"<<endl;
        }
    }
    d.close();
}
void sqldeal::connserver()
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int userid = 0 ;
    QString ip;
    QString servername;
    int app_count =0;
    int wg_count =0;
    int port= 0;
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from server_list");
        while(query.next())
        {
               userid = query.value(0).toInt();
               ip = query.value(1).toString();
               servername = query.value(2).toString();
               port = query.value(3).toInt();
               app_count +=query.value(4).toInt();
               wg_count +=query.value(5).toInt();
              qDebug()<<userid<<ip<<servername<<port<<app_count<<wg_count<<endl;
              emit sendconnserver(servername,ip,port);
        }
    }
    d.close();
}

void sqldeal::reconnect(QString servername)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from server_list where server_name='"+servername+"'");
        while(query.next())
        {
            QString name = query.value(2).toString();
            QString ip = query.value(1).toString();
            int port = query.value(3).toInt();
            emit sendreconnect(name,ip,port);
        }
    }
    d.close();
}

void sqldeal::changeserver(QString uip, int conn_count, int app_count, int wg_count, double cpu)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        QString temp = QObject::tr("UPDATE server_list SET app_count='%1',wg_count='%2',conn_count='%3',cpu ='%4' WHERE server_name ='%5'").arg(app_count).arg(wg_count).arg(conn_count).arg(cpu).arg(uip);
        bool isok;
        isok = query.exec(temp);
        if(isok){
        qDebug()<<"更新服务器成功"<<endl;
        }
        else{
            qDebug()<<"更新服务器列表失败"<<endl;
        }
    }
    d.close();
}

void sqldeal::updatetab2()
{
    qDebug()<<"数据库查询serverlist"<<endl;
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();

    int userid = 0 ;
    QString ip;
    QString servername;
    QString dmname;
    double cpu = 0;
    int app_count =0;
    int wg_count =0;
    int port= 0;
    int id = 0;
    int conn_count = 0;
    int appall = 0;
    int wgall = 0;
    bool state;
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from server_list");
        while(query.next())
        {
            userid = query.value(0).toInt();
            ip = query.value(1).toString();
            servername = query.value(2).toString();
            port = query.value(3).toInt();
            app_count =query.value(4).toInt();
            wg_count =query.value(5).toInt();
            cpu = query.value(6).toDouble();
            conn_count = query.value(7).toInt();
            state = query.value(8).toBool();
              qDebug()<<"刷新列表"<<userid<<ip<<servername<<port<<app_count<<wg_count<<dmname<<cpu<<id<<state<<endl;
              emit sendupdate_tab2(id,ip,servername,port,conn_count,app_count,wg_count,cpu,state);
              id++;
              appall += app_count;
              wgall += wg_count;
        }
    }
    qDebug()<<"到这了吗？"<<endl;
    emit connsignal(userid,ip,servername,port,appall,wgall);
    d.close();
}

void sqldeal::changeTime(QString label_time,int general_id)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        QString temp2 = QObject::tr("update general_user set permis_time='%1' where userid='%2'").arg(label_time).arg(general_id);
        query.exec(temp2);
        qDebug()<<general_id<<label_time<<endl;
    }
    d.close();
    qDebug()<<"更改管理员关闭"<<endl;
}

void sqldeal::addInfo(QString wg_name, bool radiobtn, QString serverall, QString obj, QString type, QString dateedit, QString houre, QString title, QString edit, QString link, QString linkplace, QString imgAdress,int user_id,QString imgAdress2, QString imgAdress3)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd HH:mm:ss");
    qDebug()<<"当前发送时间"<<current_date<<endl;
    qDebug()<<serverall<<obj<<type<<title<<edit<<endl;

    title = title.toUtf8();

    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int gwsid = 0;
    if(ret)
    {
        QSqlQuery query;
        QString temp1 = QObject::tr("INSERT INTO message_list (userid,num,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,radiobtn,serverall,obj,type,timebegin,imgaddress2,imgaddress3)  VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18')")
                .arg(user_id).arg(0).arg(dateedit).arg(title).arg(edit).arg(link).arg(linkplace).arg(imgAdress).arg(houre).arg(current_date).arg(wg_name).arg(radiobtn).arg(serverall).arg(obj).arg(type).arg(current_date).arg(imgAdress2).arg(imgAdress3);
        bool isok;
        query.exec("SET NAMES 'utf8'");
        isok = query.exec(temp1);
        if(isok){
        qDebug()<<"消息入库"<<radiobtn<<serverall<<obj<<type<<endl;
        }
        else{
            qDebug()<<"通知保存数据库失败"<<endl;
        }

        query.exec("select * from message_list where timebegin = '"+current_date+"'");
        while(query.next())
        {
               gwsid = query.value(4).toInt();
              qDebug()<<"test :"<<endl;
              emit sendInfoToSock(wg_name,radiobtn,serverall,obj,type,dateedit,houre,title,edit,link,linkplace,imgAdress,user_id,gwsid,imgAdress2,imgAdress3);
        }
    }
    d.close();
}

void sqldeal::test()
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int userid = 0 ;
    QString timeend;
    QString title;
    if(ret)
    {
        QSqlQuery query;
        query.exec("set character set 'utf8'");
        query.exec("select * from message_list");
        while(query.next())
        {
               userid = query.value(0).toInt();
               timeend = query.value(2).toString();
               title = query.value(3).toString();
              qDebug()<<"test :"<<userid<<timeend<<title<<endl;
        }
    }
    d.close();
}

void sqldeal::infotimer()
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int userid = 0;
    int num = 0;
    int gwids = 0;
    bool radiobtn;
    QString timeend,title,content,link,linkplace,imgaddress;
    QString intervaltime,lasttime,wg_name,serverall,obj,type,timebegin;
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from message_list");
        while(query.next())
        {
            userid = query.value(0).toInt();
            num = query.value(1).toInt();
            timeend = query.value(2).toString();
            title = query.value(3).toString();
            gwids = query.value(4).toInt();
            content = query.value(5).toString();
            link = query.value(6).toString();
            linkplace = query.value(7).toString();
            imgaddress = query.value(8).toString();
            intervaltime = query.value(9).toString();
            lasttime = query.value(10).toString();
            wg_name = query.value(11).toString();
            radiobtn = query.value(12).toBool();
            serverall = query.value(13).toString();
            obj = query.value(14).toString();
            type = query.value(15).toString();
            timebegin = query.value(16).toString();
            qDebug()<<userid<<num<<radiobtn<<timeend<<title<<content<<link<<linkplace<<imgaddress<<intervaltime<<lasttime<<wg_name<<serverall<<obj<<type<<timebegin<<endl;
            emit infoSendToThis(userid,num,radiobtn,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,serverall,obj,type,timebegin,gwids);
        }
        qDebug()<<"一个小时"<<endl;
    }
    d.close();
}

void sqldeal::dealInfoInit()
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int userid = 0;
    int num = 0;
    int gwids = 0;
    bool radiobtn;
    QString timeend,title,content,link,linkplace,imgaddress;
    QString intervaltime,lasttime,wg_name,serverall,obj,type,timebegin;
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from message_list");
        while(query.next())
        {
            userid = query.value(0).toInt();
            num = query.value(1).toInt();
            timeend = query.value(2).toString();
            title = query.value(3).toString();
            gwids = query.value(4).toInt();
            content = query.value(5).toString();
            link = query.value(6).toString();
            linkplace = query.value(7).toString();
            imgaddress = query.value(8).toString();
            intervaltime = query.value(9).toString();
            lasttime = query.value(10).toString();
            wg_name = query.value(11).toString();
            radiobtn = query.value(12).toBool();
            serverall = query.value(13).toString();
            obj = query.value(14).toString();
            type = query.value(15).toString();
            timebegin = query.value(16).toString();
            qDebug()<<userid<<num<<radiobtn<<timeend<<title<<content<<link<<linkplace<<imgaddress<<intervaltime<<lasttime<<wg_name<<serverall<<obj<<type<<timebegin<<endl;
            emit initInfoSendToThis(userid,num,radiobtn,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,serverall,obj,type,timebegin,gwids);
        }
        qDebug()<<"通知列表初始化"<<endl;
    }
    d.close();
}

void sqldeal::dealInfoUpdate()
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int userid = 0;
    int num = 0;
    int gwids = 0;
    bool radiobtn;
    int listnum = 0;
    QString timeend,title,content,link,linkplace,imgaddress;
    QString intervaltime,lasttime,wg_name,serverall,obj,type,timebegin;
    if(ret)
    {
        QSqlQuery query;
        query.exec("select * from message_list");
        while(query.next())
        {
            userid = query.value(0).toInt();
            num = query.value(1).toInt();
            timeend = query.value(2).toString();
            title = query.value(3).toString();
            gwids = query.value(4).toInt();
            content = query.value(5).toString();
            link = query.value(6).toString();
            linkplace = query.value(7).toString();
            imgaddress = query.value(8).toString();
            intervaltime = query.value(9).toString();
            lasttime = query.value(10).toString();
            wg_name = query.value(11).toString();
            radiobtn = query.value(12).toBool();
            serverall = query.value(13).toString();
            obj = query.value(14).toString();
            type = query.value(15).toString();
            timebegin = query.value(16).toString();
            qDebug()<<userid<<num<<radiobtn<<timeend<<title<<content<<link<<linkplace<<imgaddress<<intervaltime<<lasttime<<wg_name<<serverall<<obj<<type<<timebegin<<endl;
            emit updateInfoSendToThis(userid,num,radiobtn,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,serverall,obj,type,timebegin,listnum,gwids);
            listnum++;
        }
        qDebug()<<"更新消息列表"<<endl;
    }
    d.close();
}

void sqldeal::dealgwidslist(QString gwids)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    int gwidss = gwids.toInt();
    QString wgname;
    int listnum = 0;
    if(ret)
    {
        QSqlQuery query;
        QString temp = QString::fromUtf8("select * from message_wglist where message_id = '%1'").arg(gwidss);
        bool isok;
        query.exec("SET NAMES 'utf8'");
        isok = query.exec(temp);
        if(isok){
        qDebug()<<"查看发送列表"<<endl;
        while(query.next())
        {
            wgname = query.value(1).toString();
            qDebug()<<"wgname"<<wgname<<endl;
            emit sendwglist(gwids,wgname,listnum);
            listnum++;
        }
        }
        else{
            qDebug()<<"查看发送列表失败"<<endl;
        }
    }
    d.close();
}

void sqldeal::dealDeleteTablewidget(QString serverName,int i)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();

    if(ret)
    {
        QSqlQuery query;
        QString temp = QString::fromUtf8("delete from server_list where server_name = '%1'").arg(serverName);
        bool isok;
        query.exec("SET NAMES 'utf8'");
        isok = query.exec(temp);
        if(isok)
        {
            qDebug()<<"删除成功"<<endl;
            emit sendDeleteToThis(i);
        }
        else{
            qDebug()<<"数据库中查找不到"<<endl;
        }
    }
    d.close();
}

void sqldeal::changeServerState(QString serverName,bool state)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        QString temp2 = QObject::tr("update server_list set state='%1' where server_name='%2'").arg(state).arg(serverName);
        query.exec(temp2);
        qDebug()<<"服务器状态改变成功"<<serverName<<state<<endl;
    }
    d.close();
}

void sqldeal::SaveId(int gwsid,QString id)
{
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        QString temp2 = QObject::tr("INSERT INTO message_wglist (message_id,gwname)  VALUES('%1','%2')").arg(gwsid).arg(id);
        query.exec(temp2);
        qDebug()<<"通知发送给"<<id<<endl;
    }
    d.close();
}
void sqldeal::saveUpdateFile(QString wgname, QString servername, bool allserver,QString updateadress,int userid){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd HH:mm:ss");
    qDebug()<<"当前发送时间"<<current_date<<endl;

    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    if(ret)
    {
        QSqlQuery query;
        QString temp2 = QObject::tr("INSERT INTO update_wg (userid,servername,wgname,updateadress,allserver，datetime)  VALUES('%1','%2','%3','%4','%5'，'%6')").arg(userid).arg(servername).arg(wgname).arg(updateadress).arg(allserver).arg(current_date);
        bool isok = query.exec(temp2);
        qDebug()<<"更新文件给网关"<<endl;
            //if(isok){
              qDebug()<<"test :"<<endl;
              emit sendWgUpdateToSock(wgname,servername,allserver,updateadress,userid);
              //emit sendInfoToSock(wg_name,radiobtn,serverall,obj,type,dateedit,houre,title,edit,link,linkplace,imgAdress,user_id,gwsid,imgAdress2,imgAdress3);
        //}
    }
    d.close();
}
void sqldeal::gwOnline(QByteArray miwen,short timelen,QString gwid){
    qDebug()<<"数据库差截止时间"<<gwid<<endl;
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    QByteArray sqlmiwen;
    short sqltimelen;
    if(ret)
    {
        qDebug()<<"成功打开数据库"<<endl;
        bool isok;
        int num;
        QSqlQuery query;   
            isok = query.exec("select * from gw_server_time where gw_id = '" +gwid+ "'");
            qDebug()<<"查询执行结果"<<isok<<endl;
            if(query.size()){
                while(query.next())
                {
                    sqltimelen = query.value(3).toInt();
                    qDebug()<<"密文长度"<<sqltimelen<<endl;

                    sqlmiwen.resize(sqltimelen);
                    sqlmiwen = QByteArray::fromHex(query.value(2).toByteArray());
                    unsigned char* str = (unsigned char*)sqlmiwen.data();
                    //解密
                    unsigned char* p_mingwen;
                    sqltimelen = des3_api(DES_DECRYPT, str, sqltimelen, &p_mingwen);
                    int sqlstoptime = *(int*)(p_mingwen);  //数据库时间戳
                    qDebug()<<"数据库取出时间戳"<<sqlstoptime<<endl;
                    unsigned char* gw_p_miwen = (unsigned char*)miwen.data();
                    for (int i = 0; i < sqltimelen; i++) {
                        printf("%02x",*(str + i));
                    }
                    printf("\n");

                    unsigned char* gw_p_mingwen;
                    int mtimelen = des3_api(DES_DECRYPT, gw_p_miwen, timelen, &gw_p_mingwen);
                    int stoptime = *(int*)(gw_p_mingwen);  //网关发送时间戳
                    qDebug()<<"查询网关时间戳更新"<<stoptime<<endl;
                    if(stoptime<sqlstoptime){
                        //发送修改消息过去
                        qDebug()<<"发送修改消息过去"<<endl;
                        emit sendToWgAddDays(sqltimelen,sqlmiwen,gwid);
                    }
                    else if(stoptime>sqlstoptime){
                        //修改数据库
                        miwen = miwen.toHex();
                        QString temp2 = QObject::tr("update gw_server_time set gw_stop_time='%1' ,timelen='%2' where gw_id='%3'").arg(miwen.data()).arg(timelen).arg(gwid);
                        isok =query.exec(temp2);
                        if(isok){
                            qDebug()<<"时间戳更改成功"<<endl;
                        }
                        else{
                            qDebug()<<"时间戳更改失败"<<endl;
                        }
                    }
                }
                }
        else
        {
            qDebug()<<"消息过去"<<endl;
            //存进数据库  

            unsigned char* wgmiwen = (unsigned char*)miwen.data();
            unsigned char* gw_p_mingwen;
            timelen = des3_api(DES_DECRYPT, wgmiwen, timelen, &gw_p_mingwen);
            int stoptime = *(int*)(gw_p_mingwen);  //网关发送时间戳
            qDebug()<<"查询网关时间戳更新"<<stoptime<<endl;
            miwen = miwen.toHex();
            QString temp = QObject::tr("INSERT INTO gw_server_time (gw_id,gw_stop_time,timelen)  VALUES('%1','%2','%3')").arg(gwid).arg(miwen.data()).arg(timelen);
            isok = query.exec(temp);
            if(isok){
                qDebug()<<"时间戳增加成功"<<endl;
            }
            else{
                 qDebug()<<"时间戳增加失败"<<endl;
            }
        }
    }
    d.close();
}

void sqldeal::dealWgPay(int daytime,QString payfrom,QString id){
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    QByteArray sqlmiwen;
    int sqltimelen;
    time_t  nowtime = time(NULL);
    bool ret = d.open();
    if(ret)
    {
        bool isok;
        QSqlQuery query;
        isok = query.exec("select * from gw_server_time where gw_id = '"+id+"'");
        if(isok){
            while(query.next())
            {
                sqltimelen = query.value(3).toInt();
                qDebug()<<"密文长度"<<sqltimelen<<endl;
                sqlmiwen.resize(sqltimelen);
                sqlmiwen = QByteArray::fromHex(query.value(2).toByteArray());
                unsigned char* str = (unsigned char*)sqlmiwen.data();
                unsigned char* p_mingwen;
                sqltimelen = des3_api(DES_DECRYPT, str, (short)sqltimelen, &p_mingwen);
                int sqlstoptime = *(int*)(p_mingwen);  //数据库时间戳
                qDebug()<<"充值钱时间戳"<<sqlstoptime<<endl;
                if(sqlstoptime<=nowtime){
                    sqlstoptime = daytime+nowtime;
                     qDebug()<<"充值后时间戳"<<sqlstoptime<<endl;
                }
                else{
                    sqlstoptime += daytime;
                    qDebug()<<"充值后时间戳"<<sqlstoptime<<endl;
                }
                unsigned char* sendstoptime = (unsigned char*)malloc(256);
                memcpy(sendstoptime,(unsigned char*)&sqlstoptime,4);
                memcpy(sendstoptime+4,PAD_STRING,strlen(PAD_STRING));
                //加密
                unsigned char * p_anwen;
                int len = strlen((const char *)sendstoptime);
                len = des3_api(DES_ENCRYPT, (unsigned char *)sendstoptime, len, &p_anwen);
                QByteArray qbytemiwen = QByteArray((const char*)p_anwen);
                 qbytemiwen = qbytemiwen.toHex();

                //存数据库发包给网关
                QString temp = QObject::tr("update gw_server_time set gw_stop_time='%1', timelen='%2' where gw_id='%3'").arg(qbytemiwen.data()).arg(len).arg(id);
                isok = query.exec(temp);
                if(isok){
                    qDebug()<<"时间戳更改成功"<<endl;
                }
                else{
                     qDebug()<<"时间戳更改失败"<<endl;
                }
                //生成订单
                sqlmiwen = sqlmiwen.toHex();
                QString temp1 = QObject::tr("INSERT INTO wg_bill_list (gw_id,pay_time,pay_form,pay_day,gw_stop_befor,gw_stop_after)  VALUES('%1','%2','%3','%4','%5','%6')").arg(id).arg(nowtime).arg(payfrom).arg(daytime).arg(sqlmiwen.data()).arg(qbytemiwen.data());
                isok = query.exec(temp1);
                if(isok){
                    qDebug()<<"订单增加成功"<<endl;
                }
                else{
                     qDebug()<<"订单增加失败"<<endl;
                }
                qbytemiwen = QByteArray::fromHex(qbytemiwen);
                emit sendToWgAddDays(len,qbytemiwen,id);
                free(sendstoptime);
                sendstoptime = NULL;
            }
            }
        else{
            QMessageBox::about(NULL, "错误", "数据库无此网关");
        }
        }
    d.close();
}

/*void sqldeal::dealWgChange(int stoptime, QString id){
    QSqlDatabase d;  //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        d = QSqlDatabase::database("qt_sql_default_connection");
    else
    d = QSqlDatabase::addDatabase("QMYSQL");
    d.setHostName("127.0.0.1");
    d.setDatabaseName("test");
    d.setPort(3306);
    d.setUserName("root");
    d.setPassword("huge123");
    bool ret = d.open();
    QByteArray sqlmiwen;
    int sqltimelen;
    if(ret)
    {
        bool isok;
        QSqlQuery query;
        isok = query.exec("select * from gw_server_time where gw_id = '"+id+"'");
        if(isok){
            while(query.next())
            {
                sqltimelen = query.value(3).toInt();
                qDebug()<<"密文长度"<<sqltimelen<<endl;
                sqlmiwen.resize(sqltimelen);
                sqlmiwen = QByteArray::fromHex(query.value(2).toByteArray());
                unsigned char* str = (unsigned char*)sqlmiwen.data();
                unsigned char* p_mingwen;
                sqltimelen = des3_api(DES_DECRYPT, str, (short)sqltimelen, &p_mingwen);
                int sqlstoptime = *(int*)(p_mingwen);  //数据库时间戳
                qDebug()<<"充值前时间戳"<<sqlstoptime<<endl;
                if(sqlstoptime<stoptime){
                    sqlstoptime = stoptime;
                }
                else{
                    sqlstoptime = stoptime;
                    QMessageBox::about(NULL, "warring", "网关截止时间大于所修改时间");
                }

                unsigned char* sendstoptime = (unsigned char*)malloc(256);
                memcpy(sendstoptime,(unsigned char*)&sqlstoptime,4);
                memcpy(sendstoptime+4,PAD_STRING,strlen(PAD_STRING));
                //加密
                unsigned char * p_anwen;
                int len = strlen((const char *)sendstoptime);
                len = des3_api(DES_ENCRYPT, (unsigned char *)sendstoptime, len, &p_anwen);
                QByteArray qbytemiwen = QByteArray((const char*)p_anwen);
                 qbytemiwen = qbytemiwen.toHex();

                //存数据库发包给网关
                QString temp = QObject::tr("update gw_server_time set gw_stop_time='%1', timelen='%2' where gw_id='%3'").arg(qbytemiwen.data()).arg(len).arg(id);
                isok = query.exec(temp);
                if(isok){
                    qDebug()<<"时间戳更改成功"<<endl;
                }
                else{
                     qDebug()<<"时间戳更改失败"<<endl;
                }
                qbytemiwen = QByteArray::fromHex(qbytemiwen);
                emit sendToWgAddDays(len,qbytemiwen,id);
                free(sendstoptime);
                sendstoptime = NULL;
            }
            }
        else{
            QMessageBox::about(NULL, "错误", "数据库无此网关");
        }

    }
    d.close();
}*/
