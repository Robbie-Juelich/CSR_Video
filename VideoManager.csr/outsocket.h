
#ifndef OUTSOCKET_H
#define OUTSOCKET_H

#include<QtNetwork>
#include"videoplayer.h"

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
 #include<Q3SocketDevice>
#endif

class OutSocket : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<OutSocket> Instance() ;
    //static OutSocket* Instance();
    qint32 getASocket() {
        return aSocket;
    }
    qint32 getVSocket() {
        return vSocket;
    }
    ~OutSocket();

    void rebuildASocket();
    void rebuildVSocket();
    //void reopenVSocket();

public slots:
    void readAudioOutMessage();
    void readVideoOutMessage();
protected:
    OutSocket();
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    Q3SocketDevice* videoRtcpQ3Socket;
    Q3SocketDevice* audioRtcpQ3Socket;
    QSocketNotifier*  videoRtcpQ3SocketNotifier;
    QSocketNotifier*  audioRtcpQ3SocketNotifier;
#endif

    static QSharedPointer<OutSocket> _instance;
    //static OutSocket* _instance;
    qint32 aSocket;
    qint32 vSocket;
};

typedef QSharedPointer<OutSocket> OutSocketPtr;

#endif // OUTSOCKET_H

