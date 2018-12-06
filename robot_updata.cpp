#include "robot_updata.h"
#include "ui_robot_updata.h"
#include <QFileDialog>
#include <QFileInfo>

robot_updata::robot_updata(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robot_updata)
{
    ui->setupUi(this);
}

robot_updata::~robot_updata()
{
    delete ui;
}

void robot_updata::on_Button_back_clicked()
{
    emit robot_to_Tabwidget_signal();
}

void robot_updata::on_Button_switch_bag_clicked()
{
    QString file_full, file_name, file_path;
    QFileInfo fi;

    file_full = QFileDialog::getOpenFileName(this);

    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    ui->label_filename->setText(file_name);
}

void robot_updata::on_Button_updata_clicked()
{

}
