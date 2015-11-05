#ifndef UDPREGISTER_H
#define UDPREGISTER_H

#include<QtNetwork>
#include"videoplayer.h"
#include "msg.h"

class UdpRegister : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<UdpRegister> Instance() ;
    //static UdpRegister *Instance();
    QHostAddress getAddress() { return regMachineIP; }
    quint16 getPort() { return regMachinePort; }
    qint32 writeData(QByteArray &array);
    void no_shangweiji_test();

protected:
    UdpRegister();
    QUdpSocket* regUdpSocket;
    QHostAddress regMachineIP;
    quint16  regMachinePort;

    static QSharedPointer<UdpRegister> _instance;
//    static UdpRegister * _instance;
Q_SIGNALS:
    void regMsgArrived();

private slots:
    void readRegisterMessage();
    void errorRemoteConnection();
    //void errorRemoteConnection();
};

typedef QSharedPointer<UdpRegister> UdpRegisterPtr;

#endif // UDPREGISTER_H
