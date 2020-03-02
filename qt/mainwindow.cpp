#include "mainwindow.h"
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QIcon>
#include <QStandardItemModel>
#include <QTableView>
#include "logindlg.h"
#include "scripdlg.h"
#include <QDebug>
#include <string.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("简单的学校信息系统");
    memset(sqldata,0,sizeof(sqldata));
    midArea = new QMdiArea;
    setCentralWidget(midArea);
    midArea->setBackground(Qt::NoBrush);
    midArea->setStyleSheet("background-image: url(image/main.jpg);");//背景设置为main.jpg，子窗口大小可调整


    //当子窗口的范围查过父窗口的显示范围时，父窗口自动添加横向滚动条。
    midArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //当子窗口的范围查过父窗口的显示范围时，父窗口自动添加纵向滚动条。
    midArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setWindowIcon(QIcon("image/head.jpg"));//设置窗口光标

    createActions();
    createMenus();

}

MainWindow::~MainWindow()
{
}

void MainWindow::showview()
{
    QStandardItemModel *model = new QStandardItemModel(5, 3);//要建立一个5行3列的

    //设置modul的列头名称
    model->setHeaderData(0, Qt::Horizontal, "姓名");
    model->setHeaderData(1, Qt::Horizontal, "性别");
    model->setHeaderData(2, Qt::Horizontal, "年龄");
    //设置modul的每一个单元格的内容
    model->setData(model->index(0, 0, QModelIndex()), "张三");
    model->setData(model->index(0, 1, QModelIndex()), "男");
    model->setData(model->index(0, 2, QModelIndex()), "20");

    model->setData(model->index(1, 0, QModelIndex()), "李四");
    model->setData(model->index(1, 1, QModelIndex()), "男");
    model->setData(model->index(1, 2, QModelIndex()), "30");

    model->setData(model->index(2, 0, QModelIndex()), "王五");
    model->setData(model->index(2, 1, QModelIndex()), "女");
    model->setData(model->index(2, 2, QModelIndex()), "10");

    model->setData(model->index(3, 0, QModelIndex()), "赵六");
    model->setData(model->index(3, 1, QModelIndex()), "女");
    model->setData(model->index(3, 2, QModelIndex()), "40");

    model->setData(model->index(4, 0, QModelIndex()), "王麻子");
    model->setData(model->index(4, 1, QModelIndex()), "男");
    model->setData(model->index(4, 2, QModelIndex()), "30");

    QTableView *view1 = new QTableView;
    view1->setAttribute(Qt::WA_DeleteOnClose);//view在close的时候自动会delete，这个时候如果view有modul的话，这个modul会被view自动释放。
    midArea->addSubWindow(view1);
    view1->setStyleSheet("border-image: url(3.jpg);");//设置widget背景图片

    //view1继承自widget，如果没有modul，那么view不会显示任何数据.
    view1->setModel(model);
    view1->show();
    midArea->activeSubWindow()->resize(width() - 100, height() - 100);
}

void MainWindow::showsub()
{
    QWidget *w1 = new QWidget;
    w1->setAttribute(Qt::WA_DeleteOnClose);//代表关闭这个widget的时候，自动将这个widget delete。
    midArea->addSubWindow(w1);
    w1->setWindowTitle("SDFSDFDS");
    //w1->setStyleSheet("background-image: url(3.jpg);");//设置widget背景图片

    w1->setStyleSheet("border-image: url(image/timg.jpg);");//设置widget背景图片
    w1->show();
    midArea->activeSubWindow()->resize(width() - 100, height() - 100);//设置widget窗口的大小

}


//关闭对话框
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button = QMessageBox::question(this, "提示",
                                                                   "是否退出程序",
                                                                   QMessageBox::Yes | QMessageBox::No);
        //    qDebug()<<button;
        if (button == QMessageBox::Yes)
        {
            event->accept();//如果用户点击yes，接受退出
        }else
        {
            event->ignore();//否则，不接受退出
        }

}

//顶部菜单栏
void MainWindow::createMenus(){
    adminMenu = menuBar()->addMenu(tr("管理")); //添加管理菜单
    adminMenu->addAction(loginAction);
    adminMenu->addAction(logoutAction);
    adminMenu->addSeparator();  //添加
    adminMenu->addAction(exitAction);

    dataMenu = menuBar()->addMenu(tr("数据"));//添加数据菜单
    dataMenu->addAction(scriptAction);

    windowMenu=menuBar()->addMenu(tr("窗口"));
    windowMenu->addAction(cascadeAction);
    windowMenu->addAction(titleAction);

    helpMenu=menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);

}




//管理菜单的行为
void MainWindow::createActions(){
    loginAction = new QAction(tr("登录"),this);
    loginAction->setShortcut(tr("Ctrl+u"));
    connect(loginAction,SIGNAL(triggered()),this,SLOT(on_login()));

    logoutAction = new QAction(tr("注销"),this);
    logoutAction->setShortcut(tr("Ctrl+b"));
    connect(logoutAction,SIGNAL(triggered()),this,SLOT(on_logout()));

    exitAction = new QAction(tr("退出"),this);
    exitAction->setShortcut(tr("Ctrl+q"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(on_exit()));


    scriptAction = new QAction(tr("执行脚本"),this);
    scriptAction->setShortcut(tr("ctrl+p"));
    scriptAction->setEnabled(false);//默认不可用登录才可用
    connect(scriptAction,SIGNAL(triggered()),this,SLOT(on_script()));




    cascadeAction = new QAction(tr("层叠"), this);
    cascadeAction->setShortcut(tr("Ctrl+Q"));
    connect(cascadeAction, SIGNAL(triggered()), this, SLOT(cascadeSubWindows()));


    titleAction = new QAction(tr("并列"), this);
    titleAction->setShortcut(tr("Ctrl+r"));
    connect(titleAction, SIGNAL(triggered()), this, SLOT(tileSubWindows()));

    helpAction = new QAction(tr("帮助"), this);
    helpAction->setShortcut(tr("Ctrl+s"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(on_help()));

    aboutAction = new QAction(tr("关于"), this);
    aboutAction->setShortcut(tr("Ctrl+t"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(on_about()));




}

//登录
void MainWindow::on_login(){
//    QMessageBox::information(this,"","登录");
    loginDlg dlg; //实例化对话框
    dlg.exec();  //调用exec 产生一个模式对话框，程序在exec这个地方阻塞了
    if(dlg.islogin){
//        qDebug()<<dlg.hostip.toStdString().data();
        int res = db.sql_connect(dlg.hostip.toStdString().data(),
                       dlg.userid.toStdString().data(),
                       dlg.passwd.toStdString().data(),
                       dlg.dbname.toStdString().data());
        if(res == -1){

            QMessageBox::information(this, "登录失败",db.geterror());

        }else{
            QMessageBox::information(this, "","登录成功");
            scriptAction->setEnabled(true);
            //展示相应的数据库结构

            //模型 存放数据
            //视图 展示数据

            //将school数据库中所有表的结构信息打印出来
            char sql_2[] = "select table_name,column_name, data_type,column_comment from information_schema.columns where table_schema = 'school'";

//            strcpy(sqldata,"select ");
            QStandardItemModel *modul = NULL;

            res = db.sql_open(sql_2, &modul);
            QTableView *view_0 = new QTableView;
            view_0->setAttribute(Qt::WA_DeleteOnClose);//view在close的时候自动会delete，这个时候如果view有modul的话，这个modul会被view自动释放。
            midArea->addSubWindow(view_0);
            view_0->setStyleSheet("border-image: url(image/timg.JPG);");//设置widget背景图片
            //view1继承自widget，如果没有modul，那么view不会显示任何数据.
            view_0->setModel(modul);
            view_0->show();
            midArea->activeSubWindow()->resize(width() - 100, height() - 100);


        }
    }

}
//注销
void MainWindow::on_logout(){
    QMessageBox::information(this,"","注销");
}

//退出
void MainWindow::on_exit(){

    QMessageBox::information(this,"","退出");
}



void MainWindow::on_script()
{

    scripdlg dlg;//申请scriptDlg类的实例

    dlg.exec();//模式对话框,阻塞

    if (dlg.islogin)//如果用户点击了执行按钮才执行下面的代码
    {
        script_msg(dlg.SQL.toStdString().data());
    }

    //showview();

}

void MainWindow::script_msg(const char *SQL){
    int res = 0;
    //判断是否为查询函数
    if ((strncmp(SQL, "SELECT", 6) == 0) || (strncmp(SQL, "select", 6) == 0))
        {
            //模型 存放数据
            //视图 展示数据
            QStandardItemModel *modul = NULL;
            res = db.sql_open(SQL, &modul);
            QTableView *view1 = new QTableView;
            view1->setAttribute(Qt::WA_DeleteOnClose);//view在close的时候自动会delete，这个时候如果view有modul的话，这个modul会被view自动释放。
            midArea->addSubWindow(view1);
            view1->setStyleSheet("border-image: url(image/timg.JPG);");//设置widget背景图片

            //view1继承自widget，如果没有modul，那么view不会显示任何数据.
            view1->setModel(modul);

            view1->show();
            midArea->activeSubWindow()->resize(width() - 100, height() - 100);

        }else
        {
            res = db.sql_exec(SQL);//如果用户执行的是非SELECT。那么执行sql_exec函数
        }

        if (res == -1)
        {
            QMessageBox::information(this, "执行失败", db.geterror());
        }else
        {
            QMessageBox::information(this, "提示", "执行成功");
        }
}


void MainWindow::cascadeSubWindows()
{
    midArea->cascadeSubWindows();
}

void MainWindow::tileSubWindows()
{
    midArea->tileSubWindows();
}

void MainWindow::on_help()
{
    QMessageBox::information(this, "帮助", "帮助");
}

void MainWindow::on_about()
{
    QMessageBox::about(this, "关于", "版权所有");
}






















