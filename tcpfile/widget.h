#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通讯套接字
#include <QFile>
#include <QTimer>
#include <QDateTime>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void sendData(); //发送文件数据
    void sendMes(); //发送消息
    void disconnectTcp();//断开连接
private slots:
    void on_pushButtonselect_clicked();
    
    void on_pushButtonsend_clicked();
    
    void on_pushButtondis_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已发送文件的大小
    bool file_mes;  //发送消息还是发送文件 真为消息 假为文件
    QTimer timer; //定时器
    QDateTime *datetime;//当前时间
};
#endif // WIDGET_H














