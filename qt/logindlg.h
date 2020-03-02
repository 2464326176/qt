#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class loginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit loginDlg(QWidget *parent = 0);

signals:

public slots:

private:
    QLabel *label0, *label1, *label2, *label3;
    QLineEdit *lineEditUserID, *lineEditPasswd, *lineEditDBName, *lineEditHostIP;
    QPushButton *loginBtn, *logoutBtn;

private slots:
    void loginBtnOnclick();
    void logoutBtnOnclick();

public:
    QString userid;  //用户名字
    QString passwd;  //密码
    QString dbname;  //数据库名称
    QString hostip;  //IP地址
    bool islogin;

};



#endif // LOGINDLG_H
