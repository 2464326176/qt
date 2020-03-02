#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QCloseEvent>
#include <QMdiArea>
#include "mymysql.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);//再退出之前自动调用


private:
    void script_msg(const char *SQL);

    mymysql db; //mysql实例化一个db
    char sqldata[1024];
    void showview();
    void showsub();



    void createMenus();
    void createActions();
    QMdiArea *midArea;

    QMenu *adminMenu;
    QMenu *dataMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QAction *loginAction;
    QAction *logoutAction;
    QAction *exitAction;

    QAction *scriptAction;
    QAction *cascadeAction;
    QAction *titleAction;

    QAction *helpAction;
    QAction *aboutAction;




private slots:
    void on_login();
    void on_logout();
    void on_exit();

    void on_script();

    void cascadeSubWindows();
    void tileSubWindows();

    void on_help();
    void on_about();














};
#endif // MAINWINDOW_H
