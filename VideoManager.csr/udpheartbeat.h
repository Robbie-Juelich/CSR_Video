#ifndef UDPHEARTBEAT_H
#define UDPHEARTBEAT_H

#include<QtNetwork>
#include "config.h"
#ifdef HEART_BEAT_USE_Q3
#include <Q3SocketDevice>
#include<QSocketNotifier>
#else
#include<QUdpSocket>
#endif
#include"videoplayer.h"
#include "msg.h"
#include<QTimer>

const int default_heart_timeout = 10;

//const int default_yellow_timeout=10;
const int default_yellow_timeout=5;
class UdpHeartBeat : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<UdpHeartBeat> Instance(int timeout,int y_timeout) ;
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
     void stopYellowsignalcheck()
     {
         yellowsignalTimer->stop();
     }

/*   void setTimeOut(int y_timeout,int y_timeout) {
         yTimeOut = y_timeout;
         yTimeOut=y_timeout;
     }
     */
     void setTimeOut(int timeout,int y_timeout) {
            mTimeOut = timeout;
            yTimeOut=y_timeout;
     }

protected:
    UdpHeartBeat(int timeout,int y_timeout);

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

    int mTimeOut;
    int yTimeOut;
    QTimer *yellowsignalTimer; //new add

Q_SIGNALS:
    void heartBeatMsgArrived();
    void dead();

    void sYellow();//new add

private slots:
    void readHeartBeatMessage();
    void check();
    void heartbeatCheck();

    void yellowsignalcheck();//add
    //void errorRemoteConnection();
};

typedef QSharedPointer<UdpHeartBeat> UdpHeartBeatPtr;

#endif // UDPREGISTER_H
