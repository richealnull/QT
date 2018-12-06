#include "widgetthread.h"
#include <QtEndian>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>


widgetthread::widgetthread(QObject *parent) : QObject(parent)
{
    //isStop =false;

}

/*void widgetthread::widgetconn(QString time,int general_id)
{
    QString hh = time.mid(0,2);
    QString mm = time.mid(3,2);
    QString ss = time.mid(6,2);
    int a=hh.toInt()*3600+mm.toInt()*60+ss.toInt();
    qDebug()<<hh<<mm<<ss<<a<<endl;

    while(isStop == false){
    QThread::sleep(10);

    qDebug()<<"更新列表信号"<<endl;
    emit sendupdate_tab2_to_tab(general_id,a);

    a-=10;
    int hour = a/3600;
    int minute = (a - hour*3600)/60;
    int second = (a - hour*3600 - minute*60);
    QString label_time = QString::number(hour,10);
    label_time += ":";
    label_time += QString::number(minute,10);
    label_time += ":";
    label_time += QString::number(second,10);
    //qDebug()<<label_time<<endl;

    if(true == isStop)
    {  
        emit sendChangeTimeTotab(label_time,general_id);
        break;
    }
    }

}
void widgetthread::setFlag(bool flag)
{
    isStop = flag;
}*/

