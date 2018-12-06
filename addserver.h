#ifndef ADDSERVER_H
#define ADDSERVER_H

#include <QWidget>

namespace Ui {
class addServer;
}

class addServer : public QWidget
{
    Q_OBJECT

public:
    explicit addServer(QWidget *parent = 0);
    ~addServer();

signals:
    void server_to_Tabwidget_signal();

private slots:
    void on_Buttonconn_clicked();

private:
    Ui::addServer *ui;
};

#endif // ADDSERVER_H
