#ifndef EQUIPMENT_LIST_H
#define EQUIPMENT_LIST_H

#include <QWidget>

namespace Ui {
class equipment_list;
}

class equipment_list : public QWidget
{
    Q_OBJECT

public:
    explicit equipment_list(QWidget *parent = 0);
    ~equipment_list();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

public slots:
    void deviceNumSet(int,int,int,int,int);
signals:
    void backToWidget();
    void senddeviceinfo(int);
    void clearmap();

private:
    Ui::equipment_list *ui;
};

#endif // EQUIPMENT_LIST_H
