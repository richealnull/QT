#ifndef ADDTIME_H
#define ADDTIME_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class addtime;
}

class addtime : public QWidget
{
    Q_OBJECT

public:
    explicit addtime(QWidget *parent = 0);
    ~addtime();
signals:
    void timeback_super();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::addtime *ui;
};

#endif // ADDTIME_H
