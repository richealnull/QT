#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QHostAddress>
#include <QMap>

class workthread : public QObject
{
    Q_OBJECT
public:
    explicit workthread(QObject *parent = 0);

signals:
    void loginsignal(QString,QString,int,int,int,int,QString,int);



public slots:
    //线程处理登录验证
    void login(QString,QString);

private:
};

#endif // WORKTHREAD_H
