#include "logindlg.h"
#include <QGridLayout>
#include <QPalette>
#include <QMessageBox>


loginDlg::loginDlg(QWidget *parent):
    QDialog(parent)
{
    islogin =false;  //默认为未登录状态

    label0 = new QLabel;
   label0->setText(tr("用户名"));

   label1 = new QLabel;
   label1->setText(tr("密码"));

   label2 = new QLabel;
   label2->setText(tr("数据库名称"));

   label3 = new QLabel;
   label3->setText(tr("服务器IP"));


   lineEditUserID = new QLineEdit;

   lineEditPasswd = new QLineEdit;
   lineEditPasswd->setEchoMode(QLineEdit::Password);//设置为密码框
   lineEditDBName = new QLineEdit;
   lineEditHostIP = new QLineEdit;

   lineEditUserID->setText(tr("root"));
   lineEditPasswd->setText(tr("root"));
   lineEditDBName->setText(tr("school"));
   lineEditHostIP->setText(tr("127.0.0.1"));



   loginBtn = new QPushButton;
   loginBtn->setText("登录");
   logoutBtn = new QPushButton;
   logoutBtn->setText("取消");

   QGridLayout *layout1 = new QGridLayout(this);//重点提示，构造函数需要写this
   layout1->addWidget(label0, 0, 0);
   layout1->addWidget(lineEditUserID, 0, 1);

   layout1->addWidget(label1, 1, 0);
   layout1->addWidget(lineEditPasswd, 1, 1);

   layout1->addWidget(label2, 2, 0);
   layout1->addWidget(lineEditDBName, 2, 1);

   layout1->addWidget(label3, 3, 0);
   layout1->addWidget(lineEditHostIP, 3, 1);

   layout1->addWidget(loginBtn, 4, 0);
   layout1->addWidget(logoutBtn, 4, 1);

   layout1->setColumnStretch(0, 1);//设置0列
   layout1->setColumnStretch(1, 1);//设置1列

   layout1->setMargin(15);//设置layout边距
   layout1->setSpacing(10);//设置layout当中控件和控件之间的间距.
   layout1->setSizeConstraint(QLayout::SetFixedSize);//设置对话框大小不可改变


   setWindowTitle("登录");
   //设置背景图片
   this->setAutoFillBackground(true);
   QPalette palette;
   palette.setBrush(QPalette::Background, QBrush(QPixmap("image/bag_02.jpg")));
   setPalette(palette);

   connect(loginBtn, SIGNAL(clicked()), this, SLOT(loginBtnOnclick()));
   connect(logoutBtn, SIGNAL(clicked()), this, SLOT(logoutBtnOnclick()));



}



void loginDlg::loginBtnOnclick()
{
    userid = lineEditUserID->text();
    passwd = lineEditPasswd->text();
    dbname = lineEditDBName->text();
    hostip = lineEditHostIP->text();
    islogin = true;
    close();
}

void loginDlg::logoutBtnOnclick()
{
    close();
}

