#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QThread>
#include <QLineEdit>
#include <QString>
#include <QKeyEvent>
#include "supermanage.h"
#include "tabwidget.h"
#include "workthread.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    //virtual void QLineEdit::keyPressEvent(QKeyEvent *event);
    ~Login();

signals:
    void startlogin(QString,QString); //启动验证

private slots:
    void on_ButtonManage_clicked();
    void on_ButtonLogin_clicked();
     void loginslote(QString,QString,int,int,int,int,QString,int);//登录验证

private:
    Ui::Login *ui;
    supermanage super;
    TabWidget tab;
    workthread *myT;//线程对象
    QThread *thread;

};

#endif // LOGIN_H
