#ifndef SETREFERRER_H
#define SETREFERRER_H

#include <QWidget>

namespace Ui {
class setreferrer;
}

class setreferrer : public QWidget
{
    Q_OBJECT

public:
    explicit setreferrer(QWidget *parent = 0);
    ~setreferrer();
signals:
    void backToWgDemand();
    void addRefereShow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public slots:
    void setrefeshow();
private:
    Ui::setreferrer *ui;
};

#endif // SETREFERRER_H
