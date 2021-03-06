#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msg.h"
#include<QMessageBox>
#include<QtEndian>
#include<QsLog.h>
#include "sendmsg.h"
#include"outsocket.h"
#ifdef WIN32
#include<windows.h>
#else
#include<sys/stat.h>
#endif
#include "configdialog.h"

using namespace QsLogging;

//#define NO_SHANG_WEI_JI

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), videoWidth(0), videoHeight(0)
{
    ui->setupUi(this);

	QLOG_TRACE() << "to init video player";

    videoPlayer = ui->video_player;
    videoPlayer->initPlayer();

	QLOG_TRACE() << "to init audio player";

    audioPlayer = new AudioPlayer();
    audioPlayer->initPlayer();

    audioPlayerTrain = new AudioPlayerTrain(this);
    audioPlayerTrain->initPlayer();

	QLOG_TRACE() << "audio & videp player initialized";

    connect(videoPlayer, SIGNAL(stateChanged()), this, SLOT(onStateChanged()));
    connect(&testTimer, SIGNAL(timeout()), this, SLOT(on_test()));  // 用于测试

    remote_started = false;

    cur_dev_id = ui->comboBox->currentIndex();

    bool use_default = false;
    if(use_default) {
        QString ip("127.0.0.1");
        videoPlayer->setIP(QHostAddress(ip), 5008);

        audioPlayerTrain->setIP(QHostAddress(ip), 5009);

        audioPlayer->setIP(QHostAddress(ip), 5002, 0);
        audioPlayer->setIP(QHostAddress(ip), 5003, 1);
    }

    connectTimer=new QTimer(this);
    connectTimer->setInterval(1000);
    connect(connectTimer, SIGNAL(timeout()),
                 this, SLOT(onConnectTimeout()) );

    createSettings();
    loadSettings();

    udpHeartBeatSocket = UdpHeartBeat::Instance(heartbeat_timeout,yellow_timeout);
    udpRegisterSocket = UdpRegister::Instance();
    switchCapsSocket = SwitchCaps::Instance();

    connect(udpRegisterSocket.data(), SIGNAL(regMsgArrived()),
            this, SLOT(responseRegMsg()));
    connect(udpHeartBeatSocket.data(), SIGNAL(heartBeatMsgArrived()),
            this, SLOT(responseVideoHeartBeatMsg()));
    connect(switchCapsSocket.data(), SIGNAL(capsArrived(int, QString, unsigned int, unsigned int)),
            this, SLOT(responseCapsMsg(int, QString, unsigned int, unsigned int)));
      connect(switchCapsSocket.data(), SIGNAL(capsMsgarrive()),this, SLOT(rCapsMsg()));//new add1
    connect(udpHeartBeatSocket.data(), SIGNAL(dead()), this, SLOT(heartBeatDead()));

    connect(udpHeartBeatSocket.data(), SIGNAL(heartBeatMsgArrived()),
            this, SLOT(green() ) );//new add
    outsocket = OutSocket::Instance();

    videoPlayer->setSocket(outsocket->getVSocket());
    audioPlayer->setSocket(outsocket->getASocket());
    audioPlayerTrain->setSocket(outsocket->getVSocket());

    ui->label_signal->setPixmap(QPixmap(":/images/gray.png") );

    connect(udpHeartBeatSocket.data(), SIGNAL(sYellow() ), this, SLOT(yellowwarn() ) );

    ui->groupBox_system->setEnabled(false);
    ui->groupBox_video->setEnabled(false);
    ui->groupBox_test->setEnabled(false);
    ui->groupBox_host->setEnabled(false);

    ui->label_signal->update();//new add
#ifdef NO_SHANG_WEI_JI
    udpRegisterSocket->no_shangweiji_test();
#endif
}

MainWindow::~MainWindow()
{
    msg_t msg1, msg2, msg3, msg4;

    msg1 = sendMsg.wrapAudioStopMsg(0);
    msg2 = sendMsg.wrapVideoStopMsg(0);
    msg3 = sendMsg.wrapAudioStopMsg(1);
    msg4 = sendMsg.wrapVideoStopMsg(1);

    sendMsg.send(msg1);
    sendMsg.send(msg2);
    sendMsg.send(msg3);
    sendMsg.send(msg4);

    sendMsg.send(msg1);
    sendMsg.send(msg2);
    sendMsg.send(msg3);
    sendMsg.send(msg4);

    udpHeartBeatSocket->stopChecking();

    delete audioPlayer;
    delete videoPlayer; // should be delected by ui
    delete ui;
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void MainWindow::setVideoPlayerSize(unsigned int w, unsigned int h)
{
    videoWidth = w;
    videoHeight = h;
}
#endif

void MainWindow::heartBeatDead()
{
     ui->label_signal->setPixmap(QPixmap(":/images/red.png") );//new add

    QMessageBox::warning(this, QString::fromLocal8Bit("连接断开"),
        QString::fromLocal8Bit("连接断开, 请重新连接"), QMessageBox::Ok);
    videoPlayer->unsetIP();
    audioPlayer->unsetIP();
    audioPlayerTrain->unsetIP();
    ui->groupBox_video->setEnabled(false);
    ui->groupBox_system->setEnabled(false);
}

void MainWindow::responseVideoHeartBeatMsg()
{
    QHostAddress addr;
    quint16 port;

    addr = udpHeartBeatSocket->getvideoInRtcpIp();
    port = udpHeartBeatSocket->getvideoInRtcpPort();
    if (!addr.isNull()) {
       videoPlayer->setIP(addr, port);
    }

    addr = udpHeartBeatSocket->getaudioInRtcpIp();
    port = udpHeartBeatSocket->getaudioInRtcpPort();
    if (!addr.isNull()) {
       audioPlayerTrain->setIP(addr, port);
    }

    addr = udpHeartBeatSocket->getaudioOutRtpIp();
    port = udpHeartBeatSocket->getaudioOutRtpPort();
    if (!addr.isNull()) {
       audioPlayer->setIP(addr, port, 0);
    }

    addr = udpHeartBeatSocket->getaudioOutRtcpIp();
    port = udpHeartBeatSocket->getaudioOutRtcpPort();
    if (!addr.isNull()) {
       audioPlayer->setIP(addr, port, 1);
    }

    if(audioPlayer->ipPrepared() && videoPlayer->ipPrepared() &&
            audioPlayerTrain->ipPrepared() &&
            remote_started) {
        ui->groupBox_video->setEnabled(true);
        ui->groupBox_system->setEnabled(true);
    }
}

void MainWindow::responseRegMsg()
{
    ui->groupBox_host->setEnabled(true);
}

void MainWindow::responseCapsMsg(int type, QString caps,
                                 unsigned int w, unsigned int h)
{
//    QString a_caps = switchCapsSocket->getACaps();
//    QString v_caps = switchCapsSocket->getVCaps();
    if (caps.length() <= 0) {
        return;
    }
    if (type == 0) {
        videoPlayer->updateCaps(caps);
        if (videoWidth >= w && videoHeight >= h) {
            w = videoWidth;
            h = videoHeight;
        }
        videoPlayer->setFixedSize(w, h);
        QLOG_DEBUG() << "in responseCapsMsg VCaps: " << caps;
    } else {
        audioPlayerTrain->updateCaps(caps);
        QLOG_DEBUG() << "in responseCapsMsg ACaps: " << caps;
    }
}

void MainWindow::onStateChanged()
{
    //QGst::State state = videoPlayer->state();
   // ui->pushButton_6->setEnabled(state != QGst::StateNull);// 关闭视频传输按钮
   // ui->pushButton_7->setEnabled(state != QGst::StatePlaying);// 开启视频传输按钮
   // ui->pushButton_8->setEnabled(state == QGst::StatePlaying);
   // ui->pushButton_11->setEnabled(state == QGst::StatePlaying);

    //ui->pushButton_9->setEnabled(state != QGst::StateNull);// 关闭音频传输按钮
    //ui->pushButton_10->setEnabled(state != QGst::StatePlaying);// 开启视频传输按钮
}

void MainWindow::do_send_start_msg()
{
    videoPlayer->toChangeMode(0x0);

    msg_t msg;
    int i = 0;
    for (i = 0; i < 2; ++i) {
        msg = sendMsg.wrapCamMsg(cur_dev_id, 0, 0x0);
        sendMsg.send(msg);

        msg = sendMsg.wrapVideoStartMsg(cur_dev_id);
        sendMsg.send(msg);

        msg = sendMsg.wrapAudioStartMsg(cur_dev_id);
        sendMsg.send(msg);
    }
    remote_started = true;
}

/*
 * 初始化 发送开始命令 主机接收后发送UDP数据包
 *  此后将获取到IP地址和端口号
*/
void MainWindow::on_pushButton_link_remote_clicked()
{
    QLOG_DEBUG() << "Try connect to ipcam...";
    if(capsMsgarrive){
            capsMsgarrive=false;//new add1
    }
    do_send_start_msg();

    start_time = QDateTime::currentDateTime();
    if(!connectTimer->isActive()){
       connectTimer->start();
    }
    if(videoPlayer->state() != QGst::StatePlaying) {
        videoPlayer->setVpipelinePlay();
    }
    videoPlayer->setVpipelinePlay();
   // videoPlayer->update();
}

void MainWindow::on_pushButton_all_off_clicked()
{

    udpHeartBeatSocket->stopChecking();
    udpHeartBeatSocket->stopYellowsignalcheck();
    if(connectTimer->isActive()){
         connectTimer->stop();
    }

    msg_t msg1, msg2, msg3, msg4;

    msg1 = sendMsg.wrapAudioStopMsg(0);
    msg2 = sendMsg.wrapVideoStopMsg(0);
    msg3 = sendMsg.wrapAudioStopMsg(1);
    msg4 = sendMsg.wrapVideoStopMsg(1);

    sendMsg.send(msg1);
    sendMsg.send(msg2);
    sendMsg.send(msg3);
    sendMsg.send(msg4);

#ifdef WIN32
    //::Sleep(1000);
#else
    ::usleep(500000);
    //    ::sleep(1);
#endif

    QMessageBox::information(this, QString::fromLocal8Bit( "关闭连接"),
                             QString::fromLocal8Bit("关闭连接中...！"));

#ifdef WIN32
    //::Sleep(1000);
#else
    ::usleep(500000);
    //    ::sleep(1);
#endif
    udpHeartBeatSocket->stopChecking();
    udpHeartBeatSocket->stopYellowsignalcheck();
    if(connectTimer->isActive()){
         connectTimer->stop();
    }
    sendMsg.send(msg1);
    sendMsg.send(msg2);
    sendMsg.send(msg3);
    sendMsg.send(msg4);

    //videoPlayer->stop();  // unsetIP will first stop the pipeline
    videoPlayer->unsetIP();
    //audioPlayer->stop();
    audioPlayer->unsetIP();

    videoPlayer->repaint();

    ui->groupBox_system->setEnabled(false);
    ui->groupBox_test->setEnabled(false);
    ui->groupBox_video->setEnabled(false);

}

/* 开启视频传输 */
void MainWindow::on_pushButton_video_on_clicked()
{
    if(videoPlayer->state() == QGst::StatePlaying) {
        return;
    }
    videoPlayer->toChangeMode(0x0);

    QLOG_TRACE() << "cur mode " << videoPlayer->getCurMode() ;

    msg_t msg;
    msg = sendMsg.wrapCamMsg(cur_dev_id, 0x0, 0x0);
    sendMsg.send(msg);

    msg = sendMsg.wrapVideoStartMsg(cur_dev_id);
    sendMsg.send(msg);

	if(!videoPlayer->ipPrepared()){
        QMessageBox::warning(this,QString::fromLocal8Bit("错误"),
			QString::fromLocal8Bit("暂未获取到视频服务器IP地址"));
        return;
    }
    videoPlayer->play();
}

/* 停止视频传输 */
void MainWindow::on_pushButton_video_off_clicked()
{
    msg_t msg;
    msg = sendMsg.wrapVideoStopMsg(cur_dev_id);
    sendMsg.send(msg);
#ifdef WIN32
#else
    ::sleep(1);
#endif
    sendMsg.send(msg);

    videoPlayer->stop();
    audioPlayer->pause();

    ui->groupBox_video->setEnabled(false);
    remote_started = false;
}

/* 单通道选择 */
void MainWindow::on_pushButton_one_ch_clicked()
{
    if(videoPlayer->state() != QGst::StatePlaying) {
        QMessageBox::warning(this,QString::fromLocal8Bit("错误"),
            QString::fromLocal8Bit("未连接视频！请点击“开启”视频连接"));
        return;
    }

    videoPlayer->toChangeMode(0x0);
    msg_t msg;
    msg = sendMsg.wrapCamMsg(cur_dev_id, 0x0, ui->comboBox_channel->currentIndex());
    sendMsg.send(msg);
}

/* BT1120合成画面 */
void MainWindow::on_pushButton_multi_ch_clicked()
{
    if(videoPlayer->state() != QGst::StatePlaying) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"),
            QString::fromLocal8Bit("未连接视频！请点击“开启”视频连接"));
        return;
    }
    if(videoPlayer->getCurMode() == 1) {
        return;
    }

    videoPlayer->toChangeMode(0x1);
    msg_t msg;
    msg = sendMsg.wrapCamMsg(cur_dev_id, 1, 0);
    sendMsg.send(msg);
}

/* 开启音频输出 */
void MainWindow::on_pushButton_audio_on_clicked()
{
    msg_t msg;
    msg = sendMsg.wrapAudioStartMsg(cur_dev_id);
    sendMsg.send(msg);

    if(!audioPlayer->ipPrepared()){
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("暂未获取到视频服务器IP地址"));
        return;
    }
//    ::usleep(500000);
    audioPlayer->play();
}

/* 关闭音频输出 */
void MainWindow::on_pushButton_audio_off_clicked()
{
    //msg_t msg;
    //msg = sendMsg.wrapAudioStopMsg(cur_dev_id);
    //sendMsg.send(msg);
    audioPlayer->pause();
}

void MainWindow::onConnectTimeout()
{

    if (capsMsgarrive) {
        QLOG_DEBUG() << "Connect ok !";
        connectTimer->stop();
        return;
    }

    if(!capsMsgarrive){

        if (start_time.secsTo(QDateTime::currentDateTime()) >= connect_timeout) {
            QMessageBox::warning(this, "连接超时", "连接超时");
            connectTimer->stop();
        } else {
            QLOG_DEBUG() << "Try connect to ipcam again...";
            do_send_start_msg();
        }
    }

//    if (videoPlayer->ipPrepared()) {
//        QLOG_DEBUG() << "Connect ok !";
//        connectTimer.stop();
//        return;
//    }
//    if (!videoPlayer->ipPrepared() && !audioPlayerTrain->ipPrepared()) {
//        if (start_time.secsTo(QDateTime::currentDateTime()) >= connect_timeout) {
//            QMessageBox::warning(this, "连接超时", "连接超时");
//            connectTimer.stop();
//        } else {
//            QLOG_DEBUG() << "Try connect to ipcam again...";
//            do_send_start_msg();
//        }
//    }
}

void MainWindow::on_test()
{
    if(videoPlayer->getCurMode() == 0x1) {
        on_pushButton_one_ch_clicked();
    }
    else {
        on_pushButton_multi_ch_clicked();
    }
}

/* 开启测试 */
void MainWindow::on_pushButton_test_on_clicked()
{
    ui->groupBox_video->setEnabled(false);
    on_test();
    testTimer.start(15000); // 10 sec
}

/* 关闭测试 */
void MainWindow::on_pushButton_test_off_clicked()
{
    testTimer.stop();
    ui->groupBox_video->setEnabled(true);
}

void MainWindow::on_pushButton_a4_1_clicked()
{
    msg_t msg = sendMsg.wrapCamMsg(cur_dev_id, 0);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_a4_2_clicked()
{
    msg_t msg = sendMsg.wrapCamMsg(cur_dev_id, 1);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_a6_clicked()
{
    msg_t msg = sendMsg.wrapVideoStartMsg(cur_dev_id);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_a7_clicked()
{
    msg_t msg = sendMsg.wrapVideoStopMsg(cur_dev_id);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_a8_clicked()
{
    msg_t msg;
	msg = sendMsg.wrapAudioStartMsg(cur_dev_id);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_a9_clicked()
{
    msg_t msg = sendMsg.wrapAudioStopMsg(cur_dev_id);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_conf_clicked()
{
    msg_t msg = sendMsg.wrapUpdateMsg(cur_dev_id, 1);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_image_clicked()
{
    msg_t msg = sendMsg.wrapUpdateMsg(cur_dev_id, 0);
    sendMsg.send(msg);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    udpHeartBeatSocket->stopChecking();

    msg_t msg1, msg2, msg3, msg4;

    msg1 = sendMsg.wrapAudioStopMsg(0);
    msg2 = sendMsg.wrapVideoStopMsg(0);
    msg3 = sendMsg.wrapAudioStopMsg(1);
    msg4 = sendMsg.wrapVideoStopMsg(1);

    sendMsg.send(msg1);
    sendMsg.send(msg2);
    sendMsg.send(msg3);
    sendMsg.send(msg4);

#ifdef WIN32
    //::Sleep(1000);
#else
    ::usleep(500000);
    //    ::sleep(1);
#endif

    udpHeartBeatSocket->stopChecking();

    QMessageBox::information(this, QString::fromLocal8Bit( "切换设备"),
                             QString::fromLocal8Bit("切换设备中，请重新连接设备！"));

#ifdef WIN32
    //::Sleep(1000);
#else
    ::usleep(500000);
    //    ::sleep(1);
#endif

    QLOG_DEBUG() << "device changed from [" << cur_dev_id << "] to [" << index << "]";
    cur_dev_id = index;

    ui->groupBox_system->setEnabled(false);
    ui->groupBox_test->setEnabled(false);
    ui->groupBox_video->setEnabled(false);

    videoPlayer->repaint();
    videoPlayer->stop();  // unsetIP will first stop the pipeline
    audioPlayer->stop();
    audioPlayer->unsetIP();
    videoPlayer->unsetIP();

}


void MainWindow::on_pushButton_update_image_clicked()
{
	QMessageBox::StandardButton rb = QMessageBox::warning(this, QString::fromLocal8Bit("警告"), 
		QString::fromLocal8Bit("更新系统时请确保列车视频系统供电正常，确定更新系统？"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No); 
	if(rb == QMessageBox::No){
		QMessageBox::information(this, QString::fromLocal8Bit("更新系统"), 
			QString::fromLocal8Bit("取消更新系统"), QMessageBox::Ok);
		return;
	}
	msg_t msg;
    msg = sendMsg.wrapUpdateMsg(cur_dev_id, 0);
    sendMsg.send(msg);
#ifdef WIN32
#else
    ::sleep(1);
#endif
	QMessageBox::information(this, QString::fromLocal8Bit("更新系统"), 
        QString::fromLocal8Bit("更新系统中，大约需要半分钟，请稍后重启程序"), QMessageBox::Ok);

    videoPlayer->stop();
    videoPlayer->unsetIP();
    audioPlayer->pause();
    videoPlayer->unsetIP();
    ui->groupBox_system->setEnabled(false);
    ui->groupBox_video->setEnabled(false);
}

void MainWindow::on_pushButton_update_config_clicked()
{
	QMessageBox::StandardButton rb = QMessageBox::warning(this, QString::fromLocal8Bit("警告"), 
		QString::fromLocal8Bit("更新系统时请确保列车视频系统供电正常，确定更新配置？"),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No); 
	if(rb == QMessageBox::No){
		QMessageBox::information(this, QString::fromLocal8Bit("更新配置"), 
			QString::fromLocal8Bit("取消更新配置"), QMessageBox::Ok);
		return;
	}

    msg_t msg;
    msg = sendMsg.wrapUpdateMsg(cur_dev_id, 1);
    sendMsg.send(msg);
#ifdef WIN32
#else
    ::sleep(1);
#endif
	QMessageBox::information(this, QString::fromLocal8Bit("更新配置"), 
		QString::fromLocal8Bit("更新配置中，请稍后重启程序"), QMessageBox::Ok);

    videoPlayer->stop();
    videoPlayer->unsetIP();
    audioPlayer->pause();
    videoPlayer->unsetIP();
    ui->groupBox_system->setEnabled(false);
    ui->groupBox_video->setEnabled(false);
}

void MainWindow::on_pushButton_clicked()
{
    msg_t msg;
    msg = sendMsg.wrapLocMsgTest(true);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_2_clicked()
{
    msg_t msg;
    msg = sendMsg.wrapMVMsgTest(true);
    sendMsg.send(msg);
}

void MainWindow::on_pushButton_local_config_clicked()
{
    ConfigDialog diag(connect_timeout, heartbeat_timeout, yellow_timeout,this);


    if (diag.exec() == QDialog::Accepted) {
        connect_timeout = diag.get_conn_timeout();
        heartbeat_timeout = diag.get_heart_timeout();
     //   yellow_timeout=diag.get_yellow_timeout();
        yellow_timeout=diag.get_yellow_timeout();
        udpHeartBeatSocket->setTimeOut(heartbeat_timeout,yellow_timeout);
        saveSettings();
    }
}

void MainWindow::saveSettings()
{
    settings->beginGroup("Configuration");
    settings->setValue("connect-timeout",  connect_timeout);
    settings->setValue("heartbeat-timeout",  heartbeat_timeout);
    //settings->setValue("yellow-warning-timeout",yellow_timeout);
    settings->setValue("net-warning-timeout",yellow_timeout);
    settings->endGroup();
}

void MainWindow::loadSettings()
{
    settings->beginGroup("Configuration");
    connect_timeout = settings->value("connect-timeout", default_conn_timeout).toInt();
    heartbeat_timeout = settings->value("heartbeat-timeout", default_heart_timeout).toInt();
    //yellow_timeout=settings->value("yellow-warning-timeout",default_yellow_timeout).toInt();
    yellow_timeout=settings->value("net-warning-timeout",default_yellow_timeout).toInt();
    settings->endGroup();
}

void MainWindow::createSettings()
{
    QString file =  "video.ini";
    settings = new QSettings(file, QSettings::IniFormat, this);
    return;
}


void MainWindow::green()
{
    ui->label_signal->setPixmap(QPixmap(":/images/green.png") );
    //udpHeartBeatSocket->checkTimer->stop();
}


void MainWindow::yellowwarn()
{
     ui->label_signal->setPixmap(QPixmap(":/images/yellow.png") );
     //ui->label_signal->update();
}


void MainWindow::rCapsMsg()//new add1
{
    capsMsgarrive=true;
}
