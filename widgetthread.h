#ifndef WIDGETTHREAD_H
#define WIDGETTHREAD_H

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



class widgetthread : public QObject
{
    Q_OBJECT
public:
    explicit widgetthread(QObject *parent = 0);
    //void setFlag(bool flag = true);


signals:
    //void connsignal(int,QString,QString,int,int,int,QString);
    //void sendupdate_tab2(int,QString,QString,int,int,int,int,double);
    //void sendupdate_tab2_to_tab(int,int);
    //void sendChangeTimeTotab(QString,int);

public slots:
    //void widgetconn(QString,int);
private:
    //bool isStop;
};

#endif // WIDGETTHREAD_H
