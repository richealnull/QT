#include "quit.h"
#include "ui_quit.h"

quit::quit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::quit)
{
    ui->setupUi(this);
}

quit::~quit()
{
    delete ui;
}

void quit::on_pushButton_clicked()
{
    emit quit_to_Tabwidget_signal();
}

void quit::on_pushButton_2_clicked()
{
    emit quit_to_Tabwidget_signal();
}
