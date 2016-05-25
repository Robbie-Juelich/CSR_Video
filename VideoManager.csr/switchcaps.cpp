#include "switchcaps.h"
#include<QSharedMemory>
#include"QsLog.h"
#include "msg.h"
#include"socketports.h"

QSharedPointer<SwitchCaps> SwitchCaps::_instance =
        QSharedPointer<SwitchCaps>(NULL);

SwitchCaps::SwitchCaps()
{
    bool connected;

    vcaps_socket = new QUdpSocket(this);
    connected = vcaps_socket->bind(VCAPS_PORT,
                                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!connected) {
        QLOG_ERROR() << "[SwitchCaps] Bind to port " << VCAPS_PORT << "error";
    }

    acaps_socket = new QUdpSocket(this);
    connected = acaps_socket->bind(ACAPS_PORT,
                                   QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!connected) {
        QLOG_ERROR() << "[SwitchCaps] Bind to port " << ACAPS_PORT << "error";
    }
    connect(vcaps_socket, SIGNAL(readyRead()), this, SLOT(readCapsMessage()));
    connect(vcaps_socket, SIGNAL(error(QAbstractSocket::SocketError)),
              this,SLOT(errorRemoteConnection()));
    connect(acaps_socket, SIGNAL(readyRead()), this, SLOT(readCapsMessage()));
    connect(acaps_socket, SIGNAL(error(QAbstractSocket::SocketError)),
              this,SLOT(errorRemoteConnection()));
}

SwitchCaps::~SwitchCaps()
{
    if (acaps_socket)
        delete acaps_socket;
    if (vcaps_socket)
        delete vcaps_socket;
}

void SwitchCaps::errorRemoteConnection()
{
    QLOG_DEBUG() << "[SwitchCaps] caps_socket error connection";
}

void SwitchCaps::readCapsMessage()
{
    QUdpSocket* udpSocket = qobject_cast<QUdpSocket*>(this->sender());
    QHostAddress sender_ip;
    quint16 sender_port;
    quint16 recv_port;

    while(udpSocket->hasPendingDatagrams()) {
        // Returns the size of the first pending UDP datagram.
        qint32 bytes = udpSocket->pendingDatagramSize();
        Q_ASSERT(bytes == sizeof(avcaps_msg_t)); // notice
        QByteArray datagram;
        datagram.resize(bytes);
        char *msg = (datagram.data());
        // Receives a datagram no larger than maxSize bytes
        qint32 num = udpSocket->readDatagram(msg, bytes, &sender_ip, &sender_port);
        recv_port = udpSocket->localPort();
        qDebug() << "From ip:" << sender_ip.toString() << " port:" << (u_int32_t)sender_port
                    << " recv_port:" << recv_port;
        avcaps_msg_t * avcaps_msg = (avcaps_msg_t*)msg;
        QString caps(avcaps_msg->caps.caps_str);
        if (avcaps_msg->caps.type == 0) { // video
            if (caps != m_vcaps) {  // if caps changed, then update the pipeline
                m_vcaps = caps;
                unsigned int width = avcaps_msg->caps.v_width;
                unsigned int height = avcaps_msg->caps.v_height;
                emit capsArrived(0, caps, width, height);
                QLOG_DEBUG() << "[SwitchCaps] get vcaps : " << caps;
            }
        } else { // audio
            if (caps != m_acaps) {
                m_acaps = caps;
                emit capsArrived(1, caps, 0, 0);
                QLOG_DEBUG() << "[SwitchCaps] get acaps : " << caps;
            }
        }
    }
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
