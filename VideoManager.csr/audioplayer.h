#ifndef PLAYERAUDIO_H
#define PLAYERAUDIO_H

#include <QGst/Pipeline>
#include<QGst/Pad>
#include<QGst/Message>
#include<QtNetwork>

class AudioPlayer: public QObject
{
    Q_OBJECT
public:
    AudioPlayer();
    ~AudioPlayer();
    void initPlayer();
    QGst::State state() const;
    void stop();
    void pause();
    void play();

    void setIP(QHostAddress ip, quint16 port, int id);
    bool ipPrepared();
    void unsetIP();
    void setSocket(int socket);

    void updateCaps(QString acaps);

private:
    QGst::PipelinePtr m_pipeline;
    bool initialized;
    void onBusMessage(const QGst::MessagePtr & message);

    QGst::ElementPtr audio_rtp_sink;
    QGst::ElementPtr audio_rtcp_sink;
    QHostAddress audio_out_rtp_ip;

    QHostAddress audio_out_rtcp_ip;
    quint16 audio_out_rtp_port;
    quint16 audio_out_rtpc_port;
    int m_sokcet;
    QMutex mutex;
    bool first_audio;
};

#endif // PLAYERAUDIO_H
