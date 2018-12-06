#ifndef ROBOT_UPDATA_H
#define ROBOT_UPDATA_H

#include <QWidget>

namespace Ui {
class robot_updata;
}

class robot_updata : public QWidget
{
    Q_OBJECT

public:
    explicit robot_updata(QWidget *parent = 0);
    ~robot_updata();

signals:
    void robot_to_Tabwidget_signal();

private slots:
    void on_Button_back_clicked();

    void on_Button_switch_bag_clicked();

    void on_Button_updata_clicked();

private:
    Ui::robot_updata *ui;
};

#endif // ROBOT_UPDATA_H
