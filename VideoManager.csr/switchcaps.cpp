#include "switchcaps.h"
#include<QSharedMemory>
#include"QsLog.h"
#include "msg.h"
#include"socketports.h"

QSharedPointer<SwitchCaps> SwitchCaps::_instance =
        QSharedPointer<SwitchCaps>(NULL);

SwitchCaps::SwitchCaps()
{
    if(!caps_server.listen(QHostAddress::Any,  CAPS_PORT)) {
        QLOG_ERROR() << "[SwitchCaps] Listen to port " << CAPS_PORT << "error";
    }
    QLOG_DEBUG() << "[SwitchCaps] listen to port " << CAPS_PORT << "ok";
    connect(&caps_server, SIGNAL(newConnection()), this,
            SLOT(acceptCapsConnection()));//当发现新连接时，发出newconnection()信号
    caps_socket = NULL;// QSharedPointer<QTcpSocket>(NULL);
}

SwitchCaps::~SwitchCaps()
{
    if(caps_socket)
        delete caps_socket;
}

void SwitchCaps::acceptCapsConnection()
{
    QLOG_DEBUG() << "[SwitchCaps] new connection comes 1" ;
    caps_socket = //QSharedPointer<QTcpSocket>
            (caps_server.nextPendingConnection());

    connect(caps_socket, SIGNAL(readyRead()),
              this, SLOT(readCapsMessage()));
    connect(caps_socket, SIGNAL(error(QAbstractSocket::SocketError)),
              this,SLOT(errorRemoteConnection()));
    QLOG_DEBUG() << "[SwitchCaps] new connection comes set ready ok";
}

void SwitchCaps::errorRemoteConnection()
{
    QLOG_DEBUG() << "[SwitchCaps] error connection";
}

void SwitchCaps::readCapsMessage()
{
    avcaps_msg_t msg;
    int bytes = caps_socket->bytesAvailable();
    QLOG_DEBUG() << "[SwitchCaps] bytes : " << bytes;
    int len = 0;
    while(bytes >0) {
        len = caps_socket->read((char*)(&msg), bytes);
		if(len >0) bytes -= len;
		else QLOG_DEBUG() << "error read caps msg";
		bytes -= len;
    }
	
    msg.vcaps[msg.vcaps_len] = '\0';
    msg.acaps[msg.acaps_len] = '\0';
    v_caps = QString(msg.vcaps);
    a_caps = QString(msg.acaps);
    QLOG_DEBUG() << "[SwitchCaps] get vcaps : " << v_caps;
    QLOG_DEBUG() << "[SwitchCaps] get acaps : " << a_caps;
    emit capsArrived();
}


QSharedPointer<SwitchCaps> SwitchCaps::Instance()
{
    static QMutex mutex;
    if(!_instance){
        QMutexLocker locker(&mutex);
        if(!_instance){
            _instance = QSharedPointer<SwitchCaps>
                    (new SwitchCaps());
        }
    }
    return _instance;
}
