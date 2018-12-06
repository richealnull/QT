#ifndef DEVICE_SHOW_H
#define DEVICE_SHOW_H

#include <QWidget>
#include <QMetaType>
#include <QMap>

namespace Ui {
class device_show;
}

class device_show : public QWidget
{
    Q_OBJECT

public:
    explicit device_show(QWidget *parent = 0);
    ~device_show();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

public slots:
    void deviceSet(QMap<QString,unsigned char*>);
private:
    Ui::device_show *ui;
    int nCurScroller = 0; //翻页时滑动条位置
    int pageValue = 10;  //一页显示条数
};

#endif // DEVICE_SHOW_H
