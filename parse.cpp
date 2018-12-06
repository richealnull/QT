#include "parse.h"

parse::parse(QObject *parent) : QObject(parent)
{

}

void parse::onpacket(char *str, int size)
{   
    /*qDebug()<<"packet size is"<<size<<endl;

    //0x08  服务器数据
    if(str[0] == 0x08)
    {
        qDebug()<<"开始解析"<<endl;
        int iplen =   qFromBigEndian(*(int*)(str+1));
        qDebug()<<"namelen is "<<iplen<<endl;
        char* name = (char*)malloc(iplen+1);
        memcpy(name,(char*)str+1+4,iplen);
        name[iplen] = '\0';
        qDebug()<<"name is "<<name<<endl;
        int conn = qFromBigEndian(*(int *)(str+1+4+iplen));
        qDebug()<<"conn is "<<conn<<endl;
        int app =qFromBigEndian(*(int*)(str+1+4+iplen+4));
        qDebug()<<"app is "<<app<<endl;
        int wg = qFromBigEndian(*(int*)(str+1+4+iplen+4+4));
        qDebug()<<"wg is "<<wg<<endl;
        double cpu = *(double*)(str+1+4+iplen+4+4+4);
        qDebug()<<"cpu is "<<cpu<<endl;
        QString server_name = QString("%1").arg(name);
        qDebug()<<"解析完成"<<endl;
        //emit sendChangeServer(server_name,conn,app,wg,cpu);
    }
    //0x05设备详情包
    else if(str[6] == 0x05)
    {
        char* hear = (char*)malloc(3);
        memcpy(hear,(char*)str+13,3);
        QString  gwid = QString(QLatin1String(hear));
        unsigned long long long_buf =(*((unsigned long long*)(str+16)));
        gwid += QString::number(long_buf,10);  //网关id转为QString
        unsigned char device_type = (*(unsigned char*)(str+34)); //设备类型
        unsigned char device_num = (*(unsigned char*)(str+35)); //设备数量
        unsigned short device_startnumber = (*(unsigned char*)(str+36));//设备起始序号
        int device_typee = (int)(device_type-'0'); //设备类型转为int
        int device_numm=(int)(device_num-'0');  //设备数量转为int
        int pos =37;
        for(int i =0;i<device_numm;i++)
        {
            unsigned short device_number = (*(unsigned char*)(str+pos)); //设备序号
            pos += 2;
            unsigned short product_type = (*(unsigned char*)(str+pos)); //产品类型
            pos += 2;
            unsigned char namelen = (*(unsigned char*)(str+pos));
            pos += 1;
            int namelens = (int)(namelen='0');     //名字长度
            char* name = (char*)malloc(namelens+1);
            memcpy(name,(char*)str+pos,namelens);
            name[namelens] = '\0';
            QString Name = QString(QLatin1String(name));    //设备名转换
            pos += namelens;
            unsigned char version[2] ={0};  //软硬件版本合集
            unsigned char Software_version = (*(unsigned char*)(str+pos));  //软件版本
            memcpy(version,(unsigned char*)Software_version,1);
            //QString software_version = QString(Software_version); //软件版本转换
            pos += 1;
            unsigned char Hardware_version = (*(unsigned char*)(str+pos));  //硬件版本
            memcpy(version+1,(unsigned char*)Hardware_version,1);
            //QString hardware_version = QString(Hardware_version);  //硬件版本转换
            pos += 1;
            if(device_typee == 1)
            {
                light_list.insert(Name,version);
            }
            else if(device_typee == 2)
            {
                socket_list.insert(Name,version);
            }
            else if(device_type == 3)
            {
                curtain_list.insert(Name,version);
            }
            else if(device_type == 4)
            {
                infrared_list.insert(Name,version);
            }
        }
        if(device_num<20)
        {
            //继续发请求包 给socket层
        }
    }*/
}
