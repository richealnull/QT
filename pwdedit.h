#ifndef PWDEDIT_H
#define PWDEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QKeyEvent>

class PwdEdit:public QLineEdit
{
    Q_OBJECT

public:
    PwdEdit(QWidget *parent = 0);
     ~PwdEdit();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *);
};

#endif // PWDEDIT_H
