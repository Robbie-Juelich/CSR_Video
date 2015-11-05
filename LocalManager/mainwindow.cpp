#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QMessageBox>


//#define SERVER_ADDRESS	"223.255.18.169"
#define SERVER_ADDRESS	"10.10.10.100"
#define VIDEO_ADDRESS 	"10.10.10.50"

QMutex mutex;

void MainWindow::readServerMessage()
{
    QHostAddress sender_ip;
    quint16 sender_port;
    quint32 bytes = udpSocket->pendingDatagramSize();
     char msg[1024];
    if(bytes >0){

        udpSocket->readDatagram(msg, bytes, &sender_ip, &sender_port);
        //udpSocket->readDatagram((char*)msg.data, 5, &sender_ip, &sender_port);
        //qDebug() <<"Got msg data type " <<  (int)(msg.data[1]);
        //qDebug() << "From ip:" << sender_ip.toString() << " port:" << (u_int32_t)sender_port;
        //udpSocket->readDatagram((char*)(&(msg.data[2])), sizeof(msg_length(msg) -2),
          //                              &sender_ip,  &sender_port);
        QByteArray array((char*)msg, bytes);
        for(int i = 0; i < bytes; ++i){
        //    qDebug() << (unsigned char)array[i];
        }
         udpSocket->writeDatagram(array, videoIP, videoPort);
        //QByteArray array((char*)msg.data, msg_length(msg));
      /*  {
            for(int i = 0; i < msg_length(msg); ++i){
                qDebug() << (unsigned char)array[i];
            }
            QMutexLocker locker(&mutex);

            qDebug() << "write msg to media";
            udpSocket->writeDatagram(array, videoIP, videoPort);
        }
        */
    }
}

void MainWindow::initClient()
{
    bool connected;
    serverPort = 11001;
    serverIP = QHostAddress(SERVER_ADDRESS);

    udpSocket = new QUdpSocket(this);
    connected = udpSocket->bind(11003,
                                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(connected) {
        qDebug() << "udpServerSocket  bind";
    }

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readServerMessage()));

    videoPort = 11000;
    videoIP = QHostAddress(VIDEO_ADDRESS);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label->setScaledContents(true);

    initClient();
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_test()));  // 用于测试
    timer.start(2000);
}

void MainWindow::on_test()
{
    msg_t msg;
    msg.data[1] = 0xab;
    int len = msg_length(msg);
    //qDebug() << "send register msg";
    QByteArray array((char*)msg.data, len);
    {
        QMutexLocker locker(&mutex);
        udpSocket->writeDatagram(array, serverIP, serverPort);
    }
}

MainWindow::~MainWindow()
{
    delete udpSocket;
    delete ui;
}

void MainWindow::on_pushButton_ip_clicked()
{

}
