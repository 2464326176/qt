#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //监听套接字
    //初始化
    ui->pushButtonselect->setEnabled(false);
    ui->pushButtonsend->setEnabled(false);
    datetime=new QDateTime(QDateTime::currentDateTime());



    //如果客户端成功和服务器连接
    //tcpServer会自动触发 newConnection()
    tcpServer = new QTcpServer(this); //初始化

    tcpServer->listen(QHostAddress::Any, 8888);
    setWindowTitle("服务器端口为：8888");


    connect(tcpServer,&QTcpServer::newConnection,
            [=](){
        //取出建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();

        QString ip = tcpSocket->peerAddress().toString();
        quint16 port = tcpSocket->peerPort();

        QString str = QString("[%1:][%2:%3] 成功连接").arg(datetime->toString("hh:mm:ss ddd")).arg(ip).arg(port);
        ui->textEdit->setText(str); //显示到编辑区

        //成功连接后，才能按选择文件 也可以发送即时消息
        ui->pushButtonsend->setEnabled(true); //连接成功把发送按钮置为真
        ui->pushButtonselect->setEnabled(true);


        file_mes = true;  //连接成功就可以发送消息

        connect(tcpSocket,&QTcpSocket::readyRead,
                [=](){
            //取客户端信息
            QByteArray buf = tcpSocket->readAll();
            if(QString(buf)=="file done"){
                ui->textEdit->append(QString("[%1:]发送完毕").arg(datetime->toString("yyyy-MM-dd hh:mm:ss ddd")));
                file.close();
                ui->pushButtonselect->setEnabled(true);
                ui->pushButtonsend->setEnabled(true);
                file_mes = true;

            }else{
                ui->textEdit->append((QString("[%1:]%2").arg(datetime->toString("hh:mm:ss ddd")).arg(buf.data())));

            }
        });
        connect(tcpSocket,&QTcpSocket::disconnected,
                [=](){
            disconnectTcp(); //断开连接
        });

    });




    connect(&timer,&QTimer::timeout,
            [=](){

        //关闭定时器
        timer.stop();
         //false发送文件 true发送消息;
        file_mes?sendMes():sendData();

        ;
    });






}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButtonselect_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"open","../");

    if(filePath.isEmpty() == false){
        fileName.clear();
        fileSize= 0;

//        qDebug()<<info;
        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName(); //获取文件名字
        fileSize = info.size(); //获取文件大小

        sendSize = 0; //发送文件的大小

       //只读方式打开文件
       //指定文件的名字
       file.setFileName(filePath);

       //打开文件
       bool isOk = file.open(QIODevice::ReadOnly);
       if(false == isOk)
       {
           qDebug() << "只读方式打开文件失败";
       }

       //提示打开文件的路径
       ui->textEdit->append(filePath);

       ui->pushButtonselect->setEnabled(false);//文件一次发一个
       file_mes = false;

    }else{
        qDebug() << "选择文件路径出错";
    }

}

void Widget::on_pushButtonsend_clicked()
{
    if(file_mes == false){ //发文件
        ui->pushButtonsend->setEnabled(false);
        //发文件先发头文件信息   格式化 文件名##文件大小
        QString head = QString("%1##%2##%3").arg("filemark").arg(fileName).arg(fileSize);//格式化
        quint64 len = tcpSocket->write(head.toUtf8());
        if(len >0) //头部信息发送完毕
        {
            //防止TCP黏包 需要通过定时器延时 20 ms
            timer.start(20);
        }else{ //头部发送失败
            file.close();
            ui->pushButtonselect->setEnabled(true);
            ui->pushButtonsend->setEnabled(true);
        }
    }else{
        //获取编辑框内容
        QString head = "mesmark##";//发送一个标记 发送的是mes
        quint64 len = tcpSocket->write(head.toUtf8());
        if(len >0){
            timer.start(10);
//            qDebug()<<"沉睡10毫秒";
        }
    }
}



void Widget::sendData(){
    ui->textEdit->append("正在发送");
    qint64 len = 0;
     do
     {
        //每次发送数据的大小
        char buf[4*1024] = {0};
        len = 0;
        //往文件中读数据
        len = file.read(buf, sizeof(buf));
        //发送数据，读多少，发多少
        len = tcpSocket->write(buf, len);
        //发送的数据需要累积
        sendSize += len;
     }while(len > 0);
}

void Widget::sendMes(){

    QString str = ui->textEdit_write->toPlainText();
    //发送数据
    if(str.isEmpty() == false){
        qDebug()<<str;
        tcpSocket->write(str.toUtf8().data());
    }
    ui->textEdit_write->clear();

}

void Widget::disconnectTcp(){

    QString str = QString("[%1:]断开连接").arg(datetime->toString("hh:mm:ss ddd"));
    ui->textEdit->append(str); //显示到编辑区
    ui->pushButtonselect->setEnabled(false);
    ui->pushButtonsend->setEnabled(false);
}


void Widget::on_pushButtondis_clicked()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->close();

}
