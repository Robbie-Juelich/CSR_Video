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

#include "socketports.h"
#include "audioplayer.h"
#include "msg.h"

using namespace QsLogging;

AudioPlayer::AudioPlayer()
{
    initialized = false;
    first_audio = true;
}

AudioPlayer::~AudioPlayer()
{
    m_pipeline->setState(QGst::StateNull);
}

void AudioPlayer::onBusMessage(const QGst::MessagePtr & message)
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


void AudioPlayer::setSocket(int socket)
{
    if(initialized) {
#ifdef WIN32
        int port = socket;
        audio_rtp_sink->setProperty("bind-port", port);
        audio_rtcp_sink->setProperty("bind-port", port);
#else
        audio_rtp_sink->setProperty("sockfd", socket);
        audio_rtcp_sink->setProperty("sockfd", socket);
        audio_rtp_sink->setProperty("closefd", false);
        audio_rtcp_sink->setProperty("closefd", false);
#endif
        QLOG_DEBUG() << "audio socket set to " << socket;
    }
}

void AudioPlayer::unsetIP()
{
    this->stop();
    audio_out_rtp_ip = QHostAddress::Null;
    audio_out_rtcp_ip = QHostAddress::Null;
}

void AudioPlayer::setIP(QHostAddress ip,quint16 port, int id)
{
    //QLOG_TRACE() << "audio ip : " << ip.toString() << " port: " << port;
    if(id == 0x0){
        audio_out_rtp_ip = ip;
        audio_rtp_sink->setProperty("host", ip.toString());
        audio_rtp_sink->setProperty("port", (int)port);
    }
    else if(id == 0x1){
        audio_out_rtcp_ip = ip;
        audio_rtcp_sink->setProperty("host", ip.toString());
        audio_rtcp_sink->setProperty("port",(int)port);
    }
    else {
        QLOG_ERROR() << "Error ip info\n";
    }
}

bool AudioPlayer::ipPrepared()
{
    return  (!audio_out_rtcp_ip.isNull() && !audio_out_rtp_ip.isNull());
}


void AudioPlayer::initPlayer()
{
    if(initialized) {
        return;
    }
    //int rtp_in_port = 5004;
    int rtcp_in_port =  PORT_BASE + 4; //5001;
    //int rtp_out_port = 5002;  // no source
    //int rtcp_out_port = 5003;

    //int mtu = 1024;
    bool ret = false;
//    QString ip("192.168.0.10");// notice here;

    m_pipeline = QGst::Pipeline::create();
#ifdef WIN32
    QGst::ElementPtr rtpbin_audio = QGst::ElementFactory::make("rtpbin");
#else
    QGst::ElementPtr rtpbin_audio = QGst::ElementFactory::make("gstrtpbin");
#endif
    if(!rtpbin_audio){
        QLOG_FATAL() << "Failed to create rtpbin_audio";
    }
    m_pipeline->add(rtpbin_audio);

//    QGst::BinPtr src_bin = QGst::Bin::fromDescription("audiotestsrc is-live=1 ! "
//   " alawenc ! rtppcmapay mtu=1024 ");
#if 0
    QGst::BinPtr src_bin = QGst::Bin::fromDescription("audiotestsrc is-live=1 ! "
                                                      " audioconvert ! audioresample ! "
                                                      " ffenc_g722 bitrate=8000 ! rtpg722pay mtu=1024 ");
#else
	#ifdef WIN32
	QGst::BinPtr src_bin = QGst::Bin::fromDescription("directsoundsrc ! "
                                                      " audioconvert ! audioresample ! "
                                                      " avenc_g722 bitrate=8000 ! rtpg722pay mtu=1024 ");
	#else
	QGst::BinPtr src_bin = QGst::Bin::fromDescription("alsasrc ! "
                                                      " audioconvert ! audioresample ! "
                                                      " ffenc_g722 bitrate=8000 ! rtpg722pay mtu=1024 ");
	#endif

#endif
	if(!src_bin) {
		QLOG_FATAL() << "error create sound src bin";
	}
	
	m_pipeline->add(src_bin);
    QGst::ElementPtr rtp_udpsink = QGst::ElementFactory::make("udpsink");
    if(!rtp_udpsink){
        QLOG_FATAL() << "Failed to create rtp_udpsink";
    }
    //rtp_udpsink->setProperty("host", ip);
    //rtp_udpsink->setProperty("host",ip );
    //rtp_udpsink->setProperty("port", rtp_out_port);
    audio_rtp_sink = rtp_udpsink;

    m_pipeline->add(rtp_udpsink);

    QGst::ElementPtr rtcp_udpsink = QGst::ElementFactory::make("udpsink");
    if(!rtcp_udpsink){
        QLOG_FATAL() << "Failed to create rtp_udpsink";
    }
    //rtcp_udpsink->setProperty("host", ip);
    //rtcp_udpsink->setProperty("port", rtcp_out_port);
    audio_rtcp_sink = rtcp_udpsink;
    rtcp_udpsink->setProperty("sync", false);
    rtcp_udpsink->setProperty("async", false);
    //rtcp_udpsink->setProperty("sync", true);
    //rtcp_udpsink->setProperty("async", true);
    m_pipeline->add(rtcp_udpsink);

    QGst::ElementPtr rtcp_udpsrc = QGst::ElementFactory::make("udpsrc");
    if(!rtcp_udpsrc){
        QLOG_FATAL() << "Failed to create rtp_udpsink";
    }
    rtcp_udpsrc->setProperty("port",  rtcp_in_port);
    m_pipeline->add(rtcp_udpsrc);

    ret = src_bin->link(rtpbin_audio, "send_rtp_sink_0");
    if(!ret) {
        QLOG_FATAL() << "Failed to link src_bin send_rtp_sink_0";
    }
    ret = rtpbin_audio->link("send_rtp_src_0", rtp_udpsink);
    if(!ret) {
         QLOG_FATAL() << "Failed to link  send_rtp_sink_0 rtp_udpsink";
    }
    ret = rtpbin_audio->link("send_rtcp_src_0", rtcp_udpsink);
    if(!ret) {
        QLOG_FATAL() << "Failed to link  send_rtcp_src_0 rtcp_udpsink";
    }
    ret = rtcp_udpsrc->link(rtpbin_audio, "recv_rtcp_sink_0");

    QGst::BusPtr bus = m_pipeline->bus();
    bus->addSignalWatch();
    QGlib::connect(bus, "message", this, &AudioPlayer::onBusMessage);
    initialized = true;
}

QGst::State AudioPlayer::state() const
{
    return m_pipeline ? m_pipeline->currentState() : QGst::StateNull;
}

//
#ifdef WIN32
#include <Windows.h>
#else
#include<sys/stat.h>
#endif

void AudioPlayer::play()
{
    if(!ipPrepared()){
        return;
    }

    if (m_pipeline) {
        if(first_audio) {
#ifdef WIN32
			::Sleep(2000);
#else
            ::sleep(1);
#endif
			first_audio = false;
        }
        QMutexLocker locker(&mutex);
        m_pipeline->setState(QGst::StatePlaying);
    }
}

void AudioPlayer::pause()
{
    if (m_pipeline) {
        QMutexLocker locker(&mutex);
        m_pipeline->setState(QGst::StatePaused);
    }
}

void AudioPlayer::stop()
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
