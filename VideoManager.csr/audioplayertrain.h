#ifndef PLAYERAUDIOTRAIN_H
#define PLAYERAUDIOTRAIN_H

#include<QGst/Pipeline>
#include<QGst/Pad>
#include<QGst/Message>
#include<QtNetwork>

class AudioPlayerTrain: public QObject
{
    Q_OBJECT

public:
    AudioPlayerTrain(QObject *parent = NULL);
    ~AudioPlayerTrain();
    void initPlayer();
    QGst::State state() const;
    void stop();
    void pause();
    void play();

    void setIP(QHostAddress ip, quint16 port);
    bool ipPrepared();
    void unsetIP();
    void setSocket(int socket);

    void updateCaps(QString acaps);
    void setACaps(QString caps);

private:
    QGst::PipelinePtr m_pipeline;
    bool initialized;
    void onBusMessage(const QGst::MessagePtr & message);
    void onRtpBinPadAdded(const QGst::PadPtr & pad);
    void onRtpBinPadRemoved(const QGst::PadPtr &pad);
    void makeSinkBins();

    int m_sokcet;
    QMutex mutex;
    bool first_audio;

    QGst::ElementPtr audio_rtpsrc;
    QGst::ElementPtr audio_rtcpsink;
    QGst::BinPtr audio_bin;

    QGst::PadPtr audio_pad;
    QGst::PadPtr audio_sink_pad;

    QHostAddress audio_in_rtcp_ip;
    quint16 audio_in_rtcp_port;
};

#endif // PLAYERAUDIO_H
