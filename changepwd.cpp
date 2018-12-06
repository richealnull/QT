#include "changepwd.h"
#include "ui_changepwd.h"

changepwd::changepwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changepwd)
{
    ui->setupUi(this);

    setWindowTitle("密码修改");
    ui->line_oldpwd->setPlaceholderText(QStringLiteral("输入原密码"));
    ui->line_newpwd->setPlaceholderText(QStringLiteral("新密码"));
    ui->line_again->setPlaceholderText(QStringLiteral("再次输入"));
}

changepwd::~changepwd()
{
    delete ui;
}

void changepwd::on_pushButton_clicked()
{

     emit change_to_Tabwidget_signal();
}
