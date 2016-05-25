#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtNetwork>
#include <QThread>
#include"videoplayer.h"
#include"audioplayer.h"
#include"audioplayertrain.h"
#include "msg.h"
#include"udpregister.h"
#include"udpheartbeat.h"
#include"switchcaps.h"
#include "sendmsg.h"
#include"outsocket.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool connectValid();
    bool ipPrepared();

//public slots:


private slots:
    void responseRegMsg();

    void responseCapsMsg(int type, QString caps, unsigned int w, unsigned int h);

    void responseVideoHeartBeatMsg();

    void onStateChanged();

    void on_test();

    void on_pushButton_test_on_clicked();

    void on_pushButton_test_off_clicked();

    void on_pushButton_link_remote_clicked();

    void on_pushButton_video_on_clicked();

    void on_pushButton_video_off_clicked();

    void on_pushButton_one_ch_clicked();

    void on_pushButton_multi_ch_clicked();

    void on_pushButton_audio_on_clicked();

    void on_pushButton_audio_off_clicked();

    void on_pushButton_all_off_clicked();

    void on_pushButton_a4_1_clicked();

    void on_pushButton_a4_2_clicked();

    void on_pushButton_a6_clicked();

    void on_pushButton_a7_clicked();

    void on_pushButton_a8_clicked();

    void on_pushButton_a9_clicked();

    void on_pushButton_conf_clicked();

    void on_pushButton_image_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_clicked();

    void heartBeatDead();

private:
    Ui::MainWindow *ui;
    VideoPlayer *videoPlayer;
    AudioPlayerTrain *audioPlayerTrain;
    AudioPlayer *audioPlayer;
    int cur_dev_id;
    bool remote_started;
    QTimer testTimer;
    UdpHeartBeatPtr udpHeartBeatSocket;
    UdpRegisterPtr udpRegisterSocket;
    SwitchCapsPtr switchCapsSocket;
    OutSocketPtr outsocket;
    SendMsg sendMsg;
};

#endif // MAINWINDOW_H
