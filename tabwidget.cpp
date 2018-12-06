#include "tabwidget.h"
#include "ui_tabwidget.h"
#include "des3.h"

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::TabWidget)
{
    ui->setupUi(this);
    setWindowTitle("管理端");
     qDebug() <<"widget线程号:"<< QThread::currentThread()<<endl;

     //widgetT = new widgetthread;
     widgetS = new widgetsocket;
     sql = new sqldeal;
     //par = new parse;
     //创建时间子线程
     //thread = new QThread(this);
     threads = new QThread(this);
     sqlthread = new QThread(this);
     //parthread = new QThread(this);
     //把自定义的线程加入子线程
     //widgetT->moveToThread(thread);
     widgetS->moveToThread(threads);
     sql->moveToThread(sqlthread);
     //par->moveToThread(parthread);
     //定时器设置
     timer = new QTimer();
     timer->setInterval(30000);     //心跳 三十秒
     timerinfo = new QTimer();
     timerinfo->setInterval(3600000);  //通知消息查询 一小时
     timeOne = new QTimer();
     timeOne->setInterval(1000);    //一秒
     timeTen = new QTimer();
     timeTen->setInterval(10000);   //十秒
     timerfive = new QTimer();
     timerfive->setInterval(5000);  //五秒


    //曲线图绘制
    maxSize = 20; //只存储最新的20个
    maxX = 300;
    maxY = 30000;
    ipSplineSeries = new QSplineSeries();    //曲线设置
    userSplineSeries = new QSplineSeries();
    userScatterSeries = new QScatterSeries();
    ipScatterSeries = new QScatterSeries();

    ipSplineSeries->setName("用户数量");
    userSplineSeries->setName("网关数量");

    chart = new QChart();
    chart->addSeries(ipSplineSeries);
    chart->addSeries(userSplineSeries);
    chart->addSeries(userScatterSeries);
    chart->addSeries(ipScatterSeries);
    chart->legend()->hide();
    chart->setTitle("家和用户数量曲线图");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0,300);
    chart->axisY()->setRange(0,maxY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //设置布局
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(ui->label,0,0);
    baseLayout->addWidget(ui->label_2,1,0);
    baseLayout->addWidget(ui->label_3,2,0);
    baseLayout->addWidget(ui->label_4,3,0);
    baseLayout->addWidget(ui->label_5,4,0);
    baseLayout->addWidget(ui->label_time,5,0);
    baseLayout->addWidget(chartView,0,1,6,6);
    ui->tab->setLayout(baseLayout);

    //connect(this,&TabWidget::starttime,widgetT,&widgetthread::widgetconn);
    //线程更新tab3
    //connect(widgetT,&widgetthread::sendupdate_tab2_to_tab,this,&TabWidget::dealtab2_update_to_sql);
    //connect(this,&TabWidget::sendUpdateToSql,sql,&sqldeal::updatetab2);
    //定时器线程

    //线程时间更新
    //connect(widgetT,&widgetthread::sendChangeTimeTotab,this,&TabWidget::dealchangeTime);
    connect(this,&TabWidget::sendChangeTimeToSql,sql,&sqldeal::changeTime);  //修改数据库中剩余时间

    //定时器刷新界面时间一秒
    connect(timeOne,timeOne->timeout,this,&TabWidget::timeone);
    //定时器刷新服务器列表和界面数据十秒
    connect(timeTen,timeTen->timeout,this,TabWidget::timeten);
    //定时器查询网关
    connect(timerfive,timerfive->timeout,this,&TabWidget::timerFive);
    //通知数据库刷新
    connect(this,&TabWidget::sendTab2UpdateToSql,sql,&sqldeal::updatetab2);
    //更新主界面剩余时间
    connect(sql,&sqldeal::connsignal,this,&TabWidget::threadconn);
    //更新服务器列表 和曲线图
    connect(sql,&sqldeal::sendupdate_tab2,this,&TabWidget::updatetab2);


    //通讯线程数据库查询
    connect(this,&TabWidget::startsock,sql,&sqldeal::connserver);
    //通讯 sql传递给this
    connect(sql,&sqldeal::sendconnserver,this,&TabWidget::dealSendConnServer);
    //this传递给sock
    connect(this,&TabWidget::sendConnToSock,widgetS,&widgetsocket::Createsocket);

    //sock重连信号传递给this
    connect(widgetS,&widgetsocket::sendReconnect,this,&TabWidget::dealSockReconnect);
    //this重连信号传递给sql
    connect(this,&TabWidget::sendReconnectToSql,sql,&sqldeal::reconnect);
    //sql重连信号传递给this
    connect(sql,&sqldeal::sendreconnect,this,&TabWidget::dealSqlReconnect);
    //this重连信号传递给sock
    connect(this,&TabWidget::sendReconnectToSock,widgetS,&widgetsocket::reconnection);

    //线程心跳
    connect(timer,timer->timeout,widgetS,&widgetsocket::heartbeat);

    //通知消息查询发送
    connect(timerinfo,timerinfo->timeout,sql,&sqldeal::infotimer);
    //通知消息筛选给sock
    connect(sql,&sqldeal::infoSendToThis,this,&TabWidget::dealInfoFromSql);
    //通知发送id存数据库
    connect(widgetS,&widgetsocket::sendSaveIdToSql,sql,&sqldeal::SaveId);
    //sock处理定时通知消息
    connect(this,&TabWidget::sendInfoToSock,widgetS,&widgetsocket::dealInfoTimer);

    //table2初始化
    connect(this,&TabWidget::sendpaint_tab2,sql,&sqldeal::recvtab2_member);
    //table2初始化
    connect(sql,&sqldeal::painttable2,this,&TabWidget::painttab2);
    //服务器状态改变
    connect(widgetS,&widgetsocket::sendStat,sql,&sqldeal::changeServerState);

    //重连服务器
    connect(this,&TabWidget::sendrestart,widgetS,&widgetsocket::dealrestart);

    //添加服务器
    connect(this,&TabWidget::senAddServer,widgetS,&widgetsocket::dealAddServer);
    //通讯层添加服务器
    connect(widgetS,&widgetsocket::sendAddServer,this,&TabWidget::dealSendAddToSql);
    //数据库添加服务器
    connect(this,&TabWidget::sendAddToSql,sql,&sqldeal::addServer);
    //添加服务器失败
    connect(widgetS,&widgetsocket::sendAddServerFail,this,&TabWidget::dealAddServerFail);
    //添加服务器成功
    //数据库删除服务器
    connect(this,&TabWidget::senddeleteToSql,sql,&sqldeal::dealDeleteTablewidget);
    //界面删除数据库
    connect(sql,sqldeal::sendDeleteToThis,this,&TabWidget::dealDealTableWidget);

    //关闭服务器连接
    connect(this,&TabWidget::sendclosesock,widgetS,&widgetsocket::closesock);

    //通讯层更改数据库serverlist
    connect(widgetS,&widgetsocket::sendChangeServer,this,&TabWidget::dealSockChangeServer);
    //this 传递给sql层
    connect(this,&TabWidget::sendChangeServerToSql,sql,&sqldeal::changeserver);

    //combobox 初始化
    connect(widgetS,&widgetsocket::sendInitcombox,this,&TabWidget::dealInitcombox);
    connect(widgetS,&widgetsocket::sendInitcombox,&updata_wg,&wg_updata::init_combox);

    //通知消息发送
    //通知消息初始化
    connect(this,&TabWidget::sendInitInfo,widgetS,&widgetsocket::initdate);
    //this发送通知选项给tab3
    connect(this,&TabWidget::sendInfoToTab3,&tab3,&tab3_widget::dealInfobtn);
    //通知消息从tab3发送到this
    connect(&tab3,&tab3_widget::sendbufToThis,this,&TabWidget::dealsendbufFromTab3);
    //通知消息入库
    connect(this,&TabWidget::sendbufToSql,sql,&sqldeal::addInfo);
    //通知发送给this
    connect(sql,&sqldeal::sendInfoToSock,this,&TabWidget::dealInfoToSock);
    //通知消息发给服务器
    connect(this,&TabWidget::sendbufToSock,widgetS,&widgetsocket::sendInfoToServer);
    //通知列表初始化
    connect(this,&TabWidget::sendInfoInitToSql,sql,&sqldeal::dealInfoInit);
    connect(sql,&sqldeal::initInfoSendToThis,this,&TabWidget::dealInitInfoFromSql);
    connect(this,&TabWidget::sendInfoInitToInfo,&find,findInfo::initInfo);
    //通知列表更新
    connect(this,&TabWidget::sendInfoUpdate,sql,&sqldeal::dealInfoUpdate);
    connect(sql,&sqldeal::updateInfoSendToThis,this,&TabWidget::dealUpdateToInfo);
    connect(this,&TabWidget::sendUpdateToInfo,&find,&findInfo::updateInfo);

    //查看通知人员
    connect(&find,&findInfo::sengwidslist,this,&TabWidget::dealgwidslist);
    connect(this,&TabWidget::sendgwidsToSql,sql,&sqldeal::dealgwidslist);
    connect(sql,&sqldeal::sendwglist,this,&TabWidget::dealgwidsFromSql);
    connect(this,&TabWidget::sendgwidsToList,&wglist,&sendlist::showlist);
    //清空通知人员表
    connect(this,&TabWidget::sendclearlist,&wglist,&sendlist::dealclear);

    //网关查询
    connect(this,&TabWidget::sendSelectWgToSock,widgetS,&widgetsocket::dealSelectWg);//消息发送给网关
    //connect(this,&TabWidget::selectWgTime,widgetS,&widgetsocket::dealSelectWgTime); //查询消息定时器
   //网关界面权限开启
    connect(widgetS,&widgetsocket::sendWgPermission,&find_wg,&wg_demand::dealWgData);
    //网关截止时间查询
    connect(widgetS,&widgetsocket::sendTimeToSql_UI,sql,&sqldeal::gwOnline);
    //关闭定时器
    connect(widgetS,&widgetsocket::stopTimeFive,this,&TabWidget::stopTimeFive);
    //网关id传输
    connect(&find_wg,&wg_demand::sendgwidtochange,&changetime,&changeTime::recvgwid);
    //网关充值
    connect(&changetime,&changeTime::sendDayToSql,sql,&sqldeal::dealWgPay);
    //sql到socket发送充值信息
    connect(sql,&sqldeal::sendToWgAddDays,widgetS,&widgetsocket::sendStopTimeToGw);



    connect(this,&TabWidget::sendtest,sql,&sqldeal::test);
    //设备列表显示
    connect(&find_wg,&wg_demand::equiListShow,this,&TabWidget::equiListShow);
    //socket map发送
    connect(&find_wg,&wg_demand::equiListShow,widgetS,&widgetsocket::deviceNums);
    connect(widgetS,&widgetsocket::sendDeviceNums,&equiList,&equipment_list::deviceNumSet);
    //设备列表返回
    connect(&equiList,&equipment_list::backToWidget,this,&TabWidget::equiListToSelectWg);
    connect(&equiList,&equipment_list::clearmap,widgetS,&widgetsocket::dealClearMap);
    //设备详细列表显示
    connect(&equiList,&equipment_list::senddeviceinfo,this,&TabWidget::dealdeviceFromEqui);
    connect(this,&TabWidget::sendDeviceInfoToSock,widgetS,&widgetsocket::dealDeviceInfo);
    connect(widgetS,&widgetsocket::sendDeviceInfo,&device,&device_show::deviceSet);

    //推荐人显示
    connect(&find_wg,&wg_demand::setrefeShow,this,&TabWidget::dealSetRefeShow);
    //推荐人返回
    connect(&setrefe,&setreferrer::backToWgDemand,&find_wg,&wg_demand::showself);
    //添加推荐人显示
    connect(&setrefe,&setreferrer::addRefereShow,&addrefe,&addreferees::addrefeshow);
    //添加推荐人返回
    connect(&addrefe,&addreferees::backToRefere,&setrefe,&setreferrer::setrefeshow);

    //修改时间显示
    connect(&find_wg,&wg_demand::changeTimeShow,&changetime,&changeTime::changeTimeShow);
    //修改时间返回
    connect(&changetime,&changeTime::backToWgDemand,&find_wg,&wg_demand::showself);
    //设置时间显示
    //connect(&find_wg,&wg_demand::setstoptimeshow,&wg_setstoptime,&setstoptime::showself);
    //设置时间返回
    //connect(&wg_setstoptime,&setstoptime::backto_daemand,&find_wg,&wg_demand::showself);
    //id传递给设置页面
    //connect(&find_wg,&wg_demand::sendgwidtoset,&wg_setstoptime,&setstoptime::recvgwid);
    //网关时间直接更改
    //connect(&wg_setstoptime,&setstoptime::changetime,sql,&sqldeal::dealWgChange);

    //网关升级界面显示
    connect(&updata_wg,&wg_updata::showUpList,&wg_updatelist,&update_list::showUpList);
    connect(&wg_updatelist,&update_list::backToWgUpdata,&updata_wg,&wg_updata::showWgUpdata);
    connect(&wg_updatelist,&update_list::showUpWgList,this,&TabWidget::showUpWgList);

    //网关升级
    connect(&updata_wg,&wg_updata::sendFileToSql,this,&TabWidget::sendToSqlWgUpdate);
    //发送userid给sql
    connect(this,&TabWidget::sendUseridToSql,sql,&sqldeal::saveUpdateFile);
    //sql传递给sock
    connect(sql,&sqldeal::sendWgUpdateToSock,widgetS,&widgetsocket::dealWgUpdate);

    //进度条显示
    connect(widgetS,&widgetsocket::sendProgressBar,this,&TabWidget::dealProgressBar);
    //进度条更新
    connect(widgetS,&widgetsocket::sendProgressBarChange,this,&TabWidget::dealProgressBarChange);

     connect(ui->lineEdit_2, SIGNAL(returnPressed()), ui->pushButton_4, SIGNAL(clicked()), Qt::UniqueConnection);
    //解析处理
    //connect(widgetS,&widgetsocket::sendParse,par,&parse::onpacket);


    //子界面通知查询返回
    connect(&tab3,&tab3_widget::backto_tab3,
            [=]()
            {
                this->show();
                tab3.hide();
            }

    );
    //子界面通知查询返回
    connect(&find,&findInfo::findinfo_to_Tabwidget_signal,
            [=]()
            {
                    this->show();
                    find.hide();
                    wglist.hide();
            }
    );
    //子界面查询网关返回
    connect(&find_wg,&wg_demand:: findwg_to_Tabwidget_signal,
            [=]()
            {
                    this->show();
            }
    );

    //修改密码界面返回
    connect(&change,&changepwd::change_to_Tabwidget_signal,
            [=]()
            {
                    this->show();
                    change.hide();
            }
    );
    //退出界面返回
    connect(&q,&quit::quit_to_Tabwidget_signal,
            [=]()
            {
                    this->show();
                    q.hide();
            }
    );

    //网关升级界面返回
    connect(&updata_wg,&wg_updata::wgupdata_to_Tabwidget_signal,
            [=]()
            {
                    this->show();
                    updata_wg.hide();
            }
    );
    //关闭界面关闭线程,关闭socket
    connect(this,&TabWidget::destroyed,this,
            [=](){
                timer->stop();
                timerinfo->stop();
                timeOne->stop();
                timeTen->stop();


               // emit sendChangeTimeToSql(label_time,user_id);

                threads->quit();
                threads->wait();
                qDebug()<<"socked?"<<endl;
                //parthread->quit();
                //parthread->wait();
                qDebug()<<"par???"<<endl;
                sqlthread->quit();
                sqlthread->wait();
                qDebug()<<"sql??"<<endl;

            }
            );
    //设置tab2的列表
    ui->tableWidget->setColumnCount(9);   //设置列数为8
    //设置不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0); //设置当前滑动条位置
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"id"<<"服务器"<<"ip"<<"端口"<<"连接数"<<"app数量"<<"网关数量"<<"cpu"<<"连接状态");

    //初始化tab3 comboBox
     ui->comboBox->insertItem(0,"服务器列表");
     ui->comboBox->setEditable(false); //设置不可编辑
     ui->comboBox_2->setEditable(false);
     ui->comboBox_3->setEditable(false);

     //网关输入框设置只能输入数字
     QRegExp regx("[0-9]+$");
     QValidator *validator = new QRegExpValidator(regx,ui->lineEdit_2);
     ui->lineEdit_2->setValidator(validator);

    //开启线程
    sqlthread->start();
    //thread->start();
    threads->start();
    //parthread->start();
    //widgetT->setFlag(false);
    emit sendpaint_tab2();
}

TabWidget::~TabWidget()
{
    delete ui;
}

void TabWidget::time(QString time, int general_id)
{
        ui->label_time->setText(time);
        qDebug()<<"widget time"<<time<<endl;

        QString hh = time.mid(0,2);
        QString mm = time.mid(3,2);
        QString ss = time.mid(6,92);
        adminTime = hh.toInt()*3600+mm.toInt()*60+ss.toInt();
        timeOne->start();
        timeTen->start();

        //emit starttime(time,general_id);

        emit startsock();
        emit sendupdate_tab2();
        emit sendInfoInitToSql();
        timer->start();
        timerinfo->start();
        user_id = general_id;
}

void TabWidget::threadconn(int userid,QString ip,QString username,int port,int app_count,int wg_count)
{
    ui->label_2->setText(QString::number(wg_count,10));
    ui->label_4->setText(QString::number(app_count,10));
    dataReceived(app_count,wg_count);
}

void TabWidget::dataReceived(int app_count, int wg_count)
{
    ports << app_count;
    users << wg_count;
    //数据个数超过最大数量，删除最先收到数据，实现数据向前移动
    while(ports.size() > maxSize){
        ports.removeFirst();
    }
    while(users.size() > maxSize){
        users.removeFirst();
    }
    //界面hide 不绘制
    if(isVisible()){
        ipSplineSeries->clear();
        userSplineSeries->clear();
        ipScatterSeries->clear();
        userScatterSeries->clear();
        int dx = maxX / (maxSize-1);
        int less = maxSize - ports.size();

        for (int i = 0; i < ports.size(); ++i) {
              ipSplineSeries->append(less*dx+i*dx, ports.at(i));
              ipScatterSeries->append(less*dx+i*dx, ports.at(i));
            }
        for (int i = 0; i < users.size(); ++i) {
              userSplineSeries->append(less*dx+i*dx, users.at(i));
              userScatterSeries->append(less*dx+i*dx, users.at(i));
            }
    }
}

void TabWidget::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text() == NULL && ui->radioButton->isChecked() == false&&ui->comboBox->currentText()=="服务器列表"){
        QMessageBox::about(NULL, "warring", "请选择一个发送对象");
    }
    else{
        qDebug()<<ui->lineEdit->text()<<endl;
        qDebug()<<ui->radioButton->isChecked()<<endl;
        qDebug()<<ui->comboBox->currentText()<<endl;
        qDebug()<<ui->comboBox_2->currentText()<<endl;
        qDebug()<<ui->comboBox_3->currentText()<<endl;
        emit sendInitInfo();
        emit sendInfoToTab3(ui->lineEdit->text(),ui->radioButton->isChecked(),ui->comboBox->currentText(),ui->comboBox_2->currentText(),ui->comboBox_3->currentText());
        this->hide();
        tab3.show();
    }
}

void TabWidget::on_pushButton_3_clicked()
{
    this->hide();
    //更新消息列表界面信号
    emit sendInfoUpdate();
    find.setGeometry(100,200,852,404);
    find.show();
}

void TabWidget::on_pushButton_4_clicked()
{

   /* unsigned char * p_str = (unsigned char *)"0F8lA48594AC4EC8CDF AABl5E2CFF733DEEl0C8113B82F0384D073F9527Bacl{};;;allklk784 142112/?,.&^$#@~`~`";
    unsigned char * p_anwen, * p_mingwen;
    int 		len = 0, i;

    //加密
    len = strlen((const char *)p_str);
    len = des3_api(DES_ENCRYPT, (unsigned char *)p_str, len, &p_anwen);

    //解密
    len = des3_api(DES_DECRYPT, p_anwen, len, &p_mingwen);
    qDebug()<<"decrypted bin:  "<<endl;
    for (i = 0; i < len; i++) {
        printf("%c",*(p_mingwen + i));
    }
    printf("\n");*/
    QString id = ui->lineEdit_2->text();
    if(id.isEmpty())
    {
        QMessageBox::about(NULL, "错误", "网关号不能为空");
    }
    else{
        ui->lineEdit_2->clear();
        qDebug()<<id<<endl;
        this->hide();
        find_wg.show();
        gwid.clear();
        gwid = id;
        emit sendSelectWgToSock(id);
        //定时器查询网关
        timerfive->start();
        //emit selectWgTime(id);
    }
}

void TabWidget::painttab2(int id,QString ip,QString servername,int port,int conn_count,int app_count,int wg_count,double cpu,bool state)
{

    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行
    QString uid = QString::number(id,10);
    QString uport = QString::number(port,10);
    QString uconnnum = QString::number(conn_count,10);
    QString uapp = QString::number(app_count,10);
    QString uwg = QString::number(wg_count,10);
    QString ucpu = QString::number(cpu,10,2);

    ui->tableWidget->setItem(RowCont,0,new QTableWidgetItem(uid));
    ui->tableWidget->setItem(RowCont,1,new QTableWidgetItem(servername));
    ui->tableWidget->setItem(RowCont,2,new QTableWidgetItem(ip));
    ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem(uport));
    ui->tableWidget->setItem(RowCont,4,new QTableWidgetItem(uconnnum));
    ui->tableWidget->setItem(RowCont,5,new QTableWidgetItem(uapp));
    ui->tableWidget->setItem(RowCont,6,new QTableWidgetItem(uwg));
    ui->tableWidget->setItem(RowCont,7,new QTableWidgetItem(ucpu));
    if(state == true)
    {
        ui->tableWidget->setItem(id,8,new QTableWidgetItem("已连接"));
    }
    else{
        ui->tableWidget->setItem(id,8,new QTableWidgetItem("未连接"));
    }
}


void TabWidget::updatetab2(int id,QString ip,QString servername,int port,int conn_count,int app_count,int wg_count,double cpu,bool state)
{
    qDebug()<<"更新列表数据"<<id<<ip<<servername<<port<<conn_count<<app_count<<wg_count<<cpu<<state<<endl;
    QString uid = QString::number(id,10);
    QString uport = QString::number(port,10);
    QString uconnnum = QString::number(conn_count,10);
    QString uapp = QString::number(app_count,10);
    QString uwg = QString::number(wg_count,10);
    QString ucpu = QString::number(cpu,10,2);
    qDebug()<<uport<<uid<<uconnnum<<uapp<<uwg<<ucpu<<endl;
    ui->tableWidget->setItem(id,0,new QTableWidgetItem(uid));
    ui->tableWidget->setItem(id,1,new QTableWidgetItem(servername));
    ui->tableWidget->setItem(id,2,new QTableWidgetItem(ip));
    ui->tableWidget->setItem(id,3,new QTableWidgetItem(uport));
    ui->tableWidget->setItem(id,4,new QTableWidgetItem(uconnnum));
    ui->tableWidget->setItem(id,5,new QTableWidgetItem(uapp));
    ui->tableWidget->setItem(id,6,new QTableWidgetItem(uwg));
    ui->tableWidget->setItem(id,7,new QTableWidgetItem(ucpu));
    if(state == true)
    {
        ui->tableWidget->setItem(id,8,new QTableWidgetItem("已连接"));
    }
    else{
        ui->tableWidget->setItem(id,8,new QTableWidgetItem("未连接"));
    }
    qDebug()<<"这也到了"<<endl;
}

//增加服务器
void TabWidget::on_pushButton_clicked()
{

    if(ui->line_ip->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "ip不能为空");
    }
    else if(ui->line_port->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "端口不能为空");
    }
    else if(ui->line_name->text().isEmpty()){
        QMessageBox::about(NULL, "错误", "名字不能为空");
    }
    else{
        QString Serverip,Serverport,Servername;
        Serverip = ui->line_ip->text().trimmed();
        Serverport = ui->line_port->text().trimmed();
        Servername = ui->line_name->text().trimmed();
        ui->line_ip->clear();
        ui->line_port->clear();
        ui->line_name->clear();
        emit senAddServer(Servername,Serverip,Serverport.toInt());
    }
}

void TabWidget::on_pushButton_5_clicked()
{
    this->hide();
    change.show();
}

void TabWidget::on_pushButton_6_clicked()
{
    this->hide();
    q.show();
}

void TabWidget::on_Button_gw_updata_clicked()
{
    this->hide();
    updata_wg.show();
}

void TabWidget::on_Button_robot_updata_clicked()
{
    this->hide();
    updata_wg.show();
}

void TabWidget::on_Button_count_updata_clicked()
{
    this->hide();
    updata_wg.show();
}

void TabWidget::on_Button_restart_clicked()
{
        QTableWidgetItem * itemr = ui->tableWidget->currentItem();
        qDebug()<<"??"<<endl;
        if(itemr==Q_NULLPTR)return;
        int i = itemr->row();
        QString serverName = ui->tableWidget->item(i, 1)->text();//获取服务器名
        QString ip = ui->tableWidget->item(i,2)->text();//获取服务器ip
        int port = ui->tableWidget->item(i,3)->text().toInt();//获取端口
        qDebug()<<serverName<<ip<<port<<endl;
        //emit sendrestart(serverName);
        emit sendConnToSock(serverName,ip,port);

}

void TabWidget::dealSendAddToSql(QString serverip,QString servername,int serverport)
{
    emit sendAddToSql(serverip,servername,serverport);
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行
    QString Serverport = QString::number(serverport, 10);
    ui->tableWidget->setItem(RowCont,1,new QTableWidgetItem(servername));
    ui->tableWidget->setItem(RowCont,2,new QTableWidgetItem(serverip));
    ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem(Serverport));
}

void TabWidget::dealSendConnServer(QString servername,QString serverip,int serverport)
{
    emit sendConnToSock(servername,serverip,serverport);
}

void TabWidget::dealSockReconnect(QString servername)
{
    emit sendReconnectToSql(servername);
}
void TabWidget::dealSqlReconnect(QString servername, QString ip, int port)
{
    emit sendReconnectToSock(servername,ip,port);
}
void TabWidget::dealSockChangeServer(QString uname, int conn_count, int app_count, int wg_count, double cpu)
{
    qDebug()<<"?????没改数据库？"<<endl;
    emit sendChangeServerToSql(uname,conn_count,app_count,wg_count,cpu);
}
//设置chombox
void TabWidget::dealInitcombox(QString name)
{
    qDebug()<<"combox设置"<<ui->comboBox->count()+1<<endl;
    qDebug()<<name<<endl;
    for(int i=0;i<ui->comboBox->count();i++)
    {
        if (name ==ui->comboBox->itemText(i))
        {
            qDebug()<<"combox删除"<<endl;
                ui->comboBox->removeItem(i);
        }
    }
    ui->comboBox->insertItem(ui->comboBox->count()+1,name);
}

/*void TabWidget::dealtab2_update_to_sql(int general_id,int a)
{
    emit sendUpdateToSql(general_id,a);
}*/
/*void TabWidget::dealchangeTime(QString label_time,int general_id)
{
    emit sendChangeTimeToSql(label_time,general_id);
}*/

void TabWidget::dealsendbufFromTab3(QString wg_name, bool radiobtn, QString serverall, QString obj, QString type, QString dateedit, QString houre, QString title, QString edit, QString link, QString linkplace, QString imgAdress, QString imgAdress2, QString imgAdress3)
{
    qDebug()<<"消息发给sql:"<<radiobtn<<serverall<<obj<<type<<endl;
    int userid = user_id;
    emit sendbufToSql(wg_name,radiobtn,serverall,obj,type,dateedit,houre,title,edit,link,linkplace,imgAdress,userid,imgAdress2,imgAdress3);
}
void TabWidget::dealInfoToSock(QString wg_name, bool radiobtn, QString serverall, QString obj, QString type, QString dateedit, QString houre, QString title, QString edit, QString link, QString linkplace, QString imgAdress, int user_id, int gwsid,QString imgAdress2,QString imgAdress3)
{
    qDebug()<<"消息发给sock"<<endl;
    emit sendbufToSock(wg_name,radiobtn,serverall,obj,type,dateedit,houre,title,edit,link,linkplace,imgAdress,user_id,gwsid,imgAdress2,imgAdress3);
}

void TabWidget::on_pushButton_7_clicked()
{
    emit sendtest();
}

void TabWidget::on_Button_up_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value();

       if(nCurScroller>0)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(nCurScroller-pageValue);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(maxValue);
}

void TabWidget::on_Button_down_clicked()
{
    int maxValue = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
       nCurScroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

       if(nCurScroller<maxValue)
           ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue+nCurScroller);
       else
           ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
}

void TabWidget::dealAddServerFail()
{
     QMessageBox::about(NULL, "错误", "连接失败");
}

void TabWidget::dealInfoFromSql(int userid,int num,bool radiobtn,QString timeend,QString title,QString content,QString link,QString linkplace,QString imgaddress,QString intervaltime,QString lasttime,QString wg_name,QString serverall,QString obj,QString type,QString timebegin,int gwids)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    uint ntime = current_date_time.toTime_t();
    QDateTime end = QDateTime::fromString(timeend,"yyyy-MM-dd HH:mm:ss");
    uint stime = end.toTime_t();
    QDateTime last = QDateTime::fromString(lasttime,"yyyy-MM-dd HH:mm:ss");
    uint ltime = last.toTime_t();

    if(ntime<stime)
    {
        if((ntime - ltime) > intervaltime.toInt() * 3600)
        {
            emit sendInfoToSock(userid,num,radiobtn,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,serverall,obj,type,timebegin,gwids);
        }
    }
}

void TabWidget::dealInitInfoFromSql(int userid,int num,bool radiobtn,QString timeend,QString title,QString content,QString link,QString linkplace,QString imgaddress,QString intervaltime,QString lasttime,QString wg_name,QString serverall,QString obj,QString type,QString timebegin,int gwids)
{
    emit sendInfoInitToInfo(userid,num,radiobtn,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,serverall,obj,type,timebegin,gwids);
}

void TabWidget::dealUpdateToInfo(int userid,int num,bool radiobtn,QString timeend,QString title,QString content,QString link,QString linkplace,QString imgaddress,QString intervaltime,QString lasttime,QString wg_name,QString serverall,QString obj,QString type,QString timebegin,int listnum,int gwids)
{
    emit sendUpdateToInfo(userid,num,radiobtn,timeend,title,content,link,linkplace,imgaddress,intervaltime,lasttime,wg_name,serverall,obj,type,timebegin,listnum,gwids);
}

void TabWidget::dealgwidslist(QString gwids)
{
    emit sendclearlist();
    emit sendgwidsToSql(gwids);
    wglist.setGeometry(968,200,246,405);
    wglist.show();
}
void TabWidget::dealgwidsFromSql(QString gwids, QString wgname, int listnum)
{
    emit sendgwidsToList(gwids,wgname,listnum);
}
void TabWidget::equiListToSelectWg()
{
    equiList.hide();
    find_wg.show();
}
void TabWidget::equiListShow()   //设备列表显示
{
    find_wg.hide();
    equiList.show();
}
void TabWidget::dealdeviceFromEqui(int i)
{
    emit sendDeviceInfoToSock(i);
    device.show();
}
void TabWidget::dealSetRefeShow()
{
    setrefe.show();
    find_wg.hide();
}
void TabWidget::showUpWgList()
{
    upwglist.setGeometry(968,200,211,320);
    upwglist.show();
}

void TabWidget::on_pushButton_delete_clicked()
{
    QTableWidgetItem * itemr = ui->tableWidget->currentItem();
    if(itemr==Q_NULLPTR)return;
    int i = itemr->row();
    QString serverName = ui->tableWidget->item(i, 1)->text();//获取某一格内容
    emit senddeleteToSql(serverName,i);
}
void TabWidget::dealDealTableWidget(int i)
{
    ui->tableWidget->removeRow(i);
}
void TabWidget::timeone()
{
    adminTime -= 1;
    int hour = adminTime/3600;
    int minute = (adminTime-hour*3600)/60;
    int second = (adminTime- hour*3600 - minute*60);
    QString label_time = QString::number(hour,10);
    label_time += ":";
    label_time += QString::number(minute,10);
    label_time += ":";
    label_time += QString::number(second,10);
    ui->label_time->setText(label_time);

}
void TabWidget::timeten()
{
    int userid = user_id;
    int hour = adminTime/3600;
    int minute = (adminTime-hour*3600)/60;
    int second = (adminTime- hour*3600 - minute*60);
    QString label_time = QString::number(hour,10);
    label_time += ":";
    label_time += QString::number(minute,10);
    label_time += ":";
    label_time += QString::number(second,10);
    emit sendTab2UpdateToSql();
    emit sendChangeTimeToSql(label_time,userid);
}
void TabWidget::timerFive()
{
        qDebug()<<"定时器发信号了"<<endl;
        emit sendSelectWgToSock(gwid);
        if(gwSelect>=3)
        {
            timerfive->stop();
            gwSelect = 0;
            //网关不在线做一些处理？
            QMessageBox::about(NULL, "警告", "此网关不在线");
        }
        gwSelect++;
}
void TabWidget::stopTimeFive()
{
        qDebug()<<"定时器关闭"<<endl;
        timerfive->stop();
        gwSelect = 0;
}

void TabWidget::sendToSqlWgUpdate(QString wgname, QString servername, bool allserver, QString updateadress){
    emit sendUseridToSql(wgname,servername,allserver,updateadress,user_id);
}
void TabWidget::dealProgressBar(int idnum){
    progressDialog = new QProgressDialog(this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(0);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    progressDialog->setWindowTitle(tr("Please Wait"));
    progressDialog->setLabelText(tr("Copying..."));
    progressDialog->setCancelButtonText(tr("Cancel"));
    progressDialog->setRange(0,idnum);
    qDebug()<<"idnum数量"<<idnum<<endl;
    /*for(int i=0;i<50000;i++)
        {
            for(int j=0;j<20000;j++);
            progressDialog->setValue(i);
            if(progressDialog->wasCanceled())
                break;
        }*/
    if(progressDialog->wasCanceled())
      {
         progressDialog->setRange(0,0);
         progressDialog->setEnabled(false);
      }
}
void TabWidget::dealProgressBarChange(int num){
    qDebug()<<"设置数目"<<num<<endl;
    progressDialog->setValue(num);
}


