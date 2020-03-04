#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

clientwidget::clientwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientwidget)
{
    ui->setupUi(this);
    ui->progress_01->setValue(0);
    ui->progress_01->hide();  //进度条  初始值为0 默认不显示
    tcpSocket = new QTcpSocket(this);
    datetime = new QDateTime(QDateTime::currentDateTime());
    isStart = false; //初始化
    ui->pushButtonsend->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    file = new QFile(); //初始化

    setWindowTitle("客户端");
    //连接成功  提示一下
    connect(tcpSocket, &QTcpSocket::connected,
        [=]()
        {
            //提示连接成功
            ui->textEdit->clear();
            ui->pushButtonsend->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            ui->textEdit->append(QString("[%1:]和服务器连接成功，等待服务器传送文件……").arg(datetime->toString("hh:mm:ss ddd")));
        }
        );
    //接收到数据处理
    connect(tcpSocket, &QTcpSocket::readyRead,
        [=]()
        {
            qDebug()<<"isStart:"<<isStart;
            //取出接收的内容
            QByteArray buf = tcpSocket->readAll();
//                qDebug()<<mark;
            //处理头部信息
            if(isStart == true){
                isStart = false;
                QString mark = QString(buf).section("##", 0, 0);
                qDebug()<<mark<<":"<<isStart;


                if(mark== "mesmark"){
                    file_mes = true;
                }else{
                    file_mes = false;
                    dealFileHead(&buf);
                }
//                qDebug()<<file_mes;
            }else{
                qDebug()<<"执行处理函数";
                file_mes?dealMes(&buf):dealFile(&buf);
            }});

        connect(tcpSocket,&QTcpSocket::disconnected,
                [=](){
            on_pushButton_2_clicked();
        });



}
//处理头部信息
void clientwidget::dealFileHead(QByteArray *buf)
{
    qDebug()<<"处理文件头部信息";
    fileName = QString(*buf).section("##", 1, 1);
    //文件大小
    fileSize = QString(*buf).section("##", 2, 2).toInt();
    recvSize = 0;   //已经接收文件大小
    //打开文件
    file->setFileName(fileName);
    //只写方式方式，打开文件
    bool isOk = file->open(QIODevice::WriteOnly);
    if(false == isOk)
    {
        qDebug() << "WriteOnly error";
        tcpSocket->disconnectFromHost(); //断开连接
        tcpSocket->close(); //关闭套接字
        return; //如果打开文件失败，中断函数
    }

    //弹出对话框，显示接收文件的信息
    QString str = QString("接收的文件:[%1:] [%2: %3kb]").arg(datetime->toString("hh:mm:ss ddd")).arg(fileName).arg(fileSize/1024);
    //QMessageBox::information(this, "文件信息", str);
    ui->textEdit->append(str);


    ui->textEdit->append(QString("[%1:]正在接收文件……").arg(datetime->toString("hh:mm:ss ddd")));
    //设置进度条
    ui->progress_01->show();
    ui->progress_01->setMinimum(0); //最小值
    ui->progress_01->setMaximum(fileSize/1024); //最大值
    ui->progress_01->setValue(0); //当前值
}

void clientwidget::dealFile(QByteArray *buf)
{
//        file = new QFile("D:/");
        qint64 len = file->write(*buf);
        if(len >0) //接收数据大于0
        {
            recvSize += len; //累计接收大小
            qDebug() << len;
        }
        //更新进度条
        ui->progress_01->setValue(recvSize/1024);
        if(recvSize == fileSize) //文件接收完毕
        {
            ui->progress_01->hide();
            //先给服务发送(接收文件完成的信息)
            tcpSocket->write("file done");

            ui->textEdit->append(QString("[%1:]文件接收完成").arg(datetime->toString("hh:mm:ss ddd")));
//            QMessageBox::information(this, "完成", "文件接收完成");
            file->close(); //关闭文件
            isStart = true;
        }

}
void clientwidget::dealMes(QByteArray *buf)
{
       //追加到编辑区中
//       qDebug()<<buf->data();
       ui->textEdit->append((QString("[%1:]%2").arg(datetime->toString("hh:mm:ss ddd")).arg(buf->data())));

       isStart = true;

}
clientwidget::~clientwidget()
{
    delete ui;
}
//连接
void clientwidget::on_pushButtonconnect_clicked()
{
    QString ipAdress = ui->lineEditip->text();
    quint16 port = ui->lineEditport->text().toInt();
    tcpSocket->connectToHost(QHostAddress(ipAdress),port);
    isStart = true; //连接成功
    file_mes = true; //默认连接成功可以发消息
    qDebug()<<"连接成功";
    //设置进度条
    ui->progress_01->setValue(0);


}

void clientwidget::on_pushButton_2_clicked()
{
    //断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    QString str = QString("[%1:]断开连接").arg(datetime->toString("hh:mm:ss ddd"));
    ui->textEdit->setText(str); //显示到编辑区
    ui->pushButtonsend->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

}

void clientwidget::on_pushButtonsend_clicked()
{

    QString str = ui->textEdit_write->toPlainText();
    //发送数据
    if(str.isEmpty() == false){
//        qDebug()<<str;
        tcpSocket->write(str.toUtf8().data());
    }
    ui->textEdit_write->clear();

}
