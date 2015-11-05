#include"outsocket.h"
#include"udpheartbeat.h"
#include"socketports.h"

#include"QsLog.h"

#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include <arpa/inet.h>

//OutSocket*
QSharedPointer<OutSocket> OutSocket::_instance = QSharedPointer<OutSocket>(NULL);

OutSocket::OutSocket()
{
	aSocket = -1;
	vSocket = -1;
    rebuildVSocket();
    rebuildASocket();
}

void OutSocket::rebuildVSocket()
{
	if(vSocket >0){
	    ::close(vSocket);
	}

    struct sockaddr_in addr_vsocket;
//    vSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    vSocket = ::socket(AF_INET, SOCK_DGRAM,  IPPROTO_UDP);

    if(vSocket < 0){
        QLOG_FATAL() << "Error create vSocket";
    }
    QLOG_DEBUG() << "vSocket rebuild to " << vSocket;
    memset(&addr_vsocket, 0, sizeof(addr_vsocket));
    addr_vsocket.sin_family = AF_INET;/*协议族*/
    addr_vsocket.sin_port = htons(VIDEO_OUT_PORT);/*本地端口号*/
    addr_vsocket.sin_addr.s_addr = (INADDR_ANY); /*任意本地地址*/
    if(::bind(vSocket, (struct sockaddr *)&addr_vsocket, sizeof(addr_vsocket)) <0) {
        QLOG_FATAL() << "Error bind vSocket";
    }
}

void OutSocket::rebuildASocket()
{
	if(aSocket >0) {
        return; // do not modify audio
	    ::close(aSocket);
	}

    struct sockaddr_in addr_asocket;/*本地的地址信息*/
//    aSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    aSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(aSocket < 0){
        QLOG_FATAL() << "Error create aSocket";
    }
    QLOG_DEBUG() << "aSocket rebuild to " << aSocket;

    memset(&addr_asocket, 0, sizeof(addr_asocket));
    addr_asocket.sin_family = AF_INET;/*协议族*/
    addr_asocket.sin_port = htons(AUDIO_OUT_PORT);/*本地端口号*/
    addr_asocket.sin_addr.s_addr = (INADDR_ANY); /*任意本地地址*/
    if(::bind(aSocket, (struct sockaddr *)&addr_asocket, sizeof(addr_asocket)) <0) {
        QLOG_FATAL() << "Error bind aSocket";
    }
}

void OutSocket::readAudioOutMessage()
{
    char msg[10];
    int len;
    int bytes = audioRtcpQ3Socket->bytesAvailable();
    while(bytes > 0){
        len = audioRtcpQ3Socket->read(msg, 10);
        bytes -= len;
    }
}

void OutSocket::readVideoOutMessage()
{
    char msg[10];
    int len;
    int bytes = videoRtcpQ3Socket->bytesAvailable();
    while(bytes > 0){
        QLOG_DEBUG() << "zzc readVideoOutMessage ";
        len = videoRtcpQ3Socket->read(msg, 10);
        bytes -= len;
    }
}

OutSocket::~OutSocket()
{
    ::close(aSocket);
    ::close(vSocket);
#if  (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if(videoRtcpQ3Socket)
        delete videoRtcpQ3Socket;
    if(audioRtcpQ3Socket)
        delete audioRtcpQ3Socket;
#endif
}

QSharedPointer<OutSocket>
//OutSocket*
OutSocket::Instance()
{
    static QMutex mutex;
    if(!_instance){
        QMutexLocker locker(&mutex);
        if(!_instance){
            _instance = QSharedPointer<OutSocket>
                    (new OutSocket());
        }
    }
    return _instance;
}

