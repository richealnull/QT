#ifndef ADDREFEREES_H
#define ADDREFEREES_H

#include <QWidget>

namespace Ui {
class addreferees;
}

class addreferees : public QWidget
{
    Q_OBJECT

public:
    explicit addreferees(QWidget *parent = 0);
    ~addreferees();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
signals:
    void backToRefere();
public slots:
    void addrefeshow();

private:
    Ui::addreferees *ui;
};

#endif // ADDREFEREES_H
