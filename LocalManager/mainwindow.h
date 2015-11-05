#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtNetwork>
 #include <QLinkedList>
#include"msg.h"
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_ip_clicked();

public slots:
    void readServerMessage();

    void on_test(void);

private:
    Ui::MainWindow *ui;

    QUdpSocket *udpSocket;
    QMutex mutex;
    //QUdpSocket *udpVideoSocket;

    void initClient();

    msg_t msg;
    QByteArray data_block;

    QLinkedList<msg_t> msg_list; // to add the message

    QTimer timer;
//    void send_start_msg(QTcpSocket *local_socket);

    quint16 serverPort;
    QHostAddress serverIP;

    quint16 videoPort;
    QHostAddress videoIP;
};


#endif // MAINWINDOW_H
