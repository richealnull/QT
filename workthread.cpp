#include "workthread.h"

workthread::workthread(QObject *parent) : QObject(parent)
{
}

void workthread::login(QString login_user,QString login_pwd)
{
    qDebug()<<"登录验证连接数据库"<<endl;
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
   QString user;
   QString pwd ;
   int userid =0, server = 0,info = 0, inquire =0 ,update = 0;
   QString time;
   if(ret)
   {
       QSqlQuery query;
       query.exec("select * from general_user where user ='"+login_user+"'and passwor = '"+login_pwd+"'");
       while(query.next())
       {
            userid = query.value(0).toInt();
            user = query.value(1).toString();
            pwd = query.value(7).toString();
            server = query.value(2).toInt();
            info = query.value(3).toInt();
            inquire = query.value(4).toInt();
            update = query.value(5).toInt();
            time =query.value(6).toString();
            qDebug()<<server<<info<<inquire<<update<<endl;
            qDebug() <<"子线程号:"<< QThread::currentThread()<<endl;
       }
       emit loginsignal(user,pwd,server,info,inquire,update,time,userid);
   }
   d.close();
}
