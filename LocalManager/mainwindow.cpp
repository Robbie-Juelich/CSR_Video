#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QMessageBox>

//#define SERVER_ADDRESS	"223.255.18.169"
//#define SERVER_ADDRESS	"192.168.0.86"
//#define VIDEO_ADDRESS 	"192.168.0.10"

QMutex mutex;

QString SERVER_ADDRESS;  // address of the manager
QString VIDEO_ADDRESS;    // address of the ip camra

void MainWindow::readServerMessage()
{
    QHostAddress sender_ip;
    quint16 sender_port;
    while(udpSocket->hasPendingDatagrams()) {
        qint32 bytes = udpSocket->pendingDatagramSize();
        QByteArray datagram;
        datagram.resize(bytes);
        char *msg = datagram.data();
        qint32 num = udpSocket->readDatagram(msg, bytes, &sender_ip, &sender_port);
        //qDebug() <<"Got msg data type " <<  (int)(msg.data[1]);
        qDebug() << "From ip:" << sender_ip.toString() << " port:" << (u_int32_t)sender_port;
        for(int i = 0; i < (int)num; ++i){
            qDebug() << (unsigned char)datagram[i];
        }
        QMutexLocker locker(&mutex);  // do not need
        //int ret = udpSocket->writeDatagram(array, QHostAddress::Broadcast, videoPort);
        int ret = udpSocket->writeDatagram(datagram, videoIP, videoPort);
        if (ret < 0) {
            qDebug() << "send msg error";
        }
        //udpSocket->flush();
    }
}

void MainWindow::uninitClient()
{
    timer.stop();
    udpSocket->close();
    disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(readServerMessage()));
}

bool MainWindow::initClient()
{
    bool connected;

    videoPort = 11000;
    videoIP = QHostAddress(VIDEO_ADDRESS);
    serverPort = 11001;
    serverIP = QHostAddress(SERVER_ADDRESS);

    if (videoIP.isNull() || serverIP.isNull()) {
        QMessageBox::warning(this, QString::fromLocal8Bit("IP地址输入错误"),
                             QString::fromLocal8Bit("IP地址输入错误"));
        return false;
    }

    connected = udpSocket->bind(11003,
                                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(connected) {
        qDebug() << "udpServerSocket  bind";
        qDebug() << "IPCam addr: " << videoIP;
        qDebug() << "ManagerServer addr: " << serverIP;
    } else {
        QMessageBox::warning(this, QString::fromLocal8Bit("UDP绑定错误"),
                             QString::fromLocal8Bit("UDP绑定错误"));
        return false;
    }

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readServerMessage()));
    timer.start(2000);
    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label->setScaledContents(true);

    udpSocket = new QUdpSocket(this);

//    initClient();
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_test()));  // 用于测试
    m_started = false;
    ui->pushButton_start->setText(QString::fromLocal8Bit("启动"));
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

void MainWindow::on_pushButton_start_clicked()
{
    if (!m_started) {
        SERVER_ADDRESS = ui->lineEdit->text();
        VIDEO_ADDRESS = ui->lineEdit_2->text();
        if (!initClient()) {
            return;
        }
        ui->pushButton_start->setText(QString::fromLocal8Bit("关闭"));
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        m_started = true;
    } else {
        uninitClient();
        ui->pushButton_start->setText(QString::fromLocal8Bit("启动"));
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        m_started = false;
    }
}
