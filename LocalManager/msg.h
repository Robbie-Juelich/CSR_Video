#ifndef MSG_H
#define MSG_H

#define VIDEO_MODE_MSG    0xA4
#define VIDEO_START_MSG      0xA6
#define VIDEO_STOP_MSG       0xA7
#define AUDIO_START_MSG     0xA8
#define AUDIO_STOP_MSG      0xA9
#define SYSTEM_UPDATE_MSG 0xAB


#ifdef WIN32
//#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

typedef struct {
    char fstart;
    char ftype;
}msg_header_t;

typedef struct {
    msg_header_t header;
    char dev_id;
    char sum;
    char fend;
}id_msg_t;

typedef struct {
    msg_header_t header;
    char camid;
    char sum;
    char fend;
}camid_msg_t;

typedef struct {
    msg_header_t header;
    char dev_id;
    char sum;
    char fend;
}start_msg_t;

typedef struct {
    msg_header_t header;
    char dev_id;
    char sum;
    char fend;
}stop_msg_t;

typedef struct {
    msg_header_t header;
    char type;
    char sum;
    char fend;
}mx6ip_msg_t;

typedef struct{
    msg_header_t header;
    char m_ths;
    char m_huns;
    char m_tens;
    char m_ones;
    char m_dot;
    char m_dst;
    char m_dnd;
    char m_drd;
    char m_K;
    char m_M;
    char space;
    char v_huns;
    char v_tens;
    char v_ones;
    char v_K;
    char v_M;
    char v_slash;
    char v_H;
    char sum;
    char fend;
}mv_msg_t;

typedef struct{
    msg_header_t header;
    char la_dhuns;
    char la_dtens;
    char la_dones;
    char la_d;
    char la_mtens;
    char la_mones;
    char la_m;
    char la_stens;
    char la_sones;
    char la_s;
    char la;
    char space;
    char lo_dhuns;
    char lo_dtens;
    char lo_dones;
    char lo_d;
    char lo_mtens;
    char lo_mones;
    char lo_m;
    char lo_stens;
    char lo_sones;
    char lo_s;
    char lo;
    char sum;
    char fend;
}loc_msg_t;

/*
typedef struct{
    msg_header_t header;
    char dev_id;
    union {
        char cam1;
        char cam4[4];
        char cam6[6];
        char cam8[8];
    }cam;
    char sum;
    char fend;
}cam_msg_t;
*/

typedef struct{
    msg_header_t header;
    char dev_id;
    char cam[8];
    char sum;
    char fend;
}cam_msg_t;

typedef struct{
    msg_header_t header;
    char ip_b1;
    char ip_b2;
    char ip_b3;
    char ip_b4;
    char port_bh;
    char port_bl;
    unsigned char sum;
    char fend;
}ip_msg_t;

typedef struct {
                msg_header_t header; //type:0xC0
                char dev_id;
                int vcaps_len;
                char vcaps[400];
                int acaps_len;
                char acaps[400];
                char fend;
} avcaps_msg_t;// __attribute__((packed)) avcaps_msg_t;

#define MD5_LEN 16
#define LOAD_SIZE  1024 * 4 // 4K

typedef struct {
        msg_header_t header;
        char dev_id;
//        char md5[MD5_LEN];
//        char len[4];
        char fname[64];
        char sum;
        char fend;
}update_msg_t;


typedef struct {
        msg_header_t header;
        char dev_id;
        char dev_id2;
        char dev_id3;
        char dev_id4;
        char dev_type;
        char dev_name[9];
        char ju;
        char duan;
        char video_id[5];
        char sum;
        char fend;
}reg_msg_t;


typedef union {
    id_msg_t id_msg;
    start_msg_t start_msg;
    stop_msg_t stop_msg;
    mv_msg_t mv_msg;
    loc_msg_t loc_msg;
    cam_msg_t cam_msg;
    ip_msg_t ip_msg;
    update_msg_t update_msg;
    mx6ip_msg_t mx6ip_msg;
    reg_msg_t reg_msg;
    unsigned char data[80];
}msg_t;


#ifdef WIN32
//#pragma pack(pop)
#else
#pragma pack(0)
#endif

inline int msg_length(const msg_t &msg){
    int len = 0x0;
    switch(msg.data[1]){ // the head
    case 0xA0:
       len = sizeof(msg.id_msg);
       break;
    case 0xA1:
        len = sizeof(msg.mv_msg);
        break;
    case 0xA2:
        len = sizeof(msg.loc_msg);
        break;
    case 0xA3:
        len = sizeof(msg.cam_msg);// problem
         break;
    case 0xA4:
        len = sizeof(msg.cam_msg);
        break;
    case 0xA5:
        len = sizeof(msg.ip_msg);
        break;
    case 0xA6:
	case 0xA8:
        len = sizeof(msg.start_msg);
        break;
    case 0xA7:
	case 0xA9:
        len = sizeof(msg.stop_msg);
        break;
	case 0xAA:
        len = sizeof(msg.update_msg);
		break;
    case 0xAB:
        len = sizeof(msg.reg_msg);
        break;
    default:
        len = sizeof(msg.data);
        break;
    }
    return len;
}



#endif
