#include "scripdlg.h"
#include <QGridLayout>
#include <QHBoxLayout>


scripdlg::scripdlg(QWidget *parent) :
    QDialog(parent)
{

    islogin = false;
    label0 = new QLabel;
    label0->setText("请输出执行的sql脚本");
    textEditSQL = new QTextEdit;
    okBtn = new QPushButton;
    okBtn->setText("执行");
    cancelBtn = new QPushButton;
    cancelBtn->setText("取消");

    QGridLayout *layout1 = new QGridLayout(this);
    layout1->addWidget(label0, 0, 0);
    layout1->addWidget(textEditSQL, 0, 1);
    QHBoxLayout *layout2 = new QHBoxLayout;//这个layout是需要放到layout1里里面去的，所以不需要this
   layout2->addWidget(okBtn);
   layout2->addWidget(cancelBtn);

   layout1->addLayout(layout2, 1, 1);

   connect(okBtn, SIGNAL(clicked()), this, SLOT(okBtnOnclick()));
   connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnOnclick()));

}

void scripdlg::okBtnOnclick()
{

    SQL = textEditSQL->toPlainText();//TextEdit没有text函数，得到TextEdit当中用户输入内容的函数是toPlainText();
    islogin = true;
    close();
}

void scripdlg::cancelBtnOnclick()
{
    close();
}
