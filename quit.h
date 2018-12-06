#ifndef QUIT_H
#define QUIT_H

#include <QWidget>

namespace Ui {
class quit;
}

class quit : public QWidget
{
    Q_OBJECT

public:
    explicit quit(QWidget *parent = 0);
    ~quit();

signals:
    void quit_to_Tabwidget_signal();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::quit *ui;
};

#endif // QUIT_H
