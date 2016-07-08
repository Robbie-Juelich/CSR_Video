#include"outsocket.h"
#include"udpheartbeat.h"
#include"socketports.h"

#include"QsLog.h"


//OutSocket*
QSharedPointer<OutSocket> OutSocket::_instance = QSharedPointer<OutSocket>(NULL);

OutSocket::OutSocket()
{
#if   (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    bool connected;
    // to avoid some problems with heartbeat
    // use another socket
    audioRtcpQ3Socket = new Q3SocketDevice(Q3SocketDevice::Datagram);
    connected = audioRtcpQ3Socket->bind (QHostAddress::Any, AUDIO_OUT_PORT);
    if(connected) {
        aSocket = audioRtcpQ3Socket->socket();
        QLOG_DEBUG() << "audio socket [" <<  aSocket << " : " << AUDIO_OUT_PORT << "]";
    }
    audioRtcpQ3SocketNotifier  = new QSocketNotifier(audioRtcpQ3Socket->socket(),
                                                                                    QSocketNotifier::Read, this, "audioRtcpQ3Socket");
//   connect(audioRtcpQ3SocketNotifier, SIGNAL(activated(int)), this,
//           SLOT(readAudioOutMessage()));

    videoRtcpQ3Socket = new Q3SocketDevice(Q3SocketDevice::Datagram);
    connected = videoRtcpQ3Socket->bind (QHostAddress::Any, VIDEO_OUT_PORT);
    if(connected) {
        vSocket = videoRtcpQ3Socket->socket();
        QLOG_DEBUG() << "video socket [ " <<  vSocket << " : " << VIDEO_OUT_PORT << " ]";
    }
    videoRtcpQ3SocketNotifier  = new QSocketNotifier(videoRtcpQ3Socket->socket(),
                                                                                    QSocketNotifier::Read, this, "videoRtcpQ3Socket");
//    connect(videoRtcpQ3SocketNotifier,SIGNAL(activated(int)),this,
//            SLOT(readVideoOutMessage()));

#else
    aSocket = AUDIO_OUT_PORT;
    vSocket = VIDEO_OUT_PORT;
#endif
}

void OutSocket::readAudioOutMessage()
{
#if   (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    char msg[10];
    int len;
    int bytes = audioRtcpQ3Socket->bytesAvailable();
    while(bytes > 0){
        len = audioRtcpQ3Socket->read(msg, 10);
        bytes -= len;
    }
#endif
}

void OutSocket::readVideoOutMessage()
{
#if   (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    char msg[10];
    int len;
    int bytes = videoRtcpQ3Socket->bytesAvailable();
    while(bytes > 0){
        QLOG_DEBUG() << "zzc readVideoOutMessage ";
        len = videoRtcpQ3Socket->read(msg, 10);
        bytes -= len;
    }
#endif
}

OutSocket::~OutSocket()
{
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

