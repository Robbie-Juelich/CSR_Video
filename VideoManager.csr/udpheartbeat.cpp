#include "udpheartbeat.h"
#include<QSharedMemory>
#include"QsLog.h"
#include "msg.h"
#include"socketports.h"

QSharedPointer <UdpHeartBeat> UdpHeartBeat::_instance =
        QSharedPointer <UdpHeartBeat>(NULL);

//UdpHeartBeat* UdpHeartBeat::_instance = NULL;

UdpHeartBeat::UdpHeartBeat(int timeout,int y_timeout):
    mTimeOut(timeout),yTimeOut(y_timeout)
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
    checkTimer->setInterval(1000);

    last = QDateTime::currentDateTime();

    connect(checkTimer, SIGNAL(timeout()), this, SLOT(check()));

    heartbeatTimer = new QTimer(this);
    heartbeatTimer->setInterval(1000);
    connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(heartbeatCheck()));

    yellowsignalTimer=new QTimer(this); //new add
    yellowsignalTimer->setInterval(1000);//yellow signal
    connect(yellowsignalTimer, SIGNAL(timeout()), this, SLOT(yellowsignalcheck() ) );
}

void UdpHeartBeat::yellowsignalcheck()
{
    if(last.secsTo(QDateTime::currentDateTime()) >= yTimeOut){
        yellowsignalTimer->stop();
        emit sYellow();
    }
}
void UdpHeartBeat::check()
{
    if (last.secsTo(QDateTime::currentDateTime()) >= mTimeOut) {
        checkTimer->stop();
        heartbeatTimer->stop(); // ok ??
        emit dead();
    }
}

void UdpHeartBeat::heartbeatCheck()
{
    mx6ip_msg_t mx6ip_msg;
    unsigned char *data = (unsigned char*)&mx6ip_msg;
    mx6ip_msg.header.fstart = 0x5;
    mx6ip_msg.header.ftype =  0xAC;
    mx6ip_msg.type = 0x0; // ?? not used now ??
    mx6ip_msg.sum = 0;
    mx6ip_msg.fend = 0x17;
    int len = sizeof(mx6ip_msg);
    for(int i = 1; i < len - 2; ++i) {
        data[len -2] += data[i];
    }
#ifdef HEART_BEAT_USE_Q3
	heartBeatQ3UdpSocket->writeBlock((char*)data, sizeof(mx6ip_msg), // or just use write ??
                                     heartbeatOutIp, heartbeatOutPort);
#else
	heartBeatUdpSocket->writeDatagram((char*)data, sizeof(mx6ip_msg),
										heartbeatOutIp, heartbeatOutPort);
#endif
	qDebug() << "Send hearbeat msg to ip: " << heartbeatOutIp
                << " port: "<< heartbeatOutPort;
}

void UdpHeartBeat::readHeartBeatMessage()
{
    msg_t msg;
    QHostAddress sender_ip;
    quint16 sender_port;

#ifdef HEART_BEAT_USE_Q3
    int byteCount = heartBeatQ3UdpSocket->bytesAvailable();
    while(byteCount > 0){
        // or use readBlock ??
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
        case 0xB4:  // new add
            heartbeatOutIp = sender_ip;
            heartbeatOutPort = sender_port;
            break;
         default:
             QLOG_DEBUG() << "new q3  Error UDP data";
             QLOG_DEBUG() << "read count " << readCount;
             break;
         }
    }

    if ((quint8)msg.mx6ip_msg.header.ftype == 0xB4) {
        if (!heartbeatTimer->isActive()) {
            heartbeatTimer->start();
        }
    } else {
        if (!checkTimer->isActive()) {
            checkTimer->start();
        }
        if(!yellowsignalTimer->isActive()){
             yellowsignalTimer->start();//new add
        }
    }

    last = QDateTime::currentDateTime();

    emit heartBeatMsgArrived();
}

QSharedPointer<UdpHeartBeat>
//UdpHeartBeat*
UdpHeartBeat::Instance(int timeout,int y_timeout)
{
    static QMutex mutex;
    if(!_instance){
        QMutexLocker locker(&mutex);
        if(!_instance){
            _instance = QSharedPointer<UdpHeartBeat>
                    (new UdpHeartBeat(timeout,y_timeout));
        }
    }
    return _instance;
}
