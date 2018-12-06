#-------------------------------------------------
#
# Project created by QtCreator 2017-12-15T09:14:38
#
#-------------------------------------------------

QT       += core gui charts sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = management
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    supermanage.cpp \
    setmanage.cpp \
    tabwidget.cpp \
    tab3_widget.cpp \
    findinfo.cpp \
    wg_demand.cpp \
    addserver.cpp \
    changepwd.cpp \
    quit.cpp \
    wg_updata.cpp \
    robot_updata.cpp \
    workthread.cpp \
    widgetthread.cpp \
    md5.cpp \
    widgetsocket.cpp \
    sqldeal.cpp \
    pwdedit.cpp \
    sendlist.cpp \
    equipment_list.cpp \
    device_show.cpp \
    setreferrer.cpp \
    addreferees.cpp \
    changetime.cpp \
    update_list.cpp \
    update_wglist.cpp \
    parse.cpp \
    addtime.cpp \
    des3.c

HEADERS  += login.h \
    supermanage.h \
    setmanage.h \
    tabwidget.h \
    tab3_widget.h \
    findinfo.h \
    wg_demand.h \
    addserver.h \
    changepwd.h \
    quit.h \
    wg_updata.h \
    robot_updata.h \
    workthread.h \
    widgetthread.h \
    md5.h \
    widgetsocket.h \
    sqldeal.h \
    pwdedit.h \
    sendlist.h \
    equipment_list.h \
    device_show.h \
    setreferrer.h \
    addreferees.h \
    changetime.h \
    update_list.h \
    update_wglist.h \
    parse.h \
    addtime.h \
    des3.h

FORMS    += login.ui \
    supermanage.ui \
    setmanage.ui \
    tabwidget.ui \
    tab3_widget.ui \
    findinfo.ui \
    wg_demand.ui \
    addserver.ui \
    changepwd.ui \
    quit.ui \
    robot_updata.ui \
    sendlist.ui \
    equipment_list.ui \
    device_show.ui \
    setreferrer.ui \
    addreferees.ui \
    changetime.ui \
    wg_updata.ui \
    update_list.ui \
    update_wglist.ui \
    addtime.ui
CONFIG += c++11;
