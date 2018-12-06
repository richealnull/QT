#ifndef SUPERMANAGE_H
#define SUPERMANAGE_H

#include <QWidget>
#include "setmanage.h"
#include "addtime.h"

namespace Ui {
class supermanage;
}

class supermanage : public QWidget
{
    Q_OBJECT

public:
    explicit supermanage(QWidget *parent = 0);
    ~supermanage();

signals:
    void superManage_backSingle();

private slots:
    void on_Button_back_clicked();

    void on_Button_ok_clicked();

    void on_pushButton_clicked();

private:
    Ui::supermanage *ui;
    addtime set_time;
    setManage set_ui;
};

#endif // SUPERMANAGE_H
