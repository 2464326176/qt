#ifndef SCRIPDLG_H
#define SCRIPDLG_H
#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
class scripdlg:public QDialog
{
    Q_OBJECT
public:
    explicit scripdlg(QWidget *parent = 0);//抑制内置类型隐式转换

    QString  SQL;
    bool islogin;


private:
        QLabel *label0;
        QTextEdit *textEditSQL;
        QPushButton *okBtn;
        QPushButton *cancelBtn;
private slots:
    void okBtnOnclick();
    void cancelBtnOnclick();





};

#endif // SCRIPDLG_H
