#ifndef CHANGETIME_H
#define CHANGETIME_H

#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <string.h>
#include <QValidator>

namespace Ui {
class changeTime;
}

class changeTime : public QWidget
{
    Q_OBJECT

public:
    explicit changeTime(QWidget *parent = 0);
    QString gwid;
    ~changeTime();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

signals:
    void backToWgDemand();
    void sendDayToSql(int,QString,QString);
public slots:
    void changeTimeShow();
    void recvgwid(QString);


private:
    Ui::changeTime *ui;
};

#endif // CHANGETIME_H
