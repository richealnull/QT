#include "pwdedit.h"
#include <QKeyEvent>

PwdEdit::PwdEdit(QWidget *parent)
        :QLineEdit(parent)
{
    //设置无右键菜单
    this->setContextMenuPolicy(Qt::NoContextMenu);
    //设置密码提示
    this->setPlaceholderText(QString("密码"));
    //设置密码隐藏
    this->setEchoMode(QLineEdit::Password);
    //设置密码框样式
    this->setStyleSheet("QLineEdit{border-width: 1px; border-radius: 4px; font-size:12px; color:black; border:1px solid gray;}"
    "QLineEdit:hover{border-width: 1px; border-radius: 4px; font-size:12px; color: black; border:1pxsolid rgb(70, 200, 50);}");
    //设置最大长度16位
    this->setMaxLength(16);
}
PwdEdit::~PwdEdit()
{

}

void PwdEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::SelectAll))
    {
        return ;
    }
    else if(event->matches(QKeySequence::Copy))
    {
        return ;
    }
    else if(event->matches(QKeySequence::Paste))
    {
        return ;
    }
    else
    {
        QLineEdit::keyPressEvent(event);
    }
   // return QWidget::eventFilter(obj, event);
}

//主要设置鼠标位于密码框中时不可移动
void PwdEdit::mouseMoveEvent(QMouseEvent *)
{
    return ;
}

