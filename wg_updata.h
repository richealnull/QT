#ifndef WG_UPDATA_H
#define WG_UPDATA_H

#include <QWidget>

namespace Ui {
class wg_updata;
}

class wg_updata : public QWidget
{
    Q_OBJECT

public:
    explicit wg_updata(QWidget *parent = 0);
    ~wg_updata();
    QString file_name, file_path;

signals:
    void wgupdata_to_Tabwidget_signal();
    void showUpList();
    void sendFileToSql(QString,QString,bool,QString);

public slots:
    void init_combox(QString);
    void showWgUpdata();

private slots:
    void on_Button_back_clicked();

    void on_Button_switch_bag_clicked();

    void on_Button_find_clicked();

    void on_Button_updata_clicked();

private:
    Ui::wg_updata *ui;
};

#endif // WG_UPDATA_H
