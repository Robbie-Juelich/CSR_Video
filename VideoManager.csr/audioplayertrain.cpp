#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Query>
#include <QGst/ClockTime>
#include <QGst/Event>
#include <QGst/StreamVolume>
#include <QsLog.h>
#include<QGst/GhostPad>

#include "socketports.h"
#include "audioplayertrain.h"
#include "msg.h"

using namespace QsLogging;

AudioPlayerTrain::AudioPlayerTrain(QObject* parent)
    : QObject(parent)
{
    initialized = false;
}

AudioPlayerTrain::~AudioPlayerTrain()
{
    m_pipeline->setState(QGst::StateNull);
}

void AudioPlayerTrain::onBusMessage(const QGst::MessagePtr & message)
{
    switch (message->type()) {
    //case QGst::Message
    case QGst::MessageEos: //End of stream. We reached the end of the file.
        stop();
        break;
    case QGst::MessageError: //Some error occurred.
        QLOG_ERROR() <<"MessageError comes" ;
        QLOG_ERROR()  << message.staticCast<QGst::ErrorMessage>()->error();
        //rebuildPipeline();
        //play();
        break;
    default:
        break;
    }
}


void AudioPlayerTrain::setSocket(int socket)
{
    if(initialized) {
#ifdef WIN32
        int port = socket;
        audio_rtp_sink->setProperty("bind-port", port);
        audio_rtcp_sink->setProperty("bind-port", port);
#else
        audio_rtcpsink->setProperty("sockfd", socket);
        audio_rtcpsink->setProperty("closefd", false);
#endif
        QLOG_DEBUG() << "audio socket set to " << socket;
    }
}


void AudioPlayerTrain::makeSinkBins()
{
    if((!audio_bin.isNull())) {
        QLOG_INFO() << "Bin already build, exit";
        m_pipeline->add(audio_bin);
        return;
    }

#if defined G722
    QGst::ElementPtr rtppcmadepay = QGst::ElementFactory::make("rtpg722depay", "rtp pcmadepay");
    if(!rtppcmadepay) {
        qFatal("Failed to create rtppcmadepay");
    }
#ifdef WIN32
    QGst::ElementPtr alawdec = QGst::ElementFactory::make("ffdec_g722", "rtp g722");
    if(!alawdec) {
        qFatal("Failed to create ffdec_g722");
    }
#else
    QGst::ElementPtr alawdec = QGst::ElementFactory::make("avdec_g722", "rtp g722");
    if(!alawdec) {
        qFatal("Failed to create avdec_g722");
    }
#endif
#else
    QGst::ElementPtr rtppcmadepay = QGst::ElementFactory::make("rtppcmadepay", "rtp pcmadepay");
    if(!rtppcmadepay) {
        QLOG_FATAL()  << "Failed to create rtppcmadepay";
    }
    QGst::ElementPtr alawdec = QGst::ElementFactory::make("alawdec", "rtp alawdec");
    if(!alawdec) {
        QLOG_FATAL() << "Failed to create alawdec";
    }
#endif

    QGst::ElementPtr audioresample = QGst::ElementFactory::make("audioresample", "rtp audioresample");
    if(!audioresample) {
         QLOG_FATAL() << "Failed to create alawdec";
    }
    QGst::ElementPtr audioconvert = QGst::ElementFactory::make("audioconvert");
    if(!audioconvert) {
        QLOG_FATAL()  << "Failed to create audioconvert";
    }
    QGst::ElementPtr audio_filter = QGst::ElementFactory::make("capsfilter", "audio filter");
    if(!audio_filter) {
        QLOG_FATAL()  << "Failed to create audio_filter";
    }
    audio_filter->setProperty("caps", QGst::Caps::fromString(
                                  "audio/x-raw-int, endianness=(int)1234, signed=(boolean)true, width=(int)16, "
                                  "depth=(int)16, rate=(int)44100, channels=(int)1"));

#if WIN32
    QGst::ElementPtr alsasink = QGst::ElementFactory::make("directsoundsink", "directsoundsink");
    if(!alsasink) {
        QLOG_FATAL()  <<  "Failed to create alsasink";
    }
    alsasink->setProperty("sync", false);
    alsasink->setProperty("async", false);
#else
    QGst::ElementPtr alsasink = QGst::ElementFactory::make("alsasink", "rtp alsasink");
    if(!alsasink) {
        QLOG_FATAL()  <<  "Failed to create alsasink";
    }
    alsasink->setProperty("sync", false);
    alsasink->setProperty("async", false);
#endif

    audio_bin = QGst::Bin::create();
    audio_bin->add(rtppcmadepay, alawdec, audioresample,
             audioconvert, audio_filter, alsasink);
    QGst::Bin::linkMany(rtppcmadepay, alawdec, audioresample,
                        audioconvert, audio_filter, alsasink);
    audio_bin->addPad(QGst::GhostPad::create(rtppcmadepay->getStaticPad("sink"), "sink"));
    audio_bin->setName("audio_bin");

    audio_sink_pad = audio_bin->getStaticPad("sink");
    m_pipeline->add(audio_bin);
}


void AudioPlayerTrain::unsetIP()
{
    this->stop();
    audio_in_rtcp_ip = QHostAddress::Null;
    audio_in_rtcp_port = QHostAddress::Null;
}


void AudioPlayerTrain::setIP(QHostAddress ip,quint16 port)
{
    audio_in_rtcp_ip = ip;
    audio_in_rtcp_port = port;
    if(initialized){
        audio_rtcpsink->setProperty("host", audio_in_rtcp_ip.toString());
        audio_rtcpsink->setProperty("port", (int)audio_in_rtcp_port);
    }
}

void AudioPlayerTrain::updateCaps(QString acaps)
{
    QMutexLocker locker(&mutex);
    QGst::State s = state();

    m_pipeline->setState(QGst::StateNull);

    setACaps(acaps);

    m_pipeline->setState(QGst::StatePlaying);
}

void AudioPlayerTrain::setACaps(QString caps)
{
    audio_rtpsrc->setProperty("caps", QGst::Caps::fromString(caps));
}

bool AudioPlayerTrain::ipPrepared()
{
    return  (!audio_in_rtcp_ip.isNull());
}

void AudioPlayerTrain::onRtpBinPadAdded(const QGst::PadPtr & pad)
{
    QGst::BinPtr bin;
    QGst::PadPtr sink_pad;

    QLOG_DEBUG() << "AudioPlayerTrain::onRtpBinPadAdded " << pad->name() ;
  //  ::memcpy(name , pad->name().toStdString().c_str(), strlen(pad->name().toStdString().c_str()));
    //name = pad->name();
    try {
        bin = audio_bin;
        if(audio_pad)
            audio_pad->unlink(audio_sink_pad);  // useless ??
        audio_pad = pad;
        sink_pad = audio_sink_pad;
    } catch (const QGlib::Error & error) {
        QLOG_ERROR() << error;
        QLOG_FATAL() << "One ore more required elements are missing. Aborting...";
    }

    if(!sink_pad) {
        QLOG_ERROR() << "error sink_pad";
    }
    bin->syncStateWithParent();
    pad->link(sink_pad);
}

void AudioPlayerTrain::onRtpBinPadRemoved(const QGst::PadPtr &pad)
{
    QLOG_DEBUG()  << "Player::onRtpBinPadRemoved " << pad->name() ;
    //this->stopPipelineWatch();
}

void AudioPlayerTrain::initPlayer()
{
    if(initialized) {
        return;
    }

    bool ret = false;
    m_pipeline = QGst::Pipeline::create();

    QGst::ElementPtr rtpbin = QGst::ElementFactory::make("gstrtpbin");
    if(!rtpbin){
        QLOG_FATAL() << "Failed to create rtpbin_audio";
    }
    m_pipeline->add(rtpbin);

    bool audio_enable = true;

    if(audio_enable){
        //////////////////////////////////////////////////////////////
        /// rtp receiving audio
        int rtp_in_port = PORT_BASE + 2;
        int rtcp_in_port = PORT_BASE + 3;


#if defined G722
        QString caps("application/x-rtp, media=(string)audio, clock-rate=(int)8000, "
                    "encoding-name=(string)G722, encoding-params=(string)1, channels=(int)1, "
                    "ssrc=(uint)3162804502, payload=(int)96, clock-base=(uint)938612046, "
                    "seqnum-base=(uint)47532");
#else
        QString caps("application/x-rtp, media=(string)audio, clock-rate=(int)8000, "
                    "encoding-name=(string)PCMA, payload=(int)8, ssrc=(uint)794824693, "
                    "clock-base=(uint)3373301493, seqnum-base=(uint)30873");
#endif
        QGst::ElementPtr rtpudpsrc_audio = QGst::ElementFactory::make("udpsrc", "audio udpsrc");
        if(!rtpudpsrc_audio) {
            QLOG_FATAL() <<"Failed to create udpsrc_audio";
        }
        rtpudpsrc_audio->setProperty("port",  rtp_in_port);
        rtpudpsrc_audio->setProperty("caps", QGst::Caps::fromString(caps));
        audio_rtpsrc = rtpudpsrc_audio;
        m_pipeline->add(rtpudpsrc_audio);
        ret = rtpudpsrc_audio->link(rtpbin, "recv_rtp_sink_1");
        if(!ret) {
            QLOG_FATAL() << "Failed to link rtpudpsrc_audio and recv_rtp_sink_1";
        }
        //////////////////////////////////////////////////////////
        /// recv sink will be added when the pad becomes available
        /// onRtpBinPadAdded

        /////////////////////////////////////////////////////////
        /// rtcp connection
        // recv
        QGst::ElementPtr rtcpudpsrc_audio = QGst::ElementFactory::make("udpsrc", "rtcpudpsrc_audio");
        if(!rtcpudpsrc_audio) {
            QLOG_FATAL() << "Failed to create rtcpudpsrc_audio";
        }
        rtcpudpsrc_audio->setProperty("port",  rtcp_in_port);
        m_pipeline->add(rtcpudpsrc_audio);
        ret = rtcpudpsrc_audio->link(rtpbin, "recv_rtcp_sink_1");
        if(!ret){
            QLOG_FATAL() << "failed to link rtcpudpsrc_audio and recv_rtcp_sink_1";
        }

        // send
        QGst::ElementPtr rtcpudpsink_audio = QGst::ElementFactory::make("udpsink", "rtcpudpsink_audio");
        if(!rtcpudpsink_audio) {
            QLOG_FATAL() <<"Failed to create rtcpudpsink_audio";
        }
        //rtcpudpsink_audio->setProperty("host", ip);
        audio_rtcpsink = rtcpudpsink_audio;
        //rtcpudpsink_audio->setProperty("port", rtcp_out_port);
        rtcpudpsink_audio->setProperty("sync", false);
        rtcpudpsink_audio->setProperty("async", false);
        m_pipeline->add(rtcpudpsink_audio);
        ret = rtpbin->link("send_rtcp_src_1", rtcpudpsink_audio);
        if(!ret){
            QLOG_FATAL() << "failed to link send_rtcp_src_1 and udpsink";
        }
    }  // the audio pipeline

    makeSinkBins();

    QGlib::connect(rtpbin, "pad-added", this, &AudioPlayerTrain::onRtpBinPadAdded);
    QGlib::connect(rtpbin, "pad-removed", this, &AudioPlayerTrain::onRtpBinPadRemoved);

    //watchPipeline(m_pipeline);

    QGst::BusPtr bus = m_pipeline->bus();
    bus->addSignalWatch();
    QGlib::connect(bus, "message", this, &AudioPlayerTrain::onBusMessage);
    initialized = true;
}

QGst::State AudioPlayerTrain::state() const
{
    return m_pipeline ? m_pipeline->currentState() : QGst::StateNull;
}

//
#ifdef WIN32
#include <Windows.h>
#else
#include<sys/stat.h>
#endif

void AudioPlayerTrain::play()
{
    if(!ipPrepared()){
        return;
    }

    if (m_pipeline) {

        QMutexLocker locker(&mutex);
        m_pipeline->setState(QGst::StatePlaying);
    }
}

void AudioPlayerTrain::pause()
{
    if (m_pipeline) {
        QMutexLocker locker(&mutex);
        m_pipeline->setState(QGst::StatePaused);
    }
}

void AudioPlayerTrain::stop()
{
    if (m_pipeline) {
            QMutexLocker locker(&mutex);
//        m_pipeline->setState(QGst::StateNull);
             m_pipeline->setState(QGst::StatePaused);
        //once the pipeline stops, the bus is flushed so we will
        //not receive any StateChangedMessage about this.
        //so, to inform the ui, we have to emit this signal manually.
        //Q_EMIT stateChanged();
    }
}
