#ifndef PARSE_H
#define PARSE_H

#include <QObject>
#include <QDebug>
#include <QtEndian>
#include <QString>
#include <QMap>

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "md5.h"
#include <sys/types.h>


class parse : public QObject
{
    Q_OBJECT
public:
    explicit parse(QObject *parent = 0);
   /* QMap<QString,unsigned char*> wg_list;          //网关
    QMap<QString,unsigned char*> light_list;       //开关
    QMap<QString,unsigned char*> socket_list;      //插座
    QMap<QString,unsigned char*> curtain_list;     //窗帘
    QMap<QString,unsigned char*> infrared_list;    //红外*/

public slots:
    void onpacket(char*,int);
signals:
     //void sendChangeServer(QString,int,int,int,double); //发送服务器数据修改信号
};

#endif // PARSE_H
