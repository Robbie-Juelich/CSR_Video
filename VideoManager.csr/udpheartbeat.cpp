#include "udpheartbeat.h"
#include<QSharedMemory>
#include"QsLog.h"
#include "msg.h"
#include"socketports.h"

QSharedPointer <UdpHeartBeat> UdpHeartBeat::_instance =
        QSharedPointer <UdpHeartBeat>(NULL);

//UdpHeartBeat* UdpHeartBeat::_instance = NULL;

UdpHeartBeat::UdpHeartBeat()
{
    bool connected;
#ifdef HEART_BEAT_USE_Q3
    heartBeatQ3UdpSocket = new Q3SocketDevice( Q3SocketDevice::Datagram);
    connected = heartBeatQ3UdpSocket->bind (QHostAddress::Any, HEART_BEAT_PORT);
    if(connected) {
        QLOG_DEBUG() << "new q3 mx6 udp connected";
    }
    heartBeatQ3UdpNotifier = new QSocketNotifier(heartBeatQ3UdpSocket->socket(),
                                                 QSocketNotifier::Read, this, "mx6_q3_notifier");
    connect(heartBeatQ3UdpNotifier,SIGNAL(activated(int)),this,SLOT(readHeartBeatMessage()));
    mSocket = heartBeatQ3UdpSocket->socket();

#else
    heartBeatUdpSocket = new QUdpSocket(this);
    connected = heartBeatUdpSocket->bind(HEART_BEAT_PORT,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(connected) {
        QLOG_DEBUG() << "heartBeatUdpSocket [port] [" << HEART_BEAT_PORT << "]";
    }
    connect(heartBeatUdpSocket,SIGNAL(readyRead()), this, SLOT(readHeartBeatMessage()));
#endif

    checkTimer = new QTimer(this);
    checkTimer->setInterval(3000);//

    last = QDateTime::currentDateTime();

    connect(checkTimer, SIGNAL(timeout()), this, SLOT(check()));
}

void UdpHeartBeat::check()
{
    if (last.secsTo(QDateTime::currentDateTime()) >= 3) {
        checkTimer->stop();
        emit dead();
    }
}

void UdpHeartBeat::readHeartBeatMessage()
{
    msg_t msg;
    QHostAddress sender_ip;
    quint16 sender_port;

#ifdef HEART_BEAT_USE_Q3
    int byteCount = heartBeatQ3UdpSocket->bytesAvailable();
    while(byteCount > 0){
         qint32 readCount = heartBeatQ3UdpSocket->read((char*)msg.data, sizeof(msg.mx6ip_msg));
         if(readCount < 0) {
             QLOG_DEBUG() << "error read hearbeat info " <<  byteCount << "to read";
             return;
         }
         sender_ip = heartBeatQ3UdpSocket->peerAddress();
         sender_port = heartBeatQ3UdpSocket->peerPort();  // is it ok ??

#else
    int byteCount = heartBeatUdpSocket->bytesAvailable();
    while(byteCount >0){
        qint32 readCount = heartBeatUdpSocket->readDatagram((char*)msg.data,
                                                            sizeof(msg.mx6ip_msg), &sender_ip, &sender_port);
#endif
        byteCount -= readCount;
        if(videoInRtcpIp.isNull() || audioInRtcpIp.isNull() ||
                audioOutRtpIp.isNull() || audioOutRtcpIp.isNull()){
            QLOG_DEBUG() <<"Got Heart-Beat Message" <<  (int)(msg.data[1]) ;
            QLOG_DEBUG() << "From  ip " << sender_ip.toString() << " port " << (quint32)sender_port;
        }
        switch((quint8)msg.mx6ip_msg.header.ftype) {
        case 0xB0:
             videoInRtcpIp = sender_ip;
             videoInRtcpPort = sender_port;
             break;
         case 0xB1:
             audioInRtcpIp = sender_ip;
             audioInRtcpPort = sender_port;
             break;
         case 0xB2:
             audioOutRtpIp = sender_ip;
             audioOutRtpPort = sender_port;
             break;
         case 0xB3:
            audioOutRtcpIp = sender_ip;
            audioOutRtcpPort = sender_port;
            break;
         default:
             QLOG_DEBUG() << "new q3  Error UDP data";
             QLOG_DEBUG() << "read count " << readCount;
             break;
         }
    }

    if (!checkTimer->isActive()){
        checkTimer->start();
    }
    last = QDateTime::currentDateTime();

    emit heartBeatMsgArrived();
}

QSharedPointer<UdpHeartBeat>
//UdpHeartBeat*
UdpHeartBeat::Instance()
{
    static QMutex mutex;
    if(!_instance){
        QMutexLocker locker(&mutex);
        if(!_instance){
            _instance = QSharedPointer<UdpHeartBeat>
                    (new UdpHeartBeat());
        }
    }
    return _instance;
}
