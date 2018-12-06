#ifndef SETMANAGE_H
#define SETMANAGE_H

#include <QWidget>
#include <string>

namespace Ui {
class setManage;
}

class setManage : public QWidget
{
    Q_OBJECT

public:
    explicit setManage(QWidget *parent = 0);
    ~setManage();
signals:
    void setManage_backSingle();
    void toLogin_backSingle();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::setManage *ui;
};

#endif // SETMANAGE_H
