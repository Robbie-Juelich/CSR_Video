#include "sendmsg.h"
#include"QsLog.h"
//#include<cstdlib>

SendMsg::SendMsg()
{

}

msg_t SendMsg::wrapCamMsg(int devid, int mode, int camid)
{
    msg_t msg;
    setMsgHeader(msg, VIDEO_MODE_MSG);
    if(mode == 0x0){
        msg.cam_msg.dev_id = devid;
        msg.cam_msg.cam[0] = camid;
        msg.cam_msg.cam[1] = 0xff;
        msg.cam_msg.cam[2] = 0xff;
        msg.cam_msg.cam[3] = 0xff;
        msg.cam_msg.cam[4] = 0xff;
	    msg.cam_msg.cam[5] = 0xff;
        msg.cam_msg.cam[6]  = 0xff;
        msg.cam_msg.cam[7]  = 0xff;
    }
    else if(mode == 0x1){
        msg.cam_msg.dev_id = devid;
        msg.cam_msg.cam[0] = 0x0;
        msg.cam_msg.cam[1] = 0x1;
        msg.cam_msg.cam[2] = 0x2;
        msg.cam_msg.cam[3] = 0x3;
        msg.cam_msg.cam[4] = 0x4;
        msg.cam_msg.cam[5] = 0x5;
        msg.cam_msg.cam[6] = 0x6;
        msg.cam_msg.cam[7] = 0x7;
    }
    else{

    }
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapAudioStartMsg(int devid)
{
    msg_t msg;
    setMsgHeader(msg, AUDIO_START_MSG);
	msg.start_msg.dev_id = devid;
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapAudioStopMsg(int devid)
{
    msg_t msg;
    setMsgHeader(msg, AUDIO_STOP_MSG);
    msg.stop_msg.dev_id = devid;
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapVideoStartMsg(int devid)
{
    msg_t msg;
    setMsgHeader(msg, VIDEO_START_MSG);
	msg.start_msg.dev_id = devid;
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapVideoStopMsg(int devid)
{
    msg_t msg;
    setMsgHeader(msg, VIDEO_STOP_MSG);
    msg.stop_msg.dev_id = devid;
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapUpdateMsg(int devid, int type)
{
    msg_t msg;
    setMsgHeader(msg, SYSTEM_UPDATE_MSG);
    msg.stop_msg.dev_id = devid;

    if(type == 0){
        msg.update_msg.fname[0] = 'g';
        msg.update_msg.fname[1] = 's';
        msg.update_msg.fname[2] = 't';
        msg.update_msg.fname[3] = '_';
        msg.update_msg.fname[4] = 's';
        msg.update_msg.fname[5] = 'v';
        msg.update_msg.fname[6] = 'r';
        msg.update_msg.fname[7] = '\0';
    } else {
        msg.update_msg.fname[0] = 'i';
        msg.update_msg.fname[1] = 'p';
        msg.update_msg.fname[2] = 'c';
        msg.update_msg.fname[3] = 'a';
        msg.update_msg.fname[4] = 'm';
        msg.update_msg.fname[5] = '.';
        msg.update_msg.fname[6] = 'c';
        msg.update_msg.fname[7] = 'o';
        msg.update_msg.fname[8] = 'n';
        msg.update_msg.fname[9] = 'f';
        msg.update_msg.fname[10] = '\0';
    }
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapLocMsgTest(bool show)
{
    msg_t msg;
    setMsgHeader(msg, 0xa2); // need to change
    memset(&(msg.data[2]), '6',  msg_length(msg) - 4);
    if(show) {
        msg.loc_msg.la_d = 0xb0;
        msg.loc_msg.la_m = '\'';
        msg.loc_msg.la_s = '\"';
        msg.loc_msg.la = 'E';

        msg.loc_msg.lo_d= 0xb0;
        msg.loc_msg.lo_m = '\'';
        msg.loc_msg.lo_s = '\"';
        msg.loc_msg.lo = 'N';

        msg.loc_msg.space = ' ';
    }
    else {
        memset(&(msg.data[2]), 0xff,  msg_length(msg) - 4);
    }
    setMsgEnd(msg);
    return msg;
}

msg_t SendMsg::wrapMVMsgTest(bool show)
{
    msg_t msg;
    setMsgHeader(msg, 0xa1); // need to change
    memset(&(msg.data[2]), '8',  msg_length(msg) - 4);
    if(show) {
        msg.mv_msg.m_dot = '.';
        msg.mv_msg.m_K = 'K';
        msg.mv_msg.m_M = 'm';
        msg.mv_msg.v_K = 'K';
        msg.mv_msg.v_M = 'm';
        msg.mv_msg.v_slash = '/';
        msg.mv_msg.v_H = 'h';
        msg.mv_msg.space = ' ';
    }
    else {
        memset(&(msg.data[2]), 0xff,  msg_length(msg) - 4);
    }
    setMsgEnd(msg);
    return msg;
}

void SendMsg::send(msg_t &msg)
{
    //UdpRegister*
    UdpRegisterPtr registerSocket = UdpRegister::Instance();
    QByteArray array((char*)(msg.data), msg_length(msg));
    QLOG_DEBUG() << "send msg " << (int)msg.data[1];
#if 0
    for(int i = 0; i < msg_length(msg);++i){
        QLOG_DEBUG() << (unsigned char)(msg.data[i]);
    }
#endif
    registerSocket->writeData(array);
}

