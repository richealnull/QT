#ifndef WG_DEMAND_H
#define WG_DEMAND_H

#include <QWidget>

namespace Ui {
class wg_demand;
}

class wg_demand : public QWidget
{
    Q_OBJECT

public:
    explicit wg_demand(QWidget *parent = 0);
    QString gwid;
    ~wg_demand();
signals:
    void findwg_to_Tabwidget_signal();
    void equiListShow();
    void setrefeShow();
    void changeTimeShow();
    void sendgwidtochange(QString);
    void sendgwidtoset(QString);
    void setstoptimeshow();
public slots:
    void dealWgData(QString);
    void showself();

private slots:
    void on_Button_back_clicked();

    void on_Button_equipment_clicked();

    void on_Button_recommended_clicked();

    void on_Button_gatewaydate_clicked();

    //void on_pushButton_clicked();

private:
    Ui::wg_demand *ui;
};

#endif // WG_DEMAND_H
