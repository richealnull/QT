#ifndef TAB3_WIDGET_H
#define TAB3_WIDGET_H

#include <QWidget>

namespace Ui {
class tab3_widget;
}

class tab3_widget : public QWidget
{
    Q_OBJECT

public:
    explicit tab3_widget(QWidget *parent = 0);
    ~tab3_widget();

signals:
    void backto_tab3(); //界面返回信号
    //通知信息返回
    void sendbufToThis(QString,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);


public slots:

    void on_Button_back_clicked();

    void on_Button_send_clicked();

    void on_Button_img_clicked();

    void dealInfobtn(QString,bool,QString,QString,QString);

private slots:
    void on_Button_img2_clicked();

    void on_Button_img3_clicked();

private:
    Ui::tab3_widget *ui;
    QString wg_name;
    bool radiobtn;
    QString serverall;
    QString obj;
    QString type;
    QString imgAdress;
    QString imgAdress2;
    QString imgAdress3;
};

#endif // TAB3_WIDGET_H
