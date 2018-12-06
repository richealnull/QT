#ifndef UPDATE_WGLIST_H
#define UPDATE_WGLIST_H

#include <QWidget>

namespace Ui {
class update_wglist;
}

class update_wglist : public QWidget
{
    Q_OBJECT

public:
    explicit update_wglist(QWidget *parent = 0);
    int nCurScroller = 0; //翻页时滑动条位置
    int pageValue = 10;  //一页显示条数
    ~update_wglist();

signals:

public slots:

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::update_wglist *ui;
};

#endif // UPDATE_WGLIST_H
