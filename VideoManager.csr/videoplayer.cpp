/*
    Copyright (C) 2010 Marco Ballesio <gibrovacco@gmail.com>
    Copyright (C) 2011 Collabora Ltd.
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include <QGst/GhostPad>
#include <QsLog.h>
#include "switchcaps.h"
#include "socketports.h"
#include "videoplayer.h"
#include"outsocket.h"
#include "msg.h"
#include"udpheartbeat.h"
using namespace QsLogging;
//#define G722

VideoPlayer::VideoPlayer(QWidget *parent)
    : QGst::Ui::VideoWidget(parent)
{
    //this timer is used to tell the ui to change its position slider & label
    //every 100 ms, but only when the pipeline is playing
//    connect(&m_positionTimer, SIGNAL(timeout()), this, SIGNAL(positionChanged()));
    initialized = false;

#if 0  // 720 576
    caps_single = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, "
                        "sprop-parameter-sets=(string)\"Z0JAHqaAtBJkAA\\=\\=\\,aM4wpIAA\", ssrc=(uint)1538901578, "
                       	"payload=(int)96, clock-base=(uint)4087165916, seqnum-base=(uint)64031";
    caps_full =   "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, "
            			"sprop-parameter-sets=(string)\"Z0JAHqaAtBJkAA\\=\\=\\,aM4wpIAA\", ssrc=(uint)1538901578, "
            			"payload=(int)96, clock-base=(uint)4087165916, seqnum-base=(uint)64031";
#else	// 640 480

    caps_single = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, "
						"sprop-parameter-sets=(string)\"Z0JAHqaAoD2QAA\\=\\=\\,aM4wpIAA\", payload=(int)96, ssrc=(uint)2711179733, "
						"clock-base=(uint)641475754, seqnum-base=(uint)4164";

    caps_full = caps_single;
/*
    caps_full =  "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, "
                        "sprop-parameter-sets=(string)\"Z0JAHqaAoD2QAA\\=\\=\\,aM4wpIAA\", payload=(int)96, ssrc=(uint)3314852113, "
                        "clock-base=(uint)3517802442, seqnum-base=(uint)60459";
    caps_single = caps_full;
*/

    caps_full =  "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264,"
                        "sprop-parameter-sets=(string)\"Z0JAHqaAoD2QAA\\=\\=\\,aM4wpIAA\", payload=(int)96, ssrc=(uint)54501220, "
                        "clock-base=(uint)2275598026, seqnum-base=(uint)34480";
    caps_single = caps_full;

#endif

    cur_mode = 0x0;
    toset_mode = 0x0;
    Q_EMIT stateChanged();
}

VideoPlayer::~VideoPlayer()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
        stopPipelineWatch();
    }
}

void VideoPlayer::setSocket(int socket)
{
    m_socket = socket;

    if(initialized){
#if WIN32
        int port = socket;
        video_rtcpsink->setProperty("bind-port", port);
        audio_rtcpsink->setProperty("bind-port", port);
#else
        video_rtcpsink->setProperty("sockfd", socket);
        video_rtcpsink->setProperty("closefd", false);

#endif
        QLOG_DEBUG() << "video player set socket to " << socket;
    }
}

void VideoPlayer::unsetIP()
{
    this->stop();
    video_in_rtcp_ip = QHostAddress::Null;
}

void VideoPlayer::setIP(QHostAddress ip, quint16 port)
{
    video_in_rtcp_ip = ip;
    video_in_rtcp_port = port;
    if(initialized){
        video_rtcpsink->setProperty("host", video_in_rtcp_ip.toString());
        video_rtcpsink->setProperty("port", (int)video_in_rtcp_port);
    }
}

void VideoPlayer::updateCaps(QString vcaps)
{
    QMutexLocker locker(&mutex);
    QGst::State s = state();

    m_pipeline->setState(QGst::StateNull);

    setVCaps(vcaps);

//    if(s != QGst::StateNull){
        m_pipeline->setState(QGst::StatePlaying);
//    }
    cur_mode = toset_mode;
}

void VideoPlayer::setVCaps(QString caps)
{
    video_rtpsrc->setProperty("caps", QGst::Caps::fromString(caps));
}


bool VideoPlayer::ipPrepared()
{
    return (!video_in_rtcp_ip.isNull()); // && (!audio_in_rtcp_ip.isNull());
}

void VideoPlayer::makeSinkBins()
{
    if((!video_bin.isNull())) {
        QLOG_INFO() << "Bin already build, exit";
        m_pipeline->add(video_bin);
        return;
    }

#if WIN32
    // windows gstreamer 1.0
    video_bin = QGst::Bin::fromDescription(
        "rtph264depay ! avdec_h264 ! d3dvideosink sync=false async=false"
    );
#else // LINUX gstreamer 0.1
    video_bin = QGst::Bin::fromDescription(
        "rtph264depay ! ffdec_h264 ! xvimagesink sync=false async=false"
    );
#endif
	if(!video_bin){
		QLOG_FATAL() << "video bin error";
	}
			QLOG_TRACE() << "video bin ok";

	video_bin->setName("video_bin");

    video_sink_pad = video_bin->getStaticPad("sink");
    m_pipeline->add(video_bin);
}

void VideoPlayer::onRtpBinPadRemoved(const QGst::PadPtr &pad)
{
    QLOG_DEBUG()  << "Player::onRtpBinPadRemoved " << pad->name() ;
    //this->stopPipelineWatch();
}

void VideoPlayer::onRtpBinPadAdded(const QGst::PadPtr & pad)
{
    QGst::BinPtr bin;
    QGst::PadPtr sink_pad;

    QLOG_DEBUG() << "Player::onRtpBinPadAdded " << pad->name() ;
    name = pad->name();

    try {
        bin = video_bin;

        if(video_pad)
            video_pad->unlink(video_sink_pad); // useless
        sink_pad = video_sink_pad;
        video_pad = pad;

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

void VideoPlayer::rebuildPipeline()
{
    if (m_pipeline) {
        m_pipeline->setState(QGst::StateNull);
        stopPipelineWatch();
    }

    initialized = false;
    initPlayer();
    setIP(video_in_rtcp_ip, video_in_rtcp_port);
    QLOG_INFO() << "[pipeline]To rebuild the pipeline";
}

void VideoPlayer::initPlayer()
{
    if(initialized) {
        return;
    }  // if already initialized return

    bool video_enable = true;

    bool ret = false;
    QString ip("192.168.0.10");// notice here;

    m_pipeline = QGst::Pipeline::create();
#ifdef WIN32
    QGst::ElementPtr rtpbin = QGst::ElementFactory::make("rtpbin");
#else
	QGst::ElementPtr rtpbin = QGst::ElementFactory::make("gstrtpbin");
#endif
	if(!rtpbin){
        QLOG_FATAL()  << "Failed to create gstrtpbin";
    }
    m_pipeline->add(rtpbin);

    ///////////////////////////////////////////////////////////////// video
    if(video_enable){
        int rtp_in_port = PORT_BASE;  // 5004
        int rtcp_in_port = PORT_BASE + 1;
        //int rtp_out_port = 5006;  // no source
        //int rtcp_out_port = 5008;
        //////////////////////////////////////////////////////////////
        /// rtp receiving video (in)
        QGst::ElementPtr rtpudpsrc_video = QGst::ElementFactory::make("udpsrc", "udpsrc_video");
       if(!rtpudpsrc_video){
            QLOG_FATAL() << "Failed to create udpsrc";
        }
        rtpudpsrc_video->setProperty("port", rtp_in_port);
        rtpudpsrc_video->setProperty("sync", false);

        /*
        QString caps("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, "
                   "sprop-parameter-sets=(string)\"Z0JAHqaAtBJkAA\\=\\=\\,aM4wpIAA\", ssrc=(uint)1538901578, "
                   "payload=(int)96, clock-base=(uint)4087165916, seqnum-base=(uint)64031");
        */
        QString caps(caps_single);
        rtpudpsrc_video->setProperty("caps", QGst::Caps::fromString(caps));
        m_pipeline->add(rtpudpsrc_video);
        video_rtpsrc = rtpudpsrc_video;
        ret = rtpudpsrc_video->link(rtpbin, "recv_rtp_sink_0");
        if(!ret) {
            QLOG_FATAL() << "Failed to link rtpudpsrc and  recv_rtp_sink_0";
        }
        //////////////////////////////////////////////////////////
        /// recv sink will be added when the pad becomes available
        /// onRtpBinPadAdded

        /////////////////////////////////////////////////////////
        /// rtcp connection
        ///   udpsrc port=5005 ! rtpbin.recv_rtcp_sink_0
        ///   rtpbin.send_rtcp_src_0 ! udpsink host=127.0.0.1 port=5008 sync=false async=false
        QGst::ElementPtr rtcpudpsrc_video = QGst::ElementFactory::make("udpsrc", "rtcp udp src");
        if(!rtcpudpsrc_video){
            QLOG_FATAL() << "Failed to create rtcpudpsrc_video";
        }
        rtcpudpsrc_video->setProperty("port",  rtcp_in_port);
        m_pipeline->add(rtcpudpsrc_video);
        ret = rtcpudpsrc_video->link(rtpbin, "recv_rtcp_sink_0");
        if(!ret) {
            QLOG_FATAL() << "Failed link  rtcpudpsrc ->recv_rtcp_sink_0";
        }
        QGst::ElementPtr rtcpudpsink_video = QGst::ElementFactory::make("udpsink", "rtcp udp sink");
        if(!rtcpudpsink_video){
            QLOG_FATAL() << "Failed to create rtcpudpsink_video";
        }
        //rtcpudpsink_video->setProperty("host", video_in_rtcp_ip.toString());
        video_rtcpsink = rtcpudpsink_video;
        //rtcpudpsink_video->setProperty("port", rtcp_out_port);
        rtcpudpsink_video->setProperty("sync", false);
        rtcpudpsink_video->setProperty("async", false);
        m_pipeline->add(rtcpudpsink_video);
        ret = rtpbin->link("send_rtcp_src_0", rtcpudpsink_video);
        if(!ret) {
            QLOG_FATAL() << "Failed link  send_rtcp_src_0 ->rtcpudpsink";
        }
    } // video pipeline

    makeSinkBins();

    QGlib::connect(rtpbin, "pad-added", this, &VideoPlayer::onRtpBinPadAdded);
    QGlib::connect(rtpbin, "pad-removed", this, &VideoPlayer::onRtpBinPadRemoved);
    watchPipeline(m_pipeline);

    QGst::BusPtr bus = m_pipeline->bus();
    bus->addSignalWatch();
    QGlib::connect(bus, "message", this, &VideoPlayer::onBusMessage);
    initialized = true;

	QLOG_TRACE() << "video pipeline initialized";
}

QGst::State VideoPlayer::state() const
{
    return m_pipeline ? m_pipeline->currentState() : QGst::StateNull;
}

void VideoPlayer::play()
{
    if(!ipPrepared()) {
        return;
    }
    if (m_pipeline) {
        QMutexLocker locker(&mutex);

//       OutSocketPtr socket = OutSocket::Instance();
//        socket->rebuildVSocket();
//        setSocket(socket->getVSocket());

        m_pipeline->setState(QGst::StatePlaying);
    }
}

void VideoPlayer::pause()
{
    if (m_pipeline) {
        QMutexLocker locker(&mutex);
        m_pipeline->setState(QGst::StatePaused);
    }
}

void VideoPlayer::stop()
{
    if (m_pipeline) {
        QMutexLocker locker(&mutex);
        m_pipeline->setState(QGst::StateNull);

		
        //once the pipeline stops, the bus is flushed so we will
        //not receive any StateChangedMessage about this.
        //so, to inform the ui, we have to emit this signal manually.
        //this->setVisible(false);
        //stopPipelineWatch(); // ok??
        Q_EMIT stateChanged();
    }
}

void VideoPlayer::onBusMessage(const QGst::MessagePtr & message)
{
    switch (message->type()) {
    //case QGst::Message
    case QGst::MessageEos: //End of stream. We reached the end of the file.
        stop();
        break;
    case QGst::MessageError: //Some error occurred.
        QLOG_ERROR() <<"MessageError comes";
        QLOG_ERROR()  << message.staticCast<QGst::ErrorMessage>()->error();
        //rebuildPipeline();
        //play();
        break;
    case QGst::MessageStateChanged: //The element in message->source() has changed state
        if (message->source() == m_pipeline) {
            handlePipelineStateChange(message.staticCast<QGst::StateChangedMessage>());
        }
        break;
    default:
        break;
    }
}

void VideoPlayer::handlePipelineStateChange(const QGst::StateChangedMessagePtr & scm)
{
    switch (scm->newState()) {
    case QGst::StatePlaying:
        //start the timer when the pipeline starts playing
        //m_positionTimer.start(100);
        break;
    case QGst::StatePaused:
        //stop the timer when the pipeline pauses
        if(scm->oldState() == QGst::StatePlaying) {
            //m_positionTimer.stop();
        }
        break;
    default:

        break;
    }

    Q_EMIT stateChanged();
}
