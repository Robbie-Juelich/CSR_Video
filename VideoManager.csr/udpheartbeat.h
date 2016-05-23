#ifndef UDPHEARTBEAT_H
#define UDPHEARTBEAT_H

#include<QtNetwork>
#include <Q3SocketDevice>
#include<QSocketNotifier>
#include"videoplayer.h"
#include "msg.h"
#include<QTimer>

#define HEART_BEAT_USE_Q3

class UdpHeartBeat : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<UdpHeartBeat> Instance() ;
    //static UdpHeartBeat *Instance();


    QHostAddress getvideoInRtcpIp() { return videoInRtcpIp; }
    quint16 getvideoInRtcpPort() { return videoInRtcpPort; }

    QHostAddress getaudioInRtcpIp() { return audioInRtcpIp; }
    quint16 getaudioInRtcpPort() { return audioInRtcpPort; }

    QHostAddress getaudioOutRtpIp() { return audioOutRtpIp; }
    quint16 getaudioOutRtpPort() { return audioOutRtpPort; }

    QHostAddress getaudioOutRtcpIp() { return audioOutRtcpIp; }
    quint16 getaudioOutRtcpPort() { return audioOutRtcpPort; }

#ifdef HEART_BEAT_USE_Q3
    qint32 getAsocket(){
        return mSocket;
    }
#endif

     void startChecking(){
         checkTimer->start();
     }
     void stopChecking(){
         checkTimer->stop();
         heartbeatTimer->stop();
     }

protected:
    UdpHeartBeat();

#ifdef HEART_BEAT_USE_Q3
     Q3SocketDevice* heartBeatQ3UdpSocket;
     QSocketNotifier *heartBeatQ3UdpNotifier;
     qint32 mSocket;
#else
    QUdpSocket *heartBeatUdpSocket;
#endif
    QHostAddress videoInRtcpIp;
    quint16  videoInRtcpPort;

    QHostAddress audioInRtcpIp;
    quint16  audioInRtcpPort;

    QHostAddress audioOutRtpIp;
    quint16  audioOutRtpPort;

    QHostAddress audioOutRtcpIp;
    quint16  audioOutRtcpPort;

    QHostAddress heartbeatOutIp;
    quint16  heartbeatOutPort;

    static QSharedPointer<UdpHeartBeat> _instance;
//    static UdpHeartBeat* _instance;
    QTimer *checkTimer;
    QDateTime  last;

    QTimer *heartbeatTimer;  // for VideoManager send heartbeat msg to ipcam

Q_SIGNALS:
    void heartBeatMsgArrived();
    void dead();

private slots:
    void readHeartBeatMessage();
    void check();
    void heartbeatCheck();
    //void errorRemoteConnection();
};

typedef QSharedPointer<UdpHeartBeat> UdpHeartBeatPtr;

#endif // UDPREGISTER_H
