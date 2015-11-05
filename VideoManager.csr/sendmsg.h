#ifndef SENDMSG_H
#define SENDMSG_H
#include "msg.h"
#include"udpregister.h"

class SendMsg
{
public:
    SendMsg();
    void send(msg_t &msg);

    msg_t wrapCamMsg(int devid, int mode, int camid = 0);
    msg_t wrapVideoStartMsg(int devid);
    msg_t wrapVideoStopMsg(int devid);
    msg_t wrapAudioStartMsg(int devid);
    msg_t wrapAudioStopMsg(int devid);
    msg_t wrapUpdateMsg(int devid, int type);

    msg_t wrapMVMsgTest(bool show);
    msg_t wrapLocMsgTest(bool show);

private:
    msg_t msg;
    int m_devid;
    inline void setMsgHeader(msg_t& msg, char type){
		//char *data = (char*)(msg.data);
		msg.data[0] = 0x5;
        msg.data[1] = type;
    }
    inline void setMsgEnd(msg_t & msg){
        int len = msg_length(msg);
        msg.data[len -2] = 0x0;   // sum
        msg.data[len -1] = 0x17; // end
        for(int i = 1; i < len - 2; ++i) {
            msg.data[len -2] += msg.data[i];
        }
    }
};

#endif // SENDMSG_H
