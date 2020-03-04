#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket> //通信套接字
#include <QFile>
#include <QDateTime>
namespace Ui {
class clientwidget;
}

class clientwidget : public QWidget
{
    Q_OBJECT

public:
    explicit clientwidget(QWidget *parent = nullptr);
    ~clientwidget();

private slots:
    void on_pushButtonconnect_clicked();

    void on_pushButton_2_clicked();

    void on_pushButtonsend_clicked();

private:
    Ui::clientwidget *ui;

    QTcpSocket *tcpSocket;

    QFile *file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小
    void dealFile(QByteArray *buf);  //处理接受到的文件
    void dealFileHead(QByteArray *buf);  //处理接受到的文件
    void dealMes(QByteArray *buf);    //处理接收到的消息
    bool isStart;   //标志位，是否为头部信息
    bool file_mes;  //发送消息还是发送文件 真为消息 假为文件
    QDateTime *datetime;//当前时间


};






#endif // CLIENTWIDGET_H
