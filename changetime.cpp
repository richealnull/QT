#include "changetime.h"
#include "ui_changetime.h"

changeTime::changeTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changeTime)
{
    ui->setupUi(this);
    setWindowTitle("时间修改");
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->lineEdit_2);
   ui->lineEdit->setValidator(validator);
}

changeTime::~changeTime()
{
    delete ui;
}

void changeTime::on_pushButton_2_clicked()
{
    this->hide();
    emit backToWgDemand();
}
void changeTime::changeTimeShow()
{
    this->show();
}

void changeTime::recvgwid(QString id){
    gwid = id;
}

void changeTime::on_pushButton_clicked()
{
    //发送到数据库做完运算  加密发送给网关
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()){
        QMessageBox::about(NULL, "warring", "不能为空");
    }
    else{
        int daytime = ui->lineEdit->text().toInt();
        QString payfrom = ui->lineEdit_2->text();
        daytime = daytime*86400;
        emit sendDayToSql(daytime,payfrom,gwid);
    }

}
