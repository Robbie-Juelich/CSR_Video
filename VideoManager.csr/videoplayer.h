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
#ifndef PLAYER_H
#define PLAYER_H

#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QGst/Pipeline>
#include<QGst/Pad>
#include<QtNetwork>
#include <QGst/Ui/VideoWidget>

class VideoPlayer : public QGst::Ui::VideoWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void initPlayer();

    bool ipPrepared();
    void setIP(QHostAddress ip, unsigned short port);
    void unsetIP();

    void setSocket(int port);

    void updateCaps(QString vcaps);

    QGst::State state() const;

    void updateVideoSrcCaps(int mode);

    void toChangeMode(int mode) { toset_mode = mode; }

    int getCurMode()  {  return cur_mode; }

    void changeMode() {
        if(cur_mode) cur_mode = 0;
        else cur_mode = 1;
    }

    QString name;

public Q_SLOTS:
    void play();
    void pause();
    void stop();
    //void capsChanged();

Q_SIGNALS:
//    void positionChanged();
    void stateChanged();

private:
    void onBusMessage(const QGst::MessagePtr & message);
    void handlePipelineStateChange(const QGst::StateChangedMessagePtr & scm);
    QGst::ElementPtr createVideoBin();
    void onRtpBinPadAdded(const QGst::PadPtr & pad);
    void onRtpBinPadRemoved(const QGst::PadPtr &pad);
    void makeSinkBins();
    void rebuildPipeline();


    void setACaps(QString caps);
    void setVCaps(QString caps);

    QHostAddress video_in_rtcp_ip;

    quint16 video_in_rtcp_port;
    QString caps_single;
    QString caps_full;

    QGst::PipelinePtr m_pipeline;
    //QGst::PipelinePtr m_pipeline2;
    QGst::ElementPtr video_rtpsrc;
    QGst::ElementPtr video_rtcpsink;

    QGst::BinPtr video_bin;

    QGst::PadPtr video_pad;
    QGst::PadPtr video_sink_pad;
    bool initialized;
//    QTimer m_positionTimer;
    int m_socket;
    int cur_mode;
    int toset_mode;
    QMutex mutex;
};

#endif
