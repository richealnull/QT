#include "setreferrer.h"
#include "ui_setreferrer.h"

setreferrer::setreferrer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setreferrer)
{
    ui->setupUi(this);
    setWindowTitle("推荐人设置");
}

setreferrer::~setreferrer()
{
    delete ui;
}

void setreferrer::on_pushButton_clicked()
{
    emit backToWgDemand();
    this->hide();
}

void setreferrer::on_pushButton_2_clicked()
{
    this->hide();
    emit addRefereShow();
}
void setreferrer::setrefeshow()
{
    this->show();
}
