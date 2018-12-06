#include "tab3_widget.h"
#include "ui_tab3_widget.h"
#include <QDialog>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

tab3_widget::tab3_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab3_widget)
{

    ui->setupUi(this);
    setWindowTitle("通知");
    //设置输入框提示文字
    ui->line_title->setPlaceholderText(QStringLiteral("请输入标题"));
    ui->line_link->setPlaceholderText(QStringLiteral("请输入链接"));
    ui->line_linkplace->setPlaceholderText(QStringLiteral("请输入机构名称"));
    ui->textEdit->setPlaceholderText(QStringLiteral("请输入内容"));
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->dateTimeEdit->setMinimumDate(QDate::currentDate().addDays(0));
    ui->dateTimeEdit->setMaximumDate(QDate::currentDate().addDays(365));
    ui->linehour->setValidator(new QIntValidator(0, 1000, this));//设置只能输入数字
}

tab3_widget::~tab3_widget()
{
    delete ui;
}


void tab3_widget::on_Button_back_clicked()
{
    emit backto_tab3();
    ui->linehour->text().clear();
    ui->line_title->text().clear();
    ui->textEdit->clear();
    ui->line_link->text().clear();
    ui->line_linkplace->text().clear();
}

void tab3_widget::on_Button_send_clicked()
{
    QString dateedit = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString houre = ui->linehour->text();
    QString title = ui->line_title->text();
    QString edit = ui->textEdit->toPlainText();
    QString link = ui->line_link->text();
    QString linkplace = ui->line_linkplace->text();
    emit backto_tab3();
    emit sendbufToThis(wg_name,radiobtn,serverall,obj,type,dateedit,houre,title,edit,link,linkplace,imgAdress,imgAdress2,imgAdress3);
    qDebug()<<dateedit<<endl;
    QDateTime start = QDateTime::fromString(dateedit,"yyyy-MM-dd HH:mm:ss");
    uint stime = start.toTime_t();
    qDebug()<<"时间戳:"<<stime<<endl;
    qDebug()<<imgAdress<<imgAdress2<<imgAdress3<<radiobtn<<serverall<<obj<<type<<endl;
    ui->linehour->clear();
    ui->line_title->clear();
    ui->textEdit->clear();
    ui->line_link->clear();
    ui->line_linkplace->clear();
}

void tab3_widget::on_Button_img_clicked()
{
        QString filename;
        filename=QFileDialog::getOpenFileName(this,
                                              tr("选择图像"),
                                              "",
                                              tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
        if(filename.isEmpty())
        {
             return;
        }
        else
        {
            QImage* img=new QImage;

            if(! ( img->load(filename) ) ) //加载图像
            {
                QMessageBox::information(this,
                                         tr("打开图像失败"),
                                         tr("打开图像失败!"));
                delete img;
                return;
            }
            ui->label_ing->setPixmap(QPixmap::fromImage(*img));
            imgAdress = filename;
            qDebug()<<filename<<endl;
        }
}

void tab3_widget::dealInfobtn(QString wg_name_f, bool radiobtn_f, QString serverall_f, QString obj_f, QString type_f)
{
    wg_name = wg_name_f;
    radiobtn =radiobtn_f;
    serverall =serverall_f;
    obj =obj_f;
    type = type_f;
    qDebug()<<"dealinfotbtn"<<imgAdress<<radiobtn<<serverall<<obj<<type<<endl;
}

void tab3_widget::on_Button_img2_clicked()
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this,
                                          tr("选择图像"),
                                          "",
                                          tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(filename.isEmpty())
    {
         return;
    }
    else
    {
        QImage* img=new QImage;

        if(! ( img->load(filename) ) ) //加载图像
        {
            QMessageBox::information(this,
                                     tr("打开图像失败"),
                                     tr("打开图像失败!"));
            delete img;
            return;
        }
        ui->label_ing->setPixmap(QPixmap::fromImage(*img));
        imgAdress2 = filename;
        qDebug()<<filename<<endl;
    }
}

void tab3_widget::on_Button_img3_clicked()
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this,
                                          tr("选择图像"),
                                          "",
                                          tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(filename.isEmpty())
    {
         return;
    }
    else
    {
        QImage* img=new QImage;

        if(! ( img->load(filename) ) ) //加载图像
        {
            QMessageBox::information(this,
                                     tr("打开图像失败"),
                                     tr("打开图像失败!"));
            delete img;
            return;
        }
        ui->label_ing->setPixmap(QPixmap::fromImage(*img));
        imgAdress3 = filename;
        qDebug()<<filename<<endl;
    }
}
