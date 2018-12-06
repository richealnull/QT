#include "widgetsocket.h"

#include <QtEndian>
#include <QString>
#include <QFileDialog>
#include <QProgressDialog>

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "md5.h"
#include "des3.h"
#include <sys/types.h>

#define SHORT_LEN 2
#define NAME_HEAD_LEN 3
#define LONG_LEN 8
#define NAME_LEN 13
char* _miyue = (char*)"aabbcc";

static const unsigned char TCPPROCOTOL_STATE_HEAD = 0;
static const unsigned char TCPPROCOTOL_STATE_LENGTH = 1;
static const unsigned char TCPPROCOTOL_STATE_CSLENGTH = 2;
static const unsigned char TCPPROCOTOL_STATE_BODY = 3;

//封包函数，141+内容
int buff(unsigned char* buffer,int size,unsigned char** _buff){
    qDebug()<<"141封装"<<endl;
    int _length = 1 + 4 + 1 + size;
    unsigned char* _buffer = (unsigned char *)malloc(_length);
    _buffer[0]  = 0xaa;
    *((unsigned int *)(_buffer + 1))    = qToBigEndian(_length);
    _buffer[5]  = 0 - _buffer[0] - _buffer[1] - _buffer[2] - _buffer[3] - _buffer[4];
    memcpy(_buffer + 6, buffer, size);
    *_buff = _buffer;
    return _length;
}

//封包多播包函数
int fengzhuang(unsigned char** send,unsigned char* recv,int size,unsigned char* id,int idsize)
{
    qDebug()<<"id大小"<<idsize<<endl;
    qDebug()<<"size大小"<<size<<endl;
    unsigned char* buffer = (unsigned char*)malloc(size+idsize+9);
    memset(buffer,0,size+idsize);
    int len =0;
    if(buffer == NULL)
    {
        qDebug()<<"malloc error\n"<<endl;
    }
    buffer[0] = 0x02; //设置广播标识码
    len += 1;
    short ids =1; //id数量
    short IDS = qToBigEndian(ids);
    memcpy(buffer+len,(unsigned char*)&IDS,sizeof(ids));
    len += sizeof(ids);
    unsigned short nlen = qToBigEndian((unsigned short)idsize);
    memcpy(buffer+len,(unsigned char*)&nlen,sizeof(nlen));//id长度
    len += sizeof(nlen);
    memcpy(buffer+len,id,idsize);//id
    len += idsize;
    int rlen = qToBigEndian(size);
    memcpy(buffer+len,(unsigned char*)&rlen,sizeof(rlen)); //数据包长
    len += sizeof(rlen);
    memcpy(buffer+len,recv,size); //数据包内容
    len += size;
    *send = buffer;
    return len;
    qDebug()<<"多播包大小"<<len<<endl;
}

widgetsocket::widgetsocket(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QMap<QString,unsigned char*>>("QMap<QString,unsigned char*>");
    time_t  systime = time(NULL);
    srand(systime);
    int sys= rand();
    char* heat =(char*)"SYS";
    unsigned long long lrand = sys;
    qDebug()<<"longlong is"<<lrand*12345<<endl;
    lrand = qToBigEndian(lrand*12345);
    memcpy(sysname,heat,3);
    memcpy(sysname + 3,(char*)&lrand,LONG_LEN);

}

//通知数据初始化
void widgetsocket::initdate()
{
    img1.clear();
    img2.clear();
    img3.clear();
    _ids.clear();
    servernum = 0;
    sendgwsid = 0;
    idnum = 0;
    imagenum = 0;
    sendobj.clear();
    sendtype.clear();
    sendtitle.clear();
    sendedit.clear();
    sendlink.clear();
    sendlinkplace.clear();
    //sysname[0] = '\0';
    imgfile[0] = '\0';
    imgfile2[0] = '\0';
    imgfile3[0] = '\0';
}
//网关更新初始化
void widgetsocket::initWgUpdate(){
    servernum2 = 0;
    file_path.clear();
    update_id = 0;
    update_filepacknum = 0;
    wgidnum = 0;
    updatename[0] ='\0';
}

//通知数据包封装一下
int widgetsocket::packagedate(unsigned char ** str)
{
    imgfile[0] = '\0';
    imgfile2[0] = '\0';
    imgfile3[0] = '\0';
    int size = 0;
    size = 75+sendlinkplace.size()+sendtitle.size()+sendedit.size();
    char* buffer = (char*)malloc(size);
    buffer[0] = 0x08;
    time_t  time1 = time(NULL);
    srand(time1);
    int suijima= rand();
    memcpy(buffer+2,(char*)&suijima,4);
    int pos = 7;
    memcpy(buffer+pos,sysname,13);
    pos += 13;
    buffer[pos] = 0x03;
    pos += 1;
    pos += 3;
    pos += 1;
    pos += 2;
    if(sendtype == "系统通知")
    {
        buffer[pos] = 0x01;
    }
    else if(sendtype == "机器人通知")
    {
        buffer[pos] = 0x02;
    }
    pos +=1;
    pos +=1;
    int imgnum = 3;
    if(img1.isEmpty()){
        imgnum -=1;
    }
    if(img2.isEmpty()){
        imgnum -=1;
    }
    if(img3.isEmpty()){
        imgnum -=1;
    }
    if(imgnum == 0){
        buffer[pos] = 0;
    }
    else if(imgnum == 1){
        buffer[pos] = 0x01;
    }
    else if(imgnum == 2){
        buffer[pos] = 0x02;
    }
    else{
        buffer[pos] = 0x03;
    }
    pos +=1;
    imagenum = imgnum;
    memcpy(buffer+pos,(char*)&time1,4);
    pos +=4;
    buffer[pos] = (char)(sendlinkplace.size());
    pos +=1;
    std::string sendlinkplacestr = sendlinkplace.toStdString();
    const char* someonename = sendlinkplacestr.c_str();
    memcpy(buffer+pos,someonename,sendlinkplace.size());
    pos += sendlinkplace.size();
    pos +=8;
    buffer[pos] =(char)(sendtitle.size());
    pos +=1;
    std::string sendtitlestr = sendtitle.toStdString();
    const char* titlename = sendtitlestr.c_str();
    memcpy(buffer+pos,titlename,sendtitle.size());
    pos += sendtitle.size();
    buffer[pos] = (char)(sendedit.size());
    pos +=1;
    std::string sendeditstr = sendedit.toStdString();
    const char* editname = sendeditstr.c_str();
    memcpy(buffer+pos,editname,sendedit.size());
    pos +=sendedit.size();
    buffer[pos] = (char)(sendlink.size());
    pos +=1;
    std::string sendlinkstr = sendlink.toStdString();
    const char* linkname = sendlinkstr.c_str();
    memcpy(buffer+pos,linkname,sendlink.size());
    pos +=sendlink.size();
    buffer[pos] = (char)imgnum;
    pos +=1;
    if(imgnum>0){
        char imgname[9] ={0};
        time_t time2 = time(NULL);
        int utc =(int)time2;
        srand(time2);
        int imgsuijima = rand();
        char imgtype = 0x03;
        memcpy(imgname,(char*)&utc,4);
        memcpy(imgname+4,(char*)&imgsuijima,4);
        memcpy(imgname+8,(char*)&imgtype,1);
        imgnum--;
        memcpy(buffer+pos,imgname,9);
        memcpy(imgfile,imgname,9);
        pos +=9;
        if(imgnum>0){
            char imgname2[9] = {0};
            imgsuijima += 1;
            memcpy(imgname2,(char*)&utc,4);
            memcpy(imgname2+4,(char*)&imgsuijima,4);
            memcpy(imgname2+8,(char*)&imgtype,1);
            imgnum--;
            memcpy(buffer+pos,imgname2,9);
            memcpy(imgfile2,imgname2,9);
            pos +=9;
            if(imgnum>0){
                char imgname3[9] = {0};
                imgsuijima += 1;
                memcpy(imgname3,(char*)&utc,4);
                memcpy(imgname3+4,(char*)&imgsuijima,4);
                memcpy(imgname3+8,(char*)&imgtype,1);
                imgnum--;
                memcpy(buffer+pos,imgname3,9);
                memcpy(imgfile3,imgname3,9);
                pos +=9;
            }
        }
    }
    *str = (unsigned char*)buffer;
    return pos;
}

//网关文件开始封装
void widgetsocket::wgFilebegin(unsigned char **str){
    update_filepacknum = 0;
    int pos = 0;
    char* begin = (char*)malloc(40);
    short file_num = 0;

    QFileInfo fileInfo(file_path);
        //---获取文件大小（字节）
    file_num = fileInfo.size()/800;
    if(fileInfo.size()%800 >0)
    {
        file_num +=1;
    }
    memcpy(begin+27,updatename,9);
    update_filepacknum = file_num;

    begin[0] = 0x09;
    pos +=2;
    time_t  time3 = time(NULL);
    srand(time3);
    int random= rand();
    memcpy(begin+pos,(char*)&random,4);
    pos +=4;
    begin[pos] = 0;
    pos +=1;
    memcpy(begin+pos,sysname,13);
    pos +=13;
    begin[pos] =0x05;
    pos +=1;
    begin[pos] =0;
    pos +=2;
    begin[pos] =0xFFF1;
    pos +=2;
    memcpy(begin+pos,(char*)&file_num,2);
    pos +=2;
    pos +=9;
    *str = (unsigned char*)begin;
}
//网关文件结束封装
void widgetsocket::wgFileend(unsigned char **str)
{
    int pos = 0;
    char *end = (char*)malloc(40);
    if(file_path.isEmpty()){
            qDebug()<<"发送结束包错误1"<<endl;
    }
    else{
        memcpy(end+27,updatename,9);
    }

    end[0] = 0x09;
    pos +=2;
    time_t  time4 = time(NULL);
    srand(time4);
    int randomm= rand();
    memcpy(end+pos,(char*)&randomm,4);
    pos +=4;
    end[pos] = 0;
    pos +=1;
    memcpy(end+pos,sysname,13);
    pos +=13;
    end[pos] =0x05;
    pos +=1;
    end[pos] =0;
    pos +=2;
    end[pos] =0xFFF2;
    pos +=2;
    memcpy(end+pos,(char*)&update_filepacknum,2);
    pos +=2;
    pos +=9;
    *str = (unsigned char*)end;
}
//封装开始发送
int widgetsocket::packbegin(unsigned char **str,int i)
{
    filepacknum = 0;
    int pos = 0;
    char *begin = (char*)malloc(40);
    short file_num = 0;
    if(i == 0){
        if(img1.isEmpty()){
            i++;
        }
        else{
            QFileInfo fileInfo(img1);
                //---获取文件大小（字节）
            file_num = fileInfo.size()/800;
            if(fileInfo.size()%800 >0)
            {
                file_num +=1;
            }
            memcpy(begin+27,imgfile,9);
        }
    }
    if(i == 1){
        if(img2.isEmpty()){
            i++;
        }
        else{
            QFileInfo fileInfo(img2);
                //---获取文件大小（字节）
            file_num = fileInfo.size()/800;
            if(fileInfo.size()%800 >0)
            {
                file_num +=1;
            }
            memcpy(begin+27,imgfile2,9);
        }
    }
    if(i == 2){
        if(img3.isEmpty()){
            i++;
            return i;
        }
        else{
            QFileInfo fileInfo(img3);
                //---获取文件大小（字节）
            file_num = fileInfo.size()/800;
            if(fileInfo.size()%800 >0)
            {
                file_num +=1;
            }
            memcpy(begin+27,imgfile3,9);
        }
    }
    filepacknum = file_num;

    begin[0] = 0x09;
    pos +=2;
    time_t  time3 = time(NULL);
    srand(time3);
    int random= rand();
    memcpy(begin+pos,(char*)&random,4);
    pos +=4;
    begin[pos] = 0;
    pos +=1;
    memcpy(begin+pos,sysname,13);
    pos +=13;
    begin[pos] =0x05;
    pos +=1;
    begin[pos] =0;
    pos +=2;
    begin[pos] =0xFFF1;
    pos +=2;
    memcpy(begin+pos,(char*)&file_num,2);
    pos +=2;
    pos +=9;
    *str = (unsigned char*)begin;
    return i;

}
//数据发送结束封装
int widgetsocket::packend(unsigned char **str,int i){
    int pos = 0;
    char *end = (char*)malloc(40);
    if(i == 0){
        if(img1.isEmpty()){
            qDebug()<<"发送结束包错误1"<<endl;
        }
        else{
            memcpy(end+27,imgfile,9);
        }
    }
    if(i == 1){
        if(img2.isEmpty()){
            qDebug()<<"发送结束包错误2"<<endl;
        }
        else{
            memcpy(end+27,imgfile2,9);
        }
    }
    if(i == 2){
        if(img3.isEmpty()){
            qDebug()<<"发送结束包错误3"<<endl;
            return i;
        }
        else{
            memcpy(end+27,imgfile3,9);
        }
    }
    end[0] = 0x09;
    pos +=2;
    time_t  time4 = time(NULL);
    srand(time4);
    int randomm= rand();
    memcpy(end+pos,(char*)&randomm,4);
    pos +=4;
    end[pos] = 0;
    pos +=1;
    memcpy(end+pos,sysname,13);
    pos +=13;
    end[pos] =0x05;
    pos +=1;
    end[pos] =0;
    pos +=2;
    end[pos] =0xFFF2;
    pos +=2;
    memcpy(end+pos,(char*)&filepacknum,2);
    pos +=2;
    pos +=9;
    *str = (unsigned char*)end;
    return i;
}


//数据解析
void widgetsocket::onpacket(char *str, int size)
{
    qDebug()<<"packet size is"<<size<<endl;
    //0x08  服务器数据
    if(str[6] == 0x08)
    {
        qDebug()<<"开始解析"<<endl;
        int iplen =   qFromBigEndian(*(int*)(str+7));
        qDebug()<<"namelen is "<<iplen<<endl;
        char* name = (char*)malloc(iplen+1);
        memcpy(name,(char*)str+7+4,iplen);
        name[iplen] = '\0';
        qDebug()<<"name is "<<name<<endl;
        int conn = qFromBigEndian(*(int *)(str+7+4+iplen));
        qDebug()<<"conn is "<<conn<<endl;
        int app =qFromBigEndian(*(int*)(str+7+4+iplen+4));
        qDebug()<<"app is "<<app<<endl;
        int wg = qFromBigEndian(*(int*)(str+7+4+iplen+4+4));
        qDebug()<<"wg is "<<wg<<endl;
        double cpu = *(double*)(str+7+4+iplen+4+4+4);
        qDebug()<<"cpu is "<<cpu<<endl;
        QString server_name = QString("%1").arg(name);
        qDebug()<<"解析完成"<<endl;
        emit sendChangeServer(server_name,conn,app,wg,cpu);
    }
    else if((str[6] == 0x03)|| (str[6] == 0x04)){//网关发消息
        if(str[6] == 0x03)
        {
            //解析数据 存id
            short idnum = *(short*)(str+7);
            qDebug()<<"id数是"<<idnum<<endl;
            int pos = 9;
            for(int i =0; i<idnum;i++)
            {
                short size = *(short*)(str+pos);
                pos += 2;
                char id[size] = {0};
                QString qid;
                memcpy(id,str+pos,size);
                id[size] ={'\0'};
                qDebug()<<"id 是:"<<id<<endl;
                qid = QString(QLatin1String(id));
                qDebug()<<"qid 是："<<qid<<endl;
                wg_ids.append(qid);
                pos += size;
            }
        }
        else if(str[6] == 0x04)
        {
            short idnums = *(short*)(str+7);
            qDebug()<<"id数是"<<idnums<<endl;
            int poss = 9;
            for(int i =0; i<idnums;i++)
            {
                short size = *(short*)(str+poss);
                qDebug()<<"id大小是"<<size<<endl;
                poss += 2;
                char id[size+1] = {0};
                QString qid;
                memcpy(id,str+poss,size);
                id[size] ={'\0'};
                qDebug()<<"id 是:"<<id<<endl;
                qid = QString(QLatin1String(id));
                qDebug()<<"qid 是："<<qid<<endl;
                wg_ids.append(id);
                poss += size;
            }
            //一个服务器接收完毕，继续发送给下一个服务器
           if(servernum2 >0)
           {
               dealAllServerUpdate();
           }
           else if(servernum2 == 0){
               QVector<QString>::iterator iter = wg_ids.begin();
               wgidnum = wg_ids.size();  //id数量
               //遍历给网关发送文件
               while(iter != wg_ids.end()){
                   //发送文件数量包begin
                   std::string strr = iter->toStdString();
                   const char* ch = strr.c_str();
                   qDebug()<<"发送id"<<ch<<endl;
                   unsigned char* beginfile = NULL;
                   wgFilebegin(&beginfile);
                   unsigned char* beginfileduobo = NULL;
                   unsigned char* beginfile141 = NULL;
                   int beginfileduobosize = fengzhuang(&beginfileduobo,beginfile,40,(unsigned char*)ch,strr.size());
                   int beginfile141size = buff(beginfileduobo,beginfileduobosize,&beginfile141);
                   if(_sock.size()>0)
                   {   //发送文件数量包
                       _sock.begin().value()->write((char*)beginfile141,beginfile141size);
                   }
                   unsigned char* sendbuff = (unsigned char*)calloc(1024,sizeof(unsigned char));
                   //发送文件
                   QFile file(file_path);
                   if(!file.open(QIODevice::ReadWrite)){
                       qDebug()<<"打开失败";
                   }
                   else{
                       qDebug()<<"打开成功";
                   }
                   short packnum = update_filepacknum;
                   short zz = 0;
                   while((zz=file.read((char*)(sendbuff+36),800)) == 0){
                       update_filepacknum -=1;
                       int pos =0;
                       memcpy(sendbuff+36+zz,(unsigned char*)&update_filepacknum,2);
                       sendbuff[0] = 0x09;
                       pos +=2;
                       time_t  time4 = time(NULL);
                       srand(time4);
                       int randomm= rand();
                       memcpy(sendbuff+pos,(char*)&randomm,4);
                       pos +=4;
                       sendbuff[pos] = 0;
                       pos +=1;
                       memcpy(sendbuff+pos,sysname,13);
                       pos +=13;
                       sendbuff[pos] = 0x05;
                       pos +=1;
                       sendbuff[pos] = 0;
                       pos +=2;
                       short filexuhao = packnum - update_filepacknum;
                       memcpy(sendbuff+pos,(char*)&filexuhao,2);
                       pos +=2;
                       memcpy(sendbuff+pos,updatename,9);
                       pos +=9;
                       memcpy(sendbuff+pos,(char*)&zz,2);
                       pos +=2;
                       unsigned char* buffduobo = NULL;
                       unsigned char* buff141 = NULL;
                       int buffduobosize = fengzhuang(&buffduobo,sendbuff,pos+zz,(unsigned char*)ch,strr.size());
                       int buff141size = buff(buffduobo,buffduobosize,&buff141);
                       if(_sock.size()>0)
                       {   //发送包
                           _sock.begin().value()->write((char*)buff141,buff141size);
                       }
                       memset(sendbuff,0,1024);
                   }
                   //发送文件结束包
                   unsigned char* endfile = NULL;
                   wgFileend(&endfile);
                   unsigned char* endfileduobo = NULL;
                   unsigned char* endfile141 = NULL;
                   int endfileduobosize = fengzhuang(&endfileduobo,endfile,40,(unsigned char*)ch,strr.size());
                   int endfile141size = buff(endfileduobo,endfileduobosize,&endfile141);
                   if(_sock.size()>0)
                   {   //发送文件数量包
                       _sock.begin().value()->write((char*)endfile141,endfile141size);
                   }
               }
               //emit sendSaveIdToSql(sendgwsid,*iter);
               iter++;
           }
        }
    }
    else if((str[6] == 0x01)||(str[6] == 0x02))//发给app的
    {
        qDebug()<<"解析id"<<endl;
        //这里写通知消息回发的id存vector  遍历id发送通知
        if(str[6] == 0x01)
        {
            //解析数据 存id
            short idnum = *(short*)(str+7);
            qDebug()<<"id数是"<<idnum<<endl;
            int pos = 9;
            for(int i =0; i<idnum;i++)
            {
                short size = *(short*)(str+pos);
                pos += 2;
                char id[size] = {0};
                QString qid;
                memcpy(id,str+pos,size);
                id[size] ={'\0'};
                qDebug()<<"id 是:"<<id<<endl;
                qid = QString(QLatin1String(id));
                qDebug()<<"qid 是："<<qid<<endl;
                _ids.append(qid);
                pos += size;
            }
        }
        else if(str[6] == 0x02)
        {
            short idnums = *(short*)(str+7);
            qDebug()<<"id数是"<<idnums<<endl;
            int poss = 9;
            for(int i =0; i<idnums;i++)
            {
                short size = *(short*)(str+poss);
                qDebug()<<"id大小是"<<size<<endl;
                poss += 2;
                char id[size+1] = {0};
                QString qid;
                memcpy(id,str+poss,size);
                id[size] ={'\0'};
                qDebug()<<"id 是:"<<id<<endl;
                qid = QString(QLatin1String(id));
                qDebug()<<"qid 是："<<qid<<endl;
                _ids.append(id);
                poss += size;
            }
             //一个服务器接收完毕，继续发送给下一个服务器
            if(servernum >0)
            {
                dealAllServerIds(sendobj);
            }
            else if(servernum ==0) //所有服务器id接收完毕
            {
                int i = 0;
                QVector<QString>::iterator iter = _ids.begin();
                idnum = _ids.size();  //id数量
                //开始遍历id  发送通知消息 界面来个进度条加数量
                emit sendProgressBar(idnum);
                int idnum2 = idnum;

                while(iter != _ids.end())
                {
                    std::string strr = iter->toStdString();
                    const char* ch = strr.c_str();
                    qDebug()<<"发送id"<<ch<<endl;
                    unsigned char* info = NULL;
                    unsigned char* infoduobo = NULL;
                    unsigned char* info141=NULL;
                    int size = packagedate(&info);   //封装通知消息发送
                    int duobosize = fengzhuang(&infoduobo,info,size,(unsigned char*)ch,strr.size());
                    int size141 = buff(infoduobo,duobosize,&info141);
                    if(_sock.size()>0)
                    {   //发送通知包
                        _sock.begin().value()->write((char*)info141,size141);
                    }
                    unsigned char* sendbuff = (unsigned char*)calloc(1024,sizeof(unsigned char));
                    while(imagenum != 0)
                    {
                        unsigned char* beginfile = NULL;
                        int o = packbegin(&beginfile,i);
                        unsigned char* beginfileduobo = NULL;
                        unsigned char* beginfile141 = NULL;
                        int beginfileduobosize = fengzhuang(&beginfileduobo,beginfile,40,(unsigned char*)ch,strr.size());
                        int beginfile141size = buff(beginfileduobo,beginfileduobosize,&beginfile141);
                        if(_sock.size()>0)
                        {   //发送文件数量包
                            _sock.begin().value()->write((char*)beginfile141,beginfile141size);
                        }
                        if(o == 0)
                        {
                            //发送第一个文件
                            QFile file(img1);
                            if(!file.open(QIODevice::ReadWrite)){
                                qDebug()<<"打开失败";
                            }
                            else{
                                qDebug()<<"打开成功";
                            }
                            short packnum = filepacknum;
                            short zz = 0;
                            while((zz=file.read((char*)(sendbuff+36),800)) == 0){
                                filepacknum -=1;
                                int pos =0;
                                memcpy(sendbuff+36+zz,(unsigned char*)&filepacknum,2);
                                sendbuff[0] = 0x09;
                                pos +=2;
                                time_t  time4 = time(NULL);
                                srand(time4);
                                int randomm= rand();
                                memcpy(sendbuff+pos,(char*)&randomm,4);
                                pos +=4;
                                sendbuff[pos] = 0;
                                pos +=1;
                                memcpy(sendbuff+pos,sysname,13);
                                pos +=13;
                                sendbuff[pos] = 0x05;
                                pos +=1;
                                sendbuff[pos] = 0;
                                pos +=2;
                                short filexuhao = packnum - filepacknum;
                                memcpy(sendbuff+pos,(char*)&filexuhao,2);
                                pos +=2;
                                memcpy(sendbuff+pos,imgfile,9);
                                pos +=9;
                                memcpy(sendbuff+pos,(char*)&zz,2);
                                pos +=2;
                                unsigned char* buffduobo = NULL;
                                unsigned char* buff141 = NULL;
                                int buffduobosize = fengzhuang(&buffduobo,sendbuff,pos+zz,(unsigned char*)ch,strr.size());
                                int buff141size = buff(buffduobo,buffduobosize,&buff141);
                                if(_sock.size()>0)
                                {   //发送包
                                    _sock.begin().value()->write((char*)buff141,buff141size);
                                }
                                memset(sendbuff,0,1024);
                            }

                            i++;
                            imagenum--;
                        }
                        else if(o == 1)
                        {
                            //发送第二个文件
                            QFile file(img2);
                            if(!file.open(QIODevice::ReadWrite)){
                                qDebug()<<"打开失败";
                            }
                            else{
                                qDebug()<<"打开成功";
                            }
                            short packnum = filepacknum;
                            short zz = 0;
                            while((zz=file.read((char*)(sendbuff+36),800)) == 0){
                                filepacknum -=1;
                                int pos =0;
                                memcpy(sendbuff+36+zz,(unsigned char*)&filepacknum,2);
                                sendbuff[0] = 0x09;
                                pos +=2;
                                time_t  time4 = time(NULL);
                                srand(time4);
                                int randomm= rand();
                                memcpy(sendbuff+pos,(char*)&randomm,4);
                                pos +=4;
                                sendbuff[pos] = 0;
                                pos +=1;
                                memcpy(sendbuff+pos,sysname,13);
                                pos +=13;
                                sendbuff[pos] = 0x05;
                                pos +=1;
                                sendbuff[pos] = 0;
                                pos +=2;
                                short filexuhao = packnum - filepacknum;
                                memcpy(sendbuff+pos,(char*)&filexuhao,2);
                                pos +=2;
                                memcpy(sendbuff+pos,imgfile,9);
                                pos +=9;
                                memcpy(sendbuff+pos,(char*)&zz,2);
                                pos +=2;
                                unsigned char* buffduobo = NULL;
                                unsigned char* buff141 = NULL;
                                int buffduobosize = fengzhuang(&buffduobo,sendbuff,pos+zz,(unsigned char*)ch,strr.size());
                                int buff141size = buff(buffduobo,buffduobosize,&buff141);
                                if(_sock.size()>0)
                                {   //发送包
                                    _sock.begin().value()->write((char*)buff141,buff141size);
                                }
                                memset(sendbuff,0,1024);
                            }
                            i++;
                            imagenum--;
                        }
                        else if(o == 2)
                        {
                            //发送第三个文件
                            QFile file(img3);
                            if(!file.open(QIODevice::ReadWrite)){
                                qDebug()<<"打开失败";
                            }
                            else{
                                qDebug()<<"打开成功";
                            }
                            short packnum = filepacknum;
                            short zz = 0;
                            while((zz=file.read((char*)(sendbuff+36),800)) == 0){
                                filepacknum -=1;
                                int pos =0;
                                memcpy(sendbuff+36+zz,(unsigned char*)&filepacknum,2);
                                sendbuff[0] = 0x09;
                                pos +=2;
                                time_t  time4 = time(NULL);
                                srand(time4);
                                int randomm= rand();
                                memcpy(sendbuff+pos,(char*)&randomm,4);
                                pos +=4;
                                sendbuff[pos] = 0;
                                pos +=1;
                                memcpy(sendbuff+pos,sysname,13);
                                pos +=13;
                                sendbuff[pos] = 0x05;
                                pos +=1;
                                sendbuff[pos] = 0;
                                pos +=2;
                                short filexuhao = packnum - filepacknum;
                                memcpy(sendbuff+pos,(char*)&filexuhao,2);
                                pos +=2;
                                memcpy(sendbuff+pos,imgfile,9);
                                pos +=9;
                                memcpy(sendbuff+pos,(char*)&zz,2);
                                pos +=2;
                                unsigned char* buffduobo = NULL;
                                unsigned char* buff141 = NULL;
                                int buffduobosize = fengzhuang(&buffduobo,sendbuff,pos+zz,(unsigned char*)ch,strr.size());
                                int buff141size = buff(buffduobo,buffduobosize,&buff141);
                                if(_sock.size()>0)
                                {   //发送包
                                    _sock.begin().value()->write((char*)buff141,buff141size);
                                }
                                memset(sendbuff,0,1024);
                            }
                            i++;
                            imagenum--;
                        }
                        else if(o ==3)
                        {
                            i = 0;
                            break;
                        }
                        //发送文件结束包
                        unsigned char* endfile = NULL;
                        packend(&endfile,o);
                        unsigned char* endfileduobo = NULL;
                        unsigned char* endfile141 = NULL;
                        int endfileduobosize = fengzhuang(&endfileduobo,endfile,40,(unsigned char*)ch,strr.size());
                        int endfile141size = buff(endfileduobo,endfileduobosize,&endfile141);
                        if(_sock.size()>0)
                        {   //发送文件数量包
                            _sock.begin().value()->write((char*)endfile141,endfile141size);
                        }
                    }
                    //发送完存数据库
                    emit sendSaveIdToSql(sendgwsid,*iter);
                    iter++;
                    idnum--;
                    emit sendProgressBarChange(idnum2-idnum);
                }

            }
        }
    }
    else if(str[6] == 0x14 && str[26] == 0x03){
        //解析网关回发时间戳包确认网关在线
        int seq = *(int*)(str+8);  //seq
        std::string userid = std::string((char*)str+13,3);
        unsigned long long long_buf =qFromBigEndian(*(unsigned long long*)(str+16));
        char ltos2[16] ={0};
        sprintf(ltos2,"%lld",long_buf);
        userid += std::string(ltos2);
        QString qstr = QString::fromStdString(userid);
        qDebug()<<"id是"<<qstr<<endl;
        short timelen = qToBigEndian(*(short*)(str+28));  //加密时间戳的长度
        qDebug()<<timelen<<endl;
        unsigned char* miwen = (unsigned char*)malloc(timelen);
        memcpy(miwen,str+30,timelen);
        QByteArray sendmiwen = QByteArray((const char*)miwen);
        qDebug()<<"接收密文:"<<endl;
        for (int i = 0; i < timelen; i++) {
            printf("%02x",*(miwen + i));
        }
        printf("\n");
        unsigned char* p_mingwen;
        int timelen2 = des3_api(DES_DECRYPT,miwen, timelen, &p_mingwen);
         int stoptime = *(int*)(p_mingwen);
        qDebug()<<"decrypted bin:  "<<stoptime<<endl;

        if(str[26] == 0x03){
            //查询数据库是否有此网关，没有就数据库增加 有就解密对比
            emit sendTimeToSql_UI(sendmiwen,timelen,qstr);
            emit sendWgPermission(qstr);
        }
        //停止定时器
        emit stopTimeFive();
        free(miwen);
        miwen = NULL;
    }
     else if(str[6] == 0x15){
        if(str[26] == 0x01){    //触发升级
            unsigned char* version = (unsigned char*)malloc(8);
            memcpy(version,str+28,8);
            std::string sversion = (char*)version;
            QString qsstr = QString::fromStdString(sversion);

            qDebug()<<"0X01版本号"<<qsstr<<endl;
            std::string userid = std::string((char*)str+13,3);
            unsigned long long long_buf =qFromBigEndian(*(unsigned long long*)(str+16));
            char ltos2[16] ={0};
            sprintf(ltos2,"%lld",long_buf);
            userid += std::string(ltos2);
            QString qstr = QString::fromStdString(userid);
            qDebug()<<"升级id是"<<qstr<<endl;
            unsigned char* gwid = (unsigned char*)userid.c_str();
            //封装包

            int id_size = userid.size();

            unsigned char gwup[90];
            memset(gwup,0,90);
            gwup[0] = 0x15;
            int number =0;
            memcpy(gwup+2,(char*)&number,4);
            gwup[6] =0x01;
            memcpy(gwup+2+4+1,sysname,NAME_LEN);
            gwup[20] =0x02;
            gwup[21] =0x01;
            int pack_content_size =2+4+1+NAME_LEN+2+8;

            unsigned char* pack_content = NULL;   //多播包封装内容
            unsigned char* send_pack = NULL;  //141封装内容
            int duobo_pack_content_size = 0;  //封装后大小
            duobo_pack_content_size = fengzhuang(&pack_content,gwup,pack_content_size,gwid,id_size);
            qDebug()<<"多播包大小"<<duobo_pack_content_size<<endl;
            int send_pack_size = buff(pack_content,duobo_pack_content_size,&send_pack); //封装141
            qDebug()<<"141包大小"<<send_pack_size<<endl;

            if(_sock.size()>0)
            {
                _sock.begin().value()->write((char*)send_pack,send_pack_size);
                qDebug()<<"发送截止时间消息"<<endl;
                free(send_pack);
                free(pack_content);
                free(version);
                version = NULL;
                pack_content = NULL;
                send_pack = NULL;
            }
            else{
                qDebug()<<"没有服务器在线"<<endl;
                free(send_pack);
                free(pack_content);
                free(version);
                version = NULL;
                pack_content = NULL;
                send_pack = NULL;
            }
        }
        else if(str[26] == 0x02){  //显示版本
            unsigned char* version = (unsigned char*)malloc(8);
            memcpy(version,str+28,8);
            std::string sversion = (char*)version;
            QString qsstr = QString::fromStdString(sversion);

            qDebug()<<"0X02版本号"<<qsstr<<endl;
            free(version);
            version = NULL;
        }
    }
    else if(str[6] ==0x14 && str[26] ==0xfe){
        if(str[27] == 0x01){
            qDebug()<<"消息给网关成功"<<endl;
        }
        else{
            qDebug()<<"消息给网关失败"<<endl;
        }
    }
}

//创建套接字，发送注册包 保存套接字
void widgetsocket::Createsocket(QString name,QString ip,int port)
{
    QTcpSocket * sock;
    sock = new QTcpSocket(this);
    sock->connectToHost(QHostAddress(ip),port);
    sock->waitForConnected(5000);
    connect(sock,sock->readyRead,this,&widgetsocket::dealbuffer);
    //connect(sock,SIGNAL(disconnected()),this,SLOT(reconnection()),Qt::QueuedConnection);
    qDebug()<<"state"<<sock->state()<<endl;

    if(sock->state() == 0)
    {
        emit sendStat(name,false);
        return;
    }
    //connect(sock,SIGNAL(error(QAbstractSocket::SocketError)),sock,SLOT(ReadError(QAbstractSocket::SocketError)));
        emit sendInitcombox(name);
        _sock.insert(name,sock);
       unsigned char b[40]={0};
       b[0] = 0x01;
       time_t  time1 = time(NULL);
       srand(time1);
       int suijima= rand();
       qDebug()<<"随机码是"<<suijima<<endl;
       suijima = qToBigEndian(suijima);
       qDebug()<<"2222222222222"<<endl;
       memcpy(b+1,(char*)&suijima,sizeof(suijima));
       short nlen=13;
       nlen = qToBigEndian(nlen);
       memcpy(b+1+sizeof(suijima),(char* )&nlen,SHORT_LEN);
       unsigned char* buffer=NULL;
       unsigned char* buffer2 =(unsigned char*)malloc(16);
       int len2 = create_md5((unsigned char*)sysname,NAME_LEN,buffer2,_miyue);

       memcpy((char*)b + 1 + sizeof(suijima) + SHORT_LEN,sysname,NAME_LEN);
       memcpy((char*)b + 1 + sizeof(suijima) + SHORT_LEN + NAME_LEN,buffer2,len2);
       int length = 1+sizeof(suijima) + SHORT_LEN + NAME_LEN + len2;
       int len = buff(b,length,&buffer);
       qDebug()<<len<<endl;
       qDebug()<<len2<<endl;
       sock->write((char*)buffer,len);
       qDebug()<<"发送注册消息"<<endl;
       emit sendStat(name,true);
}
//发送心跳包
void widgetsocket::heartbeat()
{
    QMap<QString,QTcpSocket*>::iterator it; //遍历socket  发送心跳
    if(_sock.size()>0){
        for(it = _sock.begin(); it != _sock.end(); ++it)
        {
            unsigned char b[20]={0};
            b[0] = 0x09;
            unsigned char* buffer=NULL;

            qDebug()<<"name大小"<<it.key().size()<<endl;
            std::string str = it.key().toStdString();
            unsigned char* sendname=(unsigned char*)str.c_str();
            int name_size = str.size();
            memcpy(b+1,(char*)&name_size,4);
            memcpy(b+5,sendname,name_size);
            int len = buff(b,5+name_size,&buffer);
            if(it.value()->write((char*)buffer,len) == -1)
            {
                emit sendReconnect(it.key());
                //reconnection(it.key());
                qDebug()<<it.key()<<"重连"<<endl;
            }
            emit sendStat(it.key(),true);
            qDebug()<<"发送完心跳"<<endl;
        }
    }
}
//发送重启服务器数据包
void widgetsocket::dealrestart(QString serverName)
{
    QMap<QString,QTcpSocket*>::iterator it;
    it = _sock.find(serverName);
    if(it != _sock.end())
    {
        qDebug()<<it.key()<<endl;
        QString str = "chongqi";
        it++;
    }
}
//服务器添加进数据库
void widgetsocket::dealAddServer(QString servername,QString serverip,int serverport)
{
    QTcpSocket * sock;
    sock = new QTcpSocket(this);
    sock->connectToHost(QHostAddress(serverip),serverport);
    sock->waitForConnected(5000);
    connect(sock,sock->readyRead,this,&widgetsocket::dealbuffer);
    //connect(sock,SIGNAL(disconnected()),this,SLOT(reconnection()),Qt::QueuedConnection);
    if(sock->state() == 0)
    {
        emit sendStat(servername,false);
        emit sendAddServerFail();
        return;
    }
    _sock.insert(servername,sock);
    emit sendInitcombox(servername);
       unsigned char b[40]={0};
       b[0] = 0x01;
       time_t  time1 = time(NULL);
       srand(time1);
       int suijima= rand();
       qDebug()<<"随机码是"<<suijima<<endl;
       suijima = qToBigEndian(suijima);
       qDebug()<<"2222222222222"<<endl;
       memcpy(b+1,(char*)&suijima,sizeof(suijima));
       short nlen=13;
       nlen = qToBigEndian(nlen);
       memcpy(b+1+sizeof(suijima),(char* )&nlen,SHORT_LEN);
       unsigned char* buffer=NULL;
       unsigned char* buffer2 =(unsigned char*)malloc(16);
       int len2 = create_md5((unsigned char*)sysname,NAME_LEN,buffer2,_miyue);

       memcpy((char*)b + 1 + sizeof(suijima) + SHORT_LEN,sysname,NAME_LEN);
       memcpy((char*)b + 1 + sizeof(suijima) + SHORT_LEN + NAME_LEN,buffer2,len2);
       int length = 1+sizeof(suijima) + SHORT_LEN + NAME_LEN + len2;
       int len = buff(b,length,&buffer);
       qDebug()<<len<<endl;
       qDebug()<<len2<<endl;
       sock->write((char*)buffer,len);
       qDebug()<<"发出去了吗？"<<endl;
       free(buffer2);
       buffer2 = NULL;
       emit sendStat(servername,true);
       emit sendAddServer(serverip,servername,serverport);
}

//
void widgetsocket::closesock()
{
    QMap<QString,QTcpSocket*>::iterator it;
    while(it != _sock.end())
    {
        //it.value()->close();
        _sock.erase(it);
        it++;
    }
}

//重连服务器
void widgetsocket::reconnection(QString servername,QString ip,int port)
{
    qDebug()<<"重连服务器"<<servername<<endl;
    QTcpSocket * sock;
    sock = new QTcpSocket(this);
    sock->connectToHost(QHostAddress(ip),port);
    sock->waitForConnected(5000);
    connect(sock,sock->readyRead,this,&widgetsocket::dealbuffer);
    //connect(sock,SIGNAL(disconnected()),this,SLOT(reconnection()),Qt::QueuedConnection);
    if(sock->state() == 0)
    {
        emit sendStat(servername,false);
        qDebug()<<"重连失败"<<endl;
        QMap<QString,QTcpSocket*>::iterator it =_sock.find(servername);
        if(it !=_sock.end() && it.value() != sock)
        {
            //delete it.value();
            _sock.erase(it);
           //_sock[servername] =sock;
        }
    }
    else{
        QMap<QString,QTcpSocket*>::iterator it =_sock.find(servername);
        if(it !=_sock.end() && it.value() != sock)
        {
            //delete it.value();
            _sock.erase(it);
            _sock.insert(servername,sock);
           //_sock[servername] =sock;
        }
        unsigned char b[40]={0};
        b[0] = 0x01;
        time_t  time1 = time(NULL);
        srand(time1);
        int suijima= rand();
        qDebug()<<"随机码是"<<suijima<<endl;
        suijima = qToBigEndian(suijima);
        qDebug()<<"2222222222222"<<endl;
        memcpy(b+1,(char*)&suijima,sizeof(suijima));
        short nlen=13;
        nlen = qToBigEndian(nlen);
        memcpy(b+1+sizeof(suijima),(char* )&nlen,SHORT_LEN);
        unsigned char* buffer=NULL;
        unsigned char* buffer2 =(unsigned char*)malloc(16);
        int len2 = create_md5((unsigned char*)sysname,NAME_LEN,buffer2,_miyue);

        memcpy((char*)b + 1 + sizeof(suijima) + SHORT_LEN,sysname,NAME_LEN);
        memcpy((char*)b + 1 + sizeof(suijima) + SHORT_LEN + NAME_LEN,buffer2,len2);
        int length = 1+sizeof(suijima) + SHORT_LEN + NAME_LEN + len2;
        int len = buff(b,length,&buffer);
        qDebug()<<len<<endl;
        qDebug()<<len2<<endl;
        sock->write((char*)buffer,len);
        qDebug()<<"发出去了吗？"<<endl;
        free(buffer2);
        buffer2 =NULL;
        emit sendStat(servername,true);
    }

}
//接收数据
void widgetsocket::dealbuffer()
{
    qDebug()<<"接收到消息了？"<<endl;
    QMap<QString,QTcpSocket*>::iterator it; //遍历socket
    if(_sock.size()>0){
        for(it = _sock.begin(); it != _sock.end(); ++it)
        {
            char str[1024] = {0};
            int size = it.value()->read(str,1024);
            if(it.value()->read(str,1024)==-1){
                _sock.erase(it);
            }
            if(str[0] = 0xaa)
            {
                int i = 0;
                while(size<5)            //读到包长
                {
                    if(i>5)
                    {
                        return;
                    }
                    char str2[1024];
                    int size2 = it.value()->read(str2,1024);
                    memcpy(str+size,str2,size2);
                    size += size2;
                    i++;
                }
                int datelen = *(int*)(str+1);      //解析包长
                datelen = qToBigEndian(datelen);
                unsigned char *buf  = (unsigned char *)&datelen;
                if((unsigned char)(str[5] + 0xaa + buf[0] + buf[1] + buf[2] + buf[3])){
                    qDebug()<<"141error"<<endl;
                    emit sendStat(it.key(),false);

                }
                else{
                qDebug()<<"接收数据长为："<<datelen<<endl;
                i =0;
                while(size<datelen)             //接收整个包
                {
                    if(i>5){
                        qDebug()<<"丢掉这个包"<<endl;
                        break;
                    }
                    char str2[datelen-size];
                    int size2 = it.value()->read(str2,datelen-size);
                    memcpy(str+size,str2,size2);
                    size += size2;
                    i++;
                }
                onpacket(str,size);
                qDebug()<<it.key()<<"实际接收大小为"<<size<<endl;
                }
            }
        }
    }

}

//服务器断开处理
/*void widgetsocket::sendDisconn()
{
    qDebug()<<"服务器断开"<<endl;
    QMap<QString,QTcpSocket*>::iterator it; //遍历socket
    for(it = _sock.begin(); it != _sock.end(); ++it)
    {
        if(it.value()->state()==0)
        {

        }
    }

}*/
void widgetsocket::sendInfoToServer(QString wg_name, bool radiobtn, QString serverall, QString obj, QString type, QString dateedit, QString houre, QString title, QString edit, QString link, QString linkplace, QString imgAdress,int user_id,int gwsid,QString imgAdress2,QString imgAdress3)
{
    //封装数据体 0x10 通知发送包
    //判断发送 对象和发送内容
    initdate();
    img1 = imgAdress;
    img2 = imgAdress2;
    img3 = imgAdress3;
    sendobj = obj;
    sendtype = type;
    sendtitle = title;
    sendedit = edit;
    sendlink = link;
    sendlinkplace = linkplace;
    sendgwsid = gwsid;
    //QDateTime current_date_time =QDateTime::currentDateTime();
    //QString current_date =current_date_time.toString("yyyy-MM-dd HH:mm:ss");
    if(radiobtn) //选择所有服务器  遍历socket发送
    {
        servernum =_sock.size();
        //给服务器发送id请求包
        dealAllServerIds(obj);
        qDebug()<<"年后写的传所有服务器"<<endl;
    }
    else if(serverall != NULL&&serverall != "服务器列表")  //选择单个服务器
    {
         QMap<QString,QTcpSocket*>::iterator it = _sock.find(serverall);
         if(it != _sock.end())
         {
             qDebug()<<"找到sock发送"<<serverall<<"发送对象"<<obj<<endl;
             unsigned char b[10]={0};
             b[0] = 0x10;
             unsigned char* buffer=NULL;
             if(obj == "所有客户"){
                 b[1] = 0x01;
                 qDebug()<<obj<<endl;
             }
             else if(obj == "所有管理员"){
                 b[1] = 0x02;
                 qDebug()<<obj<<endl;
             }
             int len = buff(b,2,&buffer);
             if(it.value()->write((char*)buffer,len) == -1)
             {
                 qDebug()<<it.key()<<"获取id发送失败"<<endl;
             }
         }
        qDebug()<<"单个服务器"<<endl;
    }
    else if(wg_name !=NULL)  //发送给单个网关
    {
        qDebug()<<"单个网关发送"<<endl;
        //标题长度
        short titlelen = sendtitle.size();
        //内容长度
        short editlen = sendedit.size();
        std::string str = wg_name.toStdString();
        const char* ch = str.c_str();
        unsigned char info[26+titlelen+editlen] ={0};
        //发送通知提醒包;
        info[0] = 0x14;
        memcpy(info+7,sysname,13);

        info[20] = 0x04;
        if(sendtype =="发送到管理员"){
            info[21] =0x01;
        }
        else if(sendtype =="发送到普通用户"){
            info[21] =0x02;
        }
        else if(sendtype =="发送到所有用户"){
            info[21] =0x03;
        }
        memcpy(info+22,(unsigned char*)&titlelen,2);
        //标题
        std::string title = sendtitle.toStdString();
        memcpy(info+24,(unsigned char*)title.c_str(),titlelen);
        //内容
        memcpy(info+24+titlelen,(unsigned char*)&editlen,2);
        std::string edit = sendedit.toStdString();
        memcpy(info+26+titlelen,(unsigned char*)edit.c_str(),editlen);

        unsigned char* infoduobo = NULL;
        unsigned char* info141=NULL;
        //int size = packagedate(&info);   //封装通知消息发送
        int duobosize = fengzhuang(&infoduobo,info,20,(unsigned char*)ch,str.size());
        int size141 = buff(infoduobo,duobosize,&info141);
         unsigned char* sendbuff2 = (unsigned char*)calloc(1024,sizeof(unsigned char));
        if(_sock.size()>0)
        {   //发送通知包
            _sock.begin().value()->write((char*)info141,size141);
        }    
        //传文件
        if(img1.isEmpty() == false){
            //文件数量包发送
            unsigned char* beginfile = NULL;
            int o = packbegin(&beginfile,0);
            unsigned char* beginfileduobo = NULL;
            unsigned char* beginfile141 = NULL;
            int beginfileduobosize = fengzhuang(&beginfileduobo,beginfile,40,(unsigned char*)ch,str.size());
            int beginfile141size = buff(beginfileduobo,beginfileduobosize,&beginfile141);
            if(_sock.size()>0)
            {   //发送文件数量包
                _sock.begin().value()->write((char*)beginfile141,beginfile141size);
            }

            //发送第一张图片
            QFile file(img1);
            if(!file.open(QIODevice::ReadWrite)){
                qDebug()<<"打开失败";
            }
            else{
                qDebug()<<"打开成功";
            }
            short packnum = filepacknum;
            short zz = 0;
            memset(sendbuff2,0,1024);
            while((zz=file.read((char*)(sendbuff2+36),800)) == 0){
                filepacknum -=1;
                int pos =0;
                memcpy(sendbuff2+36+zz,(unsigned char*)&filepacknum,2);
                sendbuff2[0] = 0x09;
                pos +=2;
                time_t  time4 = time(NULL);
                srand(time4);
                int randomm= rand();
                memcpy(sendbuff2+pos,(char*)&randomm,4);
                pos +=4;
                sendbuff2[pos] = 0;
                pos +=1;
                memcpy(sendbuff2+pos,sysname,13);
                pos +=13;
                sendbuff2[pos] = 0x05;
                pos +=1;
                sendbuff2[pos] = 0;
                pos +=2;
                short filexuhao = packnum - filepacknum;
                memcpy(sendbuff2+pos,(char*)&filexuhao,2);
                pos +=2;
                memcpy(sendbuff2+pos,imgfile,9);
                pos +=9;
                memcpy(sendbuff2+pos,(char*)&zz,2);
                pos +=2;
                unsigned char* buffduobo = NULL;
                unsigned char* buff141 = NULL;
                int buffduobosize = fengzhuang(&buffduobo,sendbuff2,pos+zz,(unsigned char*)ch,str.size());
                int buff141size = buff(buffduobo,buffduobosize,&buff141);
                if(_sock.size()>0)
                {   //发送包
                    _sock.begin().value()->write((char*)buff141,buff141size);
                }

            }
            //文件结束包发送
            unsigned char* endfile = NULL;
            packend(&endfile,0);
            unsigned char* endfileduobo = NULL;
            unsigned char* endfile141 = NULL;
            int endfileduobosize = fengzhuang(&endfileduobo,endfile,40,(unsigned char*)ch,str.size());
            int endfile141size = buff(endfileduobo,endfileduobosize,&endfile141);
            if(_sock.size()>0)
            {   //发送文件结束包
                _sock.begin().value()->write((char*)endfile141,endfile141size);
            }

        }
        if(img2.isEmpty() == false){
            //发送第二张图片
            //文件数量包发送
            unsigned char* beginfile = NULL;
            int o = packbegin(&beginfile,1);
            unsigned char* beginfileduobo = NULL;
            unsigned char* beginfile141 = NULL;
            int beginfileduobosize = fengzhuang(&beginfileduobo,beginfile,40,(unsigned char*)ch,str.size());
            int beginfile141size = buff(beginfileduobo,beginfileduobosize,&beginfile141);
            if(_sock.size()>0)
            {   //发送文件数量包
                _sock.begin().value()->write((char*)beginfile141,beginfile141size);
            }

            //发送第一张图片
            QFile file(img2);
            if(!file.open(QIODevice::ReadWrite)){
                qDebug()<<"打开失败";
            }
            else{
                qDebug()<<"打开成功";
            }
            short packnum = filepacknum;
            short zz = 0;
            memset(sendbuff2,0,1024);
            while((zz=file.read((char*)(sendbuff2+36),800)) == 0){
                filepacknum -=1;
                int pos =0;
                memcpy(sendbuff2+36+zz,(unsigned char*)&filepacknum,2);
                sendbuff2[0] = 0x09;
                pos +=2;
                time_t  time4 = time(NULL);
                srand(time4);
                int randomm= rand();
                memcpy(sendbuff2+pos,(char*)&randomm,4);
                pos +=4;
                sendbuff2[pos] = 0;
                pos +=1;
                memcpy(sendbuff2+pos,sysname,13);
                pos +=13;
                sendbuff2[pos] = 0x05;
                pos +=1;
                sendbuff2[pos] = 0;
                pos +=2;
                short filexuhao = packnum - filepacknum;
                memcpy(sendbuff2+pos,(char*)&filexuhao,2);
                pos +=2;
                memcpy(sendbuff2+pos,imgfile,9);
                pos +=9;
                memcpy(sendbuff2+pos,(char*)&zz,2);
                pos +=2;
                unsigned char* buffduobo = NULL;
                unsigned char* buff141 = NULL;
                int buffduobosize = fengzhuang(&buffduobo,sendbuff2,pos+zz,(unsigned char*)ch,str.size());
                int buff141size = buff(buffduobo,buffduobosize,&buff141);
                if(_sock.size()>0)
                {   //发送包
                    _sock.begin().value()->write((char*)buff141,buff141size);
                }

            }
            //文件结束包发送
            unsigned char* endfile = NULL;
            packend(&endfile,1);
            unsigned char* endfileduobo = NULL;
            unsigned char* endfile141 = NULL;
            int endfileduobosize = fengzhuang(&endfileduobo,endfile,40,(unsigned char*)ch,str.size());
            int endfile141size = buff(endfileduobo,endfileduobosize,&endfile141);
            if(_sock.size()>0)
            {   //发送文件结束包
                _sock.begin().value()->write((char*)endfile141,endfile141size);
            }

        }
        if(img3.isEmpty() == false){
            //发送第三张图片
            //文件数量包发送
            unsigned char* beginfile = NULL;
            int o = packbegin(&beginfile,2);
            unsigned char* beginfileduobo = NULL;
            unsigned char* beginfile141 = NULL;
            int beginfileduobosize = fengzhuang(&beginfileduobo,beginfile,40,(unsigned char*)ch,str.size());
            int beginfile141size = buff(beginfileduobo,beginfileduobosize,&beginfile141);
            if(_sock.size()>0)
            {   //发送文件数量包
                _sock.begin().value()->write((char*)beginfile141,beginfile141size);
            }

            //发送第一张图片
            QFile file(img3);
            if(!file.open(QIODevice::ReadWrite)){
                qDebug()<<"打开失败";
            }
            else{
                qDebug()<<"打开成功";
            }
            short packnum = filepacknum;
            short zz = 0;
            memset(sendbuff2,0,1024);
            while((zz=file.read((char*)(sendbuff2+36),800)) == 0){
                filepacknum -=1;
                int pos =0;
                memcpy(sendbuff2+36+zz,(unsigned char*)&filepacknum,2);
                sendbuff2[0] = 0x09;
                pos +=2;
                time_t  time4 = time(NULL);
                srand(time4);
                int randomm= rand();
                memcpy(sendbuff2+pos,(char*)&randomm,4);
                pos +=4;
                sendbuff2[pos] = 0;
                pos +=1;
                memcpy(sendbuff2+pos,sysname,13);
                pos +=13;
                sendbuff2[pos] = 0x05;
                pos +=1;
                sendbuff2[pos] = 0;
                pos +=2;
                short filexuhao = packnum - filepacknum;
                memcpy(sendbuff2+pos,(char*)&filexuhao,2);
                pos +=2;
                memcpy(sendbuff2+pos,imgfile,9);
                pos +=9;
                memcpy(sendbuff2+pos,(char*)&zz,2);
                pos +=2;
                unsigned char* buffduobo = NULL;
                unsigned char* buff141 = NULL;
                int buffduobosize = fengzhuang(&buffduobo,sendbuff2,pos+zz,(unsigned char*)ch,str.size());
                int buff141size = buff(buffduobo,buffduobosize,&buff141);
                if(_sock.size()>0)
                {   //发送包
                    _sock.begin().value()->write((char*)buff141,buff141size);
                }

            }
            //文件结束包发送
            unsigned char* endfile = NULL;
            packend(&endfile,2);
            unsigned char* endfileduobo = NULL;
            unsigned char* endfile141 = NULL;
            int endfileduobosize = fengzhuang(&endfileduobo,endfile,40,(unsigned char*)ch,str.size());
            int endfile141size = buff(endfileduobo,endfileduobosize,&endfile141);
            if(_sock.size()>0)
            {   //发送文件结束包
                _sock.begin().value()->write((char*)endfile141,endfile141size);
            }
        }

    }
    qDebug()<<"发送给服务器的通知消息"<<endl;
}

void widgetsocket::dealInfoTimer(int userid,int num,bool radiobtn,QString timeend,QString title,QString content,QString link,QString linkplace,QString imgaddress,QString intervaltime,QString lasttime,QString wg_name,QString serverall,QString obj,QString type,QString timebegin)
{
    if(radiobtn) //选择所有服务器  遍历socket发送
    {
        qDebug()<<"年后写的传所有服务器"<<endl;
    }
    else if(serverall != NULL)  //选择单个服务器
    {
        qDebug()<<"稳稳的等庚老哥  单个服务器"<<endl;
    }
    else if(wg_name !=NULL)  //发送给单个网关
    {
        qDebug()<<"庚老哥说oj8k 单个网关发送"<<endl;
    }
    qDebug()<<"发送给服务器的通知消息"<<endl;
}

void widgetsocket::sendStopTimeToGw(int len,QByteArray miwen,QString id){
    //发送截止时间给网关
    //多播包封装
    unsigned char* str = (unsigned char*)miwen.data();
    unsigned char stoptime[90];
    memset(stoptime,0,90);
    stoptime[0] = 0x14;
    int number =0;
    memcpy(stoptime+2,(char*)&number,4);
    stoptime[6] =0x01;
    memcpy(stoptime+2+4+1,sysname,NAME_LEN);
    stoptime[20] =0x02;
    stoptime[21] ='b';

    memcpy(stoptime+22,(char*)&len,2);
    memcpy(stoptime+24,str,len);

    std::string charid = id.toStdString();
    unsigned char* gwid = (unsigned char*)charid.c_str();

    int id_size = id.size();
    int pack_content_size = 2+4+1+NAME_LEN+4+len;
    unsigned char* pack_content = NULL;   //多播包封装内容
    unsigned char* send_pack = NULL;  //141封装内容
    int duobo_pack_content_size = 0;  //封装后大小
    duobo_pack_content_size = fengzhuang(&pack_content,stoptime,pack_content_size,gwid,id_size);
    qDebug()<<"多播包大小"<<duobo_pack_content_size<<endl;
    int send_pack_size = buff(pack_content,duobo_pack_content_size,&send_pack); //封装141
    qDebug()<<"141包大小"<<send_pack_size<<endl;

    if(_sock.size()>0)
    {
        _sock.begin().value()->write((char*)send_pack,send_pack_size);
        qDebug()<<"发送截止时间消息"<<endl;
        free(send_pack);
        free(pack_content);
        pack_content = NULL;
        send_pack = NULL;
    }
    else{
        qDebug()<<"没有服务器在线"<<endl;
        free(send_pack);
        free(pack_content);
        pack_content = NULL;
        send_pack = NULL;
    }

}

void widgetsocket::dealSelectWg(QString id)
{
    //发送设备查询信息，等待网关回应消息
    qDebug()<<"网关查询"<<id<<endl;
    std::string str = id.toStdString();
    std::string gwm = "GWM";
    std::string gwn = "GWN";
    std::string gwa = "GWA";
    gwm+=str;
    gwn+=str;
    gwa+=str;
    unsigned char* gwmid=(unsigned char*)gwm.c_str();
    unsigned char* gwnid=(unsigned char*)gwn.c_str();
    unsigned char* gwaid=(unsigned char*)gwa.c_str();

    int id_size = id.size();
    qDebug()<<str.c_str()<<id_size<<endl;
    //多播包封包发送
    unsigned char b[30]={0};
    b[0] = 0x14;
    int number = 0;
    memcpy(b + 2,(char*)&number,4);
    b[6] =0x01;
    memcpy(b+2+4+1,sysname,NAME_LEN);
    b[20] = 0x01;
    int pack_content_size =2+sizeof(number)+1+NAME_LEN+1; //网关查询包大小
    unsigned char* pack_content = NULL;   //多播包封装内容
    unsigned char* send_pack = NULL;  //141封装内容
    int duobo_pack_content_size = 0;  //封装后大小
    duobo_pack_content_size = fengzhuang(&pack_content,b,pack_content_size,gwmid,id_size+3);
     qDebug()<<"多播包大小"<<duobo_pack_content_size<<endl;
    int send_pack_size = buff(pack_content,duobo_pack_content_size,&send_pack); //封装141
    qDebug()<<"141包大小"<<send_pack_size<<endl;

    unsigned char* pack_content2 = NULL;   //多播包封装内容
    unsigned char* send_pack2 = NULL;  //141封装内容
    int duobo_pack_content_size2 = 0;  //封装后大小
    duobo_pack_content_size2 = fengzhuang(&pack_content2,b,pack_content_size,gwnid,id_size+3);
     qDebug()<<"多播包大小"<<duobo_pack_content_size2<<endl;
    int send_pack_size2 = buff(pack_content2,duobo_pack_content_size2,&send_pack2); //封装141
    qDebug()<<"141包大小"<<send_pack_size2<<endl;

    unsigned char* pack_content3 = NULL;   //多播包封装内容
    unsigned char* send_pack3 = NULL;  //141封装内容
    int duobo_pack_content_size3 = 0;  //封装后大小
    duobo_pack_content_size3 = fengzhuang(&pack_content3,b,pack_content_size,gwaid,id_size+3);
     qDebug()<<"多播包大小"<<duobo_pack_content_size3<<endl;
    int send_pack_size3 = buff(pack_content3,duobo_pack_content_size3,&send_pack3); //封装141
    qDebug()<<"141包大小"<<send_pack_size3<<endl;
    if(_sock.size()>0)
    {
        _sock.begin().value()->write((char*)send_pack,send_pack_size);
        _sock.begin().value()->write((char*)send_pack2,send_pack_size2);
        _sock.begin().value()->write((char*)send_pack3,send_pack_size3);
        qDebug()<<"发送设备查询消息"<<endl;
        free(send_pack);
        free(pack_content);
        free(send_pack2);
        free(pack_content2);
        free(send_pack3);
        free(pack_content3);
        pack_content = NULL;
        send_pack = NULL;
        pack_content2 = NULL;
        send_pack2 = NULL;
        pack_content3 = NULL;
        send_pack3 = NULL;
    }
    else{
        qDebug()<<"没有服务器在线"<<endl;
        free(send_pack);
        free(pack_content);
        free(send_pack2);
        free(pack_content2);
        free(send_pack3);
        free(pack_content3);
        pack_content = NULL;
        send_pack = NULL;
        pack_content2 = NULL;
        send_pack2 = NULL;
        pack_content3 = NULL;
        send_pack3 = NULL;
    }
    //emit sendWgPermission();
}

void widgetsocket::deviceNums()
{
    int wg_num = 0;
    int light_num = 0;
    int socket_num = 0;
    int curtain_num = 0;
    int infrared_num = 0;
    wg_num = wg_list.size();
    light_num = light_list.size();
    socket_num = socket_list.size();
    curtain_num = curtain_list.size();
    infrared_num = infrared_list.size();
    emit sendDeviceNums(wg_num,light_num,socket_num,curtain_num,infrared_num);
}
void widgetsocket::dealDeviceInfo(int i)
{
    if(i = 0)
    {
        emit sendDeviceInfo(wg_list);
    }
    else if(i = 1)
    {
        emit sendDeviceInfo(light_list);
    }
    else if(i = 2)
    {
        emit sendDeviceInfo(socket_list);
    }
    else if(i = 3)
    {
        emit sendDeviceInfo(curtain_list);
    }
    else if(i = 4)
    {
        emit sendDeviceInfo(infrared_list);
    }
}

void widgetsocket::dealClearMap()
{
    qDebug()<<"map 清除"<<endl;
    wg_list.clear();          //网关清除
    light_list.clear();       //开关清除
    socket_list.clear();      //插座清除
    curtain_list.clear();     //窗帘清除
    infrared_list.clear();    //红外清除
}

void widgetsocket::dealAllServerIds(QString obj)
{
    QMap<QString,QTcpSocket*>::iterator it = _sock.begin(); //拿到socket  发送id请求
    int num = _sock.size() - servernum;
    it += num;
    if(it!=_sock.end()){
            if(it.value()->state() == 0){
                servernum--;
            }
            else{
                unsigned char b[10]={0};
                b[0] = 0x10;
                unsigned char* buffer=NULL;
                if(obj == "所有客户"){
                    b[1] = 0x01;
                    qDebug()<<obj<<endl;
                }
                else if(obj == "所有管理员"){
                    b[1] = 0x02;
                    qDebug()<<obj<<endl;
                }
                int len = buff(b,2,&buffer);
                if(it.value()->write((char*)buffer,len) == -1)
                {
                    servernum--;
                    qDebug()<<it.key()<<"获取id发送失败"<<endl;
                }
                else{
                    servernum--;
                }
            }
    }
}
//网关单个提醒更新
void widgetsocket::dealWgUpdate(QString wgname, QString servername, bool allserver, QString updateadress,int userid){
    initWgUpdate();
    servernum2 = _sock.size();
    //file_path = filepath;
    //update_id = updateid;
    //文件名封装
    /*char updatefilename[9] ={0};
    time_t time2 = time(NULL);
    int utc =(int)time2;
    srand(time2);
    int updatesuijima = rand();
    char updatetype = 0x02;
    memcpy(updatefilename,(char*)&utc,4);
    memcpy(updatefilename+4,(char*)&updatesuijima,4);
    memcpy(updatefilename+8,(char*)&updatetype,1);
    memcpy(updatename,updatefilename,9);

    if(allserver) //选择所有服务器  遍历socket发送
    {
        servernum2 =_sock.size();
        //给服务器发送id请求包
        dealAllServerUpdate();
    }
    //选择单个服务器
    else if(servername !=NULL && servername !="服务器列表"){
        QMap<QString,QTcpSocket*>::iterator it = _sock.find(servername);
        if(it != _sock.end())
        {
            qDebug()<<"找到sock发送"<<servername<<endl;
            unsigned char b[10]={0};
            b[0] = 0x10;
            unsigned char* buffer=NULL;
            b[1] = 0x02;
            int len = buff(b,2,&buffer);
            if(it.value()->write((char*)buffer,len) == -1)
            {
                qDebug()<<it.key()<<"获取id发送失败"<<endl;
            }
        }
       qDebug()<<"单个服务器"<<endl;
    }*/
    //单个网关
        if(wgname != NULL){
        std::string strr = wgname.toStdString();
        unsigned char* sendid = (unsigned char*)strr.c_str();
        int id_size = strr.size();

        unsigned char gwup[90];
        memset(gwup,0,90);
        gwup[0] = 0x15;
        int number =0;
        memcpy(gwup+2,(char*)&number,4);
        gwup[6] =0x01;
        memcpy(gwup+2+4+1,sysname,NAME_LEN);
        gwup[20] =0x01;
        gwup[21] =0x01;
        int pack_content_size =2+4+1+NAME_LEN+2;

        unsigned char* pack_content = NULL;   //多播包封装内容
        unsigned char* send_pack = NULL;  //141封装内容
        int duobo_pack_content_size = 0;  //封装后大小
        duobo_pack_content_size = fengzhuang(&pack_content,gwup,pack_content_size,sendid,id_size);
        qDebug()<<"多播包大小"<<duobo_pack_content_size<<endl;
        int send_pack_size = buff(pack_content,duobo_pack_content_size,&send_pack); //封装141
        qDebug()<<"141包大小"<<send_pack_size<<endl;

        if(_sock.size()>0)
        {
            _sock.begin().value()->write((char*)send_pack,send_pack_size);
            qDebug()<<"发送网关更新"<<endl;
            free(send_pack);
            free(pack_content);
            pack_content = NULL;
            send_pack = NULL;
        }
        else{
            qDebug()<<"没有服务器在线"<<endl;
            free(send_pack);
            free(pack_content);
            pack_content = NULL;
            send_pack = NULL;
        }

        /*const char* ch = strr.c_str();
        qDebug()<<"发送id"<<ch<<endl;
        unsigned char* beginfile = NULL;
        //wgFilebegin(&beginfile);
        unsigned char* beginfileduobo = NULL;
        unsigned char* beginfile141 = NULL;
        int beginfileduobosize = fengzhuang(&beginfileduobo,beginfile,40,(unsigned char*)ch,strr.size());
        int beginfile141size = buff(beginfileduobo,beginfileduobosize,&beginfile141);
        if(_sock.size()>0)
        {   //发送文件数量包
            _sock.begin().value()->write((char*)beginfile141,beginfile141size);
        }
        unsigned char* sendbuff = (unsigned char*)calloc(1024,sizeof(unsigned char));
        //发送文件
        QFile file(file_path);
        if(!file.open(QIODevice::ReadWrite)){
            qDebug()<<"打开失败";
        }
        else{
            qDebug()<<"打开成功";
        }
        short packnum = update_filepacknum;
        short zz = 0;
        while((zz=file.read((char*)(sendbuff+36),800)) == 0){
            update_filepacknum -=1;
            int pos =0;
            memcpy(sendbuff+36+zz,(unsigned char*)&update_filepacknum,2);
            sendbuff[0] = 0x09;
            pos +=2;
            time_t  time4 = time(NULL);
            srand(time4);
            int randomm= rand();
            memcpy(sendbuff+pos,(char*)&randomm,4);
            pos +=4;
            sendbuff[pos] = 0;
            pos +=1;
            memcpy(sendbuff+pos,sysname,13);
            pos +=13;
            sendbuff[pos] = 0x05;
            pos +=1;
            sendbuff[pos] = 0;
            pos +=2;
            short filexuhao = packnum - update_filepacknum;
            memcpy(sendbuff+pos,(char*)&filexuhao,2);
            pos +=2;
            memcpy(sendbuff+pos,updatename,9);
            pos +=9;
            memcpy(sendbuff+pos,(char*)&zz,2);
            pos +=2;
            unsigned char* buffduobo = NULL;
            unsigned char* buff141 = NULL;
            int buffduobosize = fengzhuang(&buffduobo,sendbuff,pos+zz,(unsigned char*)ch,strr.size());
            int buff141size = buff(buffduobo,buffduobosize,&buff141);
            if(_sock.size()>0)
            {   //发送包
                _sock.begin().value()->write((char*)buff141,buff141size);
            }
            memset(sendbuff,0,1024);
        }
        //发送文件结束包
        unsigned char* endfile = NULL;
        wgFileend(&endfile);
        unsigned char* endfileduobo = NULL;
        unsigned char* endfile141 = NULL;
        int endfileduobosize = fengzhuang(&endfileduobo,endfile,40,(unsigned char*)ch,strr.size());
        int endfile141size = buff(endfileduobo,endfileduobosize,&endfile141);
        if(_sock.size()>0)
        {   //发送文件数量包
            _sock.begin().value()->write((char*)endfile141,endfile141size);
        }*/
    }
}
void widgetsocket::dealAllServerUpdate(){
    QMap<QString,QTcpSocket*>::iterator it = _sock.begin(); //拿到socket  发送id请求
    int num = _sock.size() - servernum2;
    it += num;
    if(it !=_sock.end()){
            if(it.value()->state() == 0){
                servernum2--;
            }
            else{
                unsigned char b[10]={0};
                b[0] = 0x10;
                unsigned char* buffer=NULL;
                b[1] = 0x02;
                int len = buff(b,2,&buffer);
                if(it.value()->write((char*)buffer,len) == -1)
                {
                    servernum2--;
                    qDebug()<<it.key()<<"获取id发送失败"<<endl;
                }
                else{
                    servernum2--;
                }
            }
    }
}
