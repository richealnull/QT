#ifndef FINDINFO_H
#define FINDINFO_H

#include <QWidget>

namespace Ui {
class findInfo;
}

class findInfo : public QWidget
{
    Q_OBJECT

public:
    explicit findInfo(QWidget *parent = 0);
    ~findInfo();
signals:
    void findinfo_to_Tabwidget_signal();
    void sengwidslist(QString);

public slots:
    //初始化表
    void initInfo(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    //更新表
    void updateInfo(int,int,bool,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int,int);
private slots:
    void on_pushButton_clicked();

    void on_Button_up_clicked();

    void on_Button_down_clicked();

    void on_Button_look_clicked();

private:
    Ui::findInfo *ui;
    int nCurScroller = 0; //翻页时滑动条位置
    int pageValue = 10;  //一页显示条数
};

#endif // FINDINFO_H
