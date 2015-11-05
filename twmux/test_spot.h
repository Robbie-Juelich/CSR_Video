#ifndef _SPOT_H_
#define _SPOT_H_

#define SPOTQUEMAXSIZE	16

#define SEL_SBUF 	0x10
#define SEL_RBUF 	0
#define SEL_PBBUF	0x80

#define SPOTMODE_QUAD		0
#define SPOTMODE_16CIF	1	
#define SPOTMODE_D1			2
#define SPOTMODE_HALFD1 3
#define SPOTMODE_9CH   	4	
#define SPOTMODE_HD			5
#define SPOTMODE_HD_PB5	6
#define SPOTMODE_HDQUAD 7
#define SPOTMODE_HDMIX  8

typedef struct SportInfo {
	u8 en;
	u16 w,h;
	u8 rport; // use rport to spot DAC output
	u8 rotate_en;
	u32 rotate_timer;
	u32 rotate_cnt;
	u8 step;
	u8 curbinx;
	u8 bsize; // Number of Buffer using
	u8 buf[SPOTQUEMAXSIZE];
} _SportInfo;

//--

extern const _RecSpotBuf rTbBuf_All_CIF_Frame[];
extern const _RecSpotBuf sTbBuf_QUAD_Frame[];
extern const _RecSpotBuf pTbBuf_13_D1_Frame[];
extern const _RecSpotBuf rTbBuf_Mix_CIF_Frame[];
extern const _RecSpotBuf rTbBuf_9_12_D1_Frame[];
extern const _RecSpotBuf rTbBuf_1_8_D1_Frame[];
extern const u8 NORMAL_REC_PORT_TB[];
extern const u8 NORMAL_REC_SPOTBUF_PORT_TB[];

void SpotInit(u8 id, u8 e1, u8 e2);
int SpotTest(int argc, char **argv);

void printspotinfo(u8 p);

void InitSpot_4_HD_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel);
void InitSpot_Quad_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel);
void InitSpot_HDMix_Quad_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel);
void InitSpot_4_HD_Rotate_by_PB5(u8 no, u8 p, u8 rp, u8 buf_sel);
void InitSpot_HD_Quad(u8 no, u8 p, u8 rp, u8 buf_sel);
void InitSpot_16_D1_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel);

void CheckSpotPortRotate(u8 id);

void InitSpotAll_16CIF_OneStop(u8 no, u8 p, u8 rp, u8 buf_sel);

#endif
