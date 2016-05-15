#ifndef MODESWITCH_H
#define MODESWITCH_H

#include<QtNetwork>
#include"videoplayer.h"

class SwitchCaps : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<SwitchCaps> Instance() ;
    //static SwitchCaps* Instance();
    //QString getACaps() { return a_caps;  }
    //QString getVCaps() { return v_caps;  }
    ~SwitchCaps();
protected:
    SwitchCaps();

    //QTcpServer caps_server;
    //QTcpSocket *caps_socket;
    QUdpSocket *acaps_socket;
    QUdpSocket *vcaps_socket;

    static QSharedPointer<SwitchCaps> _instance;
    //static SwitchCaps *_instance;
    QString m_acaps;
    QString m_vcaps;

Q_SIGNALS:
    void capsArrived(int type, QString caps);

private slots:
    void errorRemoteConnection();
    void readCapsMessage();
};

typedef QSharedPointer<SwitchCaps> SwitchCapsPtr;

#endif // MODESWITCH_H
