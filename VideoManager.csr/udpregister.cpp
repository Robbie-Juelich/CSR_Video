#include "udpregister.h"
#include<QSharedMemory>
#include"QsLog.h"
#include "msg.h"
#include"socketports.h"


QSharedPointer <UdpRegister> UdpRegister::_instance =
        QSharedPointer <UdpRegister>(NULL);

//UdpRegister* UdpRegister::_instance = NULL;

UdpRegister::UdpRegister()
{
    regUdpSocket = new QUdpSocket(this);
    bool connected = regUdpSocket->bind(REGISTER_PORT,
                                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(connected) {
        QLOG_DEBUG() << "regUdpSocket  bind";
    }
    connect(regUdpSocket, SIGNAL(readyRead()), this, SLOT(readRegisterMessage()));
}


void UdpRegister::no_shangweiji_test()
{
    // 某些测试情况下 没有上位机  直接发送命令给视频主机
    regMachineIP.setAddress("192.168.0.10");
    regMachinePort = REMOTE_PORT;
    emit(regMsgArrived());
}

#define REPEAT_COUNT 5

qint32 UdpRegister::writeData(QByteArray& array)
{
    if(regMachineIP.isNull()){
        return -1;
    }
    qint32 bytes;
    static QMutex mutex;
    {
        QMutexLocker locker(&mutex);
        /*
        for(int i = 0; i < array.length(); ++i){
            qDebug() << (unsigned char)array[i];
        }*/
        for (int i = 0;  i < REPEAT_COUNT; ++i) { // to ensure message is sent
            bytes = regUdpSocket->writeDatagram(array, regMachineIP, regMachinePort);
            QLOG_DEBUG() << "regUdpSocket write " << bytes << " bytes, repeat " << i << "times";
        }
    }
    return bytes;
}

void UdpRegister::readRegisterMessage()
{
    msg_t msg;
    QHostAddress sender_ip;
    quint16 sender_port;

    QLOG_DEBUG() <<"Got register data ";
    while(regUdpSocket->hasPendingDatagrams()){
        regUdpSocket->readDatagram((char*)msg.data, 2,
                                     &sender_ip,  &sender_port);
        if( regMachineIP.isNull()){
            QLOG_DEBUG() <<"Got register data type " <<  (int)(msg.data[1]);
            QLOG_DEBUG() << "From ip:" << sender_ip.toString() << " port:" << (u_int32_t)sender_port;
        }
        regMachineIP.setAddress(sender_ip.toIPv4Address()); // 上位机地址  id_msg
        regMachinePort = sender_port;

        regUdpSocket->readDatagram((char*)(msg.data), sizeof(msg_length(msg)- 2),
                                        &sender_ip,  &sender_port);
    }
    emit regMsgArrived();
}

void UdpRegister::errorRemoteConnection()
{
    QLOG_DEBUG() << "[SwitchCaps] error connection";
}

QSharedPointer<UdpRegister>
//UdpRegister*
UdpRegister::Instance()
{
    static QMutex mutex;
    if(!_instance){
        QMutexLocker locker(&mutex);
        if(!_instance){
            _instance = QSharedPointer<UdpRegister>
                    (new UdpRegister());
        }
    }
    return _instance;
}
