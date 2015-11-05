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
    QString getACaps() { return a_caps;  }
    QString getVCaps() { return v_caps;  }
    ~SwitchCaps();
protected:
    SwitchCaps();

    QTcpServer caps_server;
    QTcpSocket *caps_socket;

    static QSharedPointer<SwitchCaps> _instance;
    //static SwitchCaps *_instance;
    QString a_caps;
    QString v_caps;

Q_SIGNALS:
    void capsArrived();

private slots:
    void acceptCapsConnection();
    void readCapsMessage();
    void errorRemoteConnection();
};

typedef QSharedPointer<SwitchCaps> SwitchCapsPtr;

#endif // MODESWITCH_H
