#ifndef UPDATE_LIST_H
#define UPDATE_LIST_H

#include <QWidget>

namespace Ui {
class update_list;
}

class update_list : public QWidget
{
    Q_OBJECT

public:
    explicit update_list(QWidget *parent = 0);
    int nCurScroller = 0; //翻页时滑动条位置
    int pageValue = 10;  //一页显示条数
    ~update_list();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();
    void on_pushButton_4_clicked();

signals:
    void backToWgUpdata();
    void showUpWgList();
public slots:
    void showUpList();
private:
    Ui::update_list *ui;
};

#endif // UPDATE_LIST_H
