#ifndef SENDLIST_H
#define SENDLIST_H

#include <QWidget>

namespace Ui {
class sendlist;
}

class sendlist : public QWidget
{
    Q_OBJECT

public:
    explicit sendlist(QWidget *parent = 0);
    ~sendlist();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

public slots:
    void showlist(QString,QString,int);
    void dealclear();

private:
    Ui::sendlist *ui;
    int nCurScroller = 0; //翻页时滑动条位置
    int pageValue = 10;  //一页显示条数
};

#endif // SENDLIST_H
