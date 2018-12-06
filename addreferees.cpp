#include "addreferees.h"
#include "ui_addreferees.h"

addreferees::addreferees(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addreferees)
{
    ui->setupUi(this);
    setWindowTitle("添加推荐人");
}

addreferees::~addreferees()
{
    delete ui;
}

void addreferees::on_pushButton_clicked()
{

}

void addreferees::on_pushButton_2_clicked()
{
    this->hide();
    emit backToRefere();
}
void addreferees::addrefeshow()
{
    this->show();
}
