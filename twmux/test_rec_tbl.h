#ifndef _REC_TBL_H_
#define _REC_TBL_H_

//note : CIF should be frame!

#include "tw2828_twmux.h"


const _RecSpotBuf pTbBuf_All_D1_Frame[] = {
	{R_1,	 ON,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,		  V_0, 0, 0},
	{R_2,	 ON,		CH2,	 BUF_D1,	BUF_FRAME,	H_720,		V_0, 0, 0},
	{R_3,	 ON,		CH3,	 BUF_D1,	BUF_FRAME,	H_1440,		V_0, 0, 0},
	{R_4,	 ON,		CH4,	 BUF_D1,	BUF_FRAME,	H_2160,		V_0, 0, 0},
	{R_5,	 ON,		CH5,	 BUF_D1,	BUF_FRAME,	H_0,		  V_576, 0, 0},
	{R_6,	 ON,		CH6,	 BUF_D1,	BUF_FRAME,	H_720,		V_576, 0, 0},
	{R_7,	 ON,		CH7,	 BUF_D1,	BUF_FRAME,	H_1440,		V_576, 0, 0},
	{R_8,	 ON,		CH8,	 BUF_D1,	BUF_FRAME,	H_2160,		V_576, 0, 0},
	{R_PB1, OFF,	CH9,	 BUF_D1,	BUF_FRAME,	H_0,		  V_0, 0, 0},
	{R_PB2, OFF,	CH10,	 BUF_D1,	BUF_FRAME,	H_720,		V_0, 0, 0},
	{R_PB3, OFF,	CH11,	 BUF_D1,	BUF_FRAME,	H_1440, 	V_0, 0, 0},
	{R_PB4, OFF,	CH12,	 BUF_D1,	BUF_FRAME,	H_2160,		V_0, 0, 0},
	{R_PB5, OFF,	CH13,	 BUF_D1,	BUF_FRAME,	H_0,		  V_576, 0, 0},
	{R_14, OFF,		CH14,	 BUF_D1,	BUF_FRAME,	H_720,		V_576, 0, 0},
	{R_15, OFF,		CH15,	 BUF_D1,	BUF_FRAME,	H_1440,		V_576, 0, 0},
	{R_16, OFF,		CH16,	 BUF_D1,	BUF_FRAME,	H_2160,		V_576, 0, 0},
	{S_1,	 ON,		CH9,	 BUF_D1,	BUF_FRAME,	H_0,		  SPT_V_0, 0, 0},
	{S_2,	 ON,		CH10,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_0, 0, 0},
	{S_3,	 ON,		CH11,	 BUF_D1,	BUF_FRAME,	H_1440,		SPT_V_0, 0, 0},
	{S_4,	 ON,		CH12,	 BUF_D1,	BUF_FRAME,	H_2160,		SPT_V_0, 0, 0},
	{S_5,	 ON,		CH13,	 BUF_D1,	BUF_FRAME,	H_0,		  SPT_V_576_480, 0, 0},
	{S_6,	 ON,		CH14,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_576_480, 0, 0},
	{S_7,	 ON,		CH15,	 BUF_D1,	BUF_FRAME,	H_1440,		SPT_V_576_480, 0, 0},
	{S_8,	 ON,		CH16,	 BUF_D1,	BUF_FRAME,	H_2160,		SPT_V_576_480, 0, 0},
	{S_9,  ON,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,		  SPT_V_1152_960, 0, 0}, //SPOT1
	{S_10, OFF,		CH10,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_0, 0, 0},
	{S_11, OFF,		CH11,	 BUF_D1,	BUF_FRAME,	H_1440, 	SPT_V_0, 0, 0},
	{S_12, OFF,		CH12,	 BUF_D1,	BUF_FRAME,	H_2160,		SPT_V_0, 0, 0},
	{S_13, OFF,		CH13,	 BUF_D1,	BUF_FRAME,	H_0,		  SPT_V_576_480, 0, 0},
	{S_14, OFF,		CH14,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_576_480, 0, 0},
	{S_15, OFF,		CH15,	 BUF_D1,	BUF_FRAME,	H_1440,		SPT_V_576_480, 0, 0},
	{S_16, OFF,		CH16,	 BUF_D1,	BUF_FRAME,	H_2160,		SPT_V_576_480, 0, 0},
  {NOBUF,0,0,0,0,0,0}
};

const _RecSpotBuf rTbBuf_1_8_D1_Frame[] = {
	{R_1,	 ON,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,		  V_0, 0, 0},
	{R_2,	 ON,		CH2,	 BUF_D1,	BUF_FRAME,	H_720,		V_0, 0, 0},
	{R_3,	 ON,		CH3,	 BUF_D1,	BUF_FRAME,	H_1440,		V_0, 0, 0},
	{R_4,	 ON,		CH4,	 BUF_D1,	BUF_FRAME,	H_2160,		V_0, 0, 0},
	{R_5,	 ON,		CH5,	 BUF_D1,	BUF_FRAME,	H_0,		  V_576, 0, 0},
	{R_6,	 ON,		CH6,	 BUF_D1,	BUF_FRAME,	H_720,		V_576, 0, 0},
	{R_7,	 ON,		CH7,	 BUF_D1,	BUF_FRAME,	H_1440,		V_576, 0, 0},
	{R_8,	 ON,		CH8,	 BUF_D1,	BUF_FRAME,	H_2160,		V_576, 0, 0},
	{R_PB1,	OFF,		CH9,	 BUF_D1,	BUF_FRAME,	H_0,		  V_0, 0, 0},
	{R_PB2, OFF,		CH10,	 BUF_D1,	BUF_FRAME,	H_720,		V_0, 0, 0},
	{R_PB3, OFF,		CH11,	 BUF_D1,	BUF_FRAME,	H_1440, 	V_0, 0, 0},
	{R_PB4, OFF,		CH12,	 BUF_D1,	BUF_FRAME,	H_2160,		V_0, 0, 0},
	{R_PB5, OFF,		CH13,	 BUF_D1,	BUF_FRAME,	H_0,		  V_576, 0, 0},
	{R_14, OFF,		CH14,	 BUF_D1,	BUF_FRAME,	H_720,		V_576, 0, 0},
	{R_15, OFF,		CH15,	 BUF_D1,	BUF_FRAME,	H_1440,		V_576, 0, 0},
	{R_16, OFF,		CH16,	 BUF_D1,	BUF_FRAME,	H_2160,		V_576, 0, 0},
  {NOBUF,0,0,0,0,0,0}
};

const _RecSpotBuf pTbBuf_13_D1_Frame[] = {
	{R_1,  OFF,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,	  	V_0 , 0, 0},
	{R_2,  OFF,		CH2,	 BUF_D1,	BUF_FRAME,	H_720,		V_0 , 0, 0},
	{R_3,  OFF,		CH3,	 BUF_D1,	BUF_FRAME,	H_1440,		V_0 , 0, 0},
	{R_4,  OFF,		CH4,	 BUF_D1,	BUF_FRAME,	H_2160,		V_0 , 0, 0},
	{R_5,  OFF,		CH5,	 BUF_D1,	BUF_FRAME,	H_0,	  	V_576	 , 0, 0},
	{R_6,  OFF,		CH6,	 BUF_D1,	BUF_FRAME,	H_720,    V_576	 , 0, 0},
	{R_7,  OFF,		CH7,	 BUF_D1,	BUF_FRAME,	H_1440,		V_576  , 0, 0},
	{R_8,  OFF,		CH8,	 BUF_D1,	BUF_FRAME,	H_2160,   V_576  , 0, 0},
	{R_PB1, OFF,		PB1,	 BUF_D1,	BUF_FRAME,	H_0,	  	V_0 , 0, 0},       
	{R_PB2, OFF,		PB2,	 BUF_D1,	BUF_FRAME,	H_720,	  V_0 , 0, 0},       
	{R_PB3, OFF,		PB3,	 BUF_D1,	BUF_FRAME,	H_1440, 	V_0 , 0, 0},       
	{R_PB4, OFF,		PB4,	 BUF_D1,	BUF_FRAME,	H_2160,   V_0 , 0, 0},       
	{R_PB5, ON,	 	PB5,	 BUF_D1,	BUF_FRAME,	H_0,			V_0 , 0, 0},
	{R_14, OFF,		CH14,	 BUF_D1,	BUF_FRAME,	H_360,    V_576	 , 0, 0},
	{R_15, OFF,		CH15,	 BUF_D1,	BUF_FRAME,	H_720,		V_576  , 0, 0},
	{R_16, OFF,		CH16,	 BUF_D1,	BUF_FRAME,	H_1080,   V_576  , 0, 0},
  {NOBUF,0,0,0,0,0,0}
};


const _RecSpotBuf rTbBuf_9_12_D1_Frame[] = {
	{R_1,  OFF,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,	  	V_0 , 0, 0},
	{R_2,  OFF,		CH2,	 BUF_D1,	BUF_FRAME,	H_720,		V_0 , 0, 0},
	{R_3,  OFF,		CH3,	 BUF_D1,	BUF_FRAME,	H_1440,		V_0 , 0, 0},
	{R_4,  OFF,		CH4,	 BUF_D1,	BUF_FRAME,	H_2160,		V_0 , 0, 0},
	{R_5,  OFF,		CH5,	 BUF_D1,	BUF_FRAME,	H_0,	  	V_576	 , 0, 0},
	{R_6,  OFF,		CH6,	 BUF_D1,	BUF_FRAME,	H_720,    V_576	 , 0, 0},
	{R_7,  OFF,		CH7,	 BUF_D1,	BUF_FRAME,	H_1440,		V_576  , 0, 0},
	{R_8,  OFF,		CH8,	 BUF_D1,	BUF_FRAME,	H_2160,   V_576  , 0, 0},
	{R_PB1, ON,		PB1,	 BUF_D1,	BUF_FRAME,	H_0,	  	V_0 , 0, 0},       
	{R_PB2, ON,		PB2,	 BUF_D1,	BUF_FRAME,	H_720,	  V_0 , 0, 0},       
	{R_PB3, ON,		PB3,	 BUF_D1,	BUF_FRAME,	H_1440, 	V_0 , 0, 0},       
	{R_PB4, ON,		PB4,	 BUF_D1,	BUF_FRAME,	H_2160,   V_0 , 0, 0},       
	{R_PB5, OFF,		PB5,	 BUF_D1,	BUF_FRAME,	H_0,			V_576	 , 0, 0},
	{R_14, OFF,		CH14,	 BUF_D1,	BUF_FRAME,	H_360,    V_576	 , 0, 0},
	{R_15, OFF,		CH15,	 BUF_D1,	BUF_FRAME,	H_720,		V_576  , 0, 0},
	{R_16, OFF,		CH16,	 BUF_D1,	BUF_FRAME,	H_1080,   V_576  , 0, 0},
  {NOBUF,0,0,0,0,0,0}
};

const _RecSpotBuf rTbBuf_All_CIF_Frame[] = {
	{R_1,  ON,		CH1,	 BUF_CIF,	BUF_FRAME,	H_0,	  	V_0, 0, 0},
	{R_2,  ON,		CH2,	 BUF_CIF,	BUF_FRAME,	H_360,		V_0, 0, 0},
	{R_3,  ON,		CH3,	 BUF_CIF,	BUF_FRAME,	H_720,		V_0, 0, 0},
	{R_4,  ON,		CH4,	 BUF_CIF,	BUF_FRAME,	H_1080,		V_0, 0, 0},
	{R_5,  ON,		CH5,	 BUF_CIF,	BUF_FRAME,	H_0,		  V_288_240, 0, 0},
	{R_6,  ON,		CH6,	 BUF_CIF,	BUF_FRAME,	H_360,		V_288_240, 0, 0},
	{R_7,  ON,		CH7,	 BUF_CIF,	BUF_FRAME,	H_720,		V_288_240, 0, 0},
	{R_8,  ON,		CH8,	 BUF_CIF,	BUF_FRAME,	H_1080, 	V_288_240, 0, 0},
	{R_PB1, OFF,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,		  V_576, 0, 0},
	{R_PB2, OFF,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,		V_576, 0, 0},
	{R_PB3, OFF,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,	 	V_576, 0, 0},
	{R_PB4, OFF,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,		V_576, 0, 0},
	{R_PB5, OFF,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,		  V_864_816, 0, 0},
	{R_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,		V_864_816, 0, 0},
	{R_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		V_864_816, 0, 0},
	{R_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080, 	V_864_816, 0, 0},
  {NOBUF,0,0,0,0,0,0}
};

const _RecSpotBuf rTbBuf_All_D1_Field[] = {
	{R_1,  ON,		CH1,	 BUF_D1,	BUF_FIELD,	H_0,		V_0, 0, 0},
	{R_2,  ON,		CH2,	 BUF_D1,	BUF_FIELD,	H_720,		V_0, 0, 0},
	{R_3,  ON,		CH3,	 BUF_D1,	BUF_FIELD,	H_1440,		V_0, 0, 0},
	{R_4,  ON,		CH4,	 BUF_D1,	BUF_FIELD,	H_2160,		V_0, 0, 0},
	{R_5,  ON,		CH5,	 BUF_D1,	BUF_FIELD,	H_0,		V_288_240, 0, 0},
	{R_6,  ON,		CH6,	 BUF_D1,	BUF_FIELD,	H_720,		V_288_240, 0, 0},
	{R_7,  ON,		CH7,	 BUF_D1,	BUF_FIELD,	H_1440,		V_288_240, 0, 0},
	{R_8,  ON,		CH8,	 BUF_D1,	BUF_FIELD,	H_2160,		V_288_240, 0, 0},
	{R_PB1,  OFF,		CH9,	 BUF_D1,	BUF_FIELD,	H_0,		V_576, 0, 0},
	{R_PB2, OFF,		CH10,	 BUF_D1,	BUF_FIELD,	H_720,		V_576, 0, 0},
	{R_PB3, OFF,		CH11,	 BUF_D1,	BUF_FIELD,	H_1440, 	V_576, 0, 0},
	{R_PB4, OFF,		CH12,	 BUF_D1,	BUF_FIELD,	H_2160,		V_576, 0, 0},
	{R_PB5, OFF,		CH13,	 BUF_D1,	BUF_FIELD,	H_0,		V_864_816, 0, 0},
	{R_14, OFF,		CH14,	 BUF_D1,	BUF_FIELD,	H_720,		V_864_816, 0, 0},
	{R_15, OFF,		CH15,	 BUF_D1,	BUF_FIELD,	H_1440,		V_864_816, 0, 0},
	{R_16, OFF,		CH16,	 BUF_D1,	BUF_FIELD,	H_2160,		V_864_816, 0, 0},
  {NOBUF,0,0,0,0,0,0}
};

const _RecSpotBuf rTbBuf_Mix_CIF_Frame[] = {
	{R_1,  ON,		CH1,	 BUF_CIF,	BUF_FRAME,	H_0,	  	V_288_240, 0, 0},
	{R_2,  ON,		CH2,	 BUF_CIF,	BUF_FRAME,	H_360,		V_288_240, 0, 0},
	{R_3,  ON,		CH3,	 BUF_CIF,	BUF_FRAME,	H_720,		V_288_240, 0, 0},
	{R_4,  ON,		CH4,	 BUF_CIF,	BUF_FRAME,	H_1080,		V_288_240, 0, 0},
	{R_5,  ON,		CH5,	 BUF_CIF,	BUF_FRAME,	H_1440,	  V_288_240, 0, 0},
	{R_6,  ON,		CH6,	 BUF_CIF,	BUF_FRAME,	H_1880,		V_288_240, 0, 0},
	{R_7,  ON,		CH7,	 BUF_CIF,	BUF_FRAME,	H_2160,		V_288_240, 0, 0},
	{R_8,  ON,		CH8,	 BUF_CIF,	BUF_FRAME,	H_2520, 	V_288_240, 0, 0},
	{R_PB1, ON,		PB1,	 BUF_CIF,	BUF_FRAME,	H_0,		  V_0, 0, 0},
	{R_PB2, ON,		PB2,	 BUF_CIF,	BUF_FRAME,	H_360,		V_0, 0, 0},
	{R_PB3, ON,		PB3,	 BUF_CIF,	BUF_FRAME,	H_720,	 	V_0, 0, 0},
	{R_PB4, ON,		PB4,	 BUF_CIF,	BUF_FRAME,	H_1080,		V_0, 0, 0},
	{R_PB5, ON, 	PB5,	 BUF_CIF,	BUF_FRAME,	H_1440,	  V_0, 0, 0},
	{R_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,		V_864_816, 0, 0},
	{R_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		V_864_816, 0, 0},
	{R_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080, 	V_864_816, 0, 0},
/*	{S_1,	 ON,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,		  SPT_V_576_480, 0, 0},
	{S_2,	 ON,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,		SPT_V_576_480, 0, 0},
	{S_3,	 ON,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_576_480, 0, 0},
	{S_4,	 ON,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,		SPT_V_576_480, 0, 0},
	{S_5,	 ON,		CH13,	 BUF_CIF,	BUF_FRAME,	H_1440,		SPT_V_576_480, 0, 0},
	{S_6,	 ON,		CH14,	 BUF_CIF,	BUF_FRAME,	H_1880,		SPT_V_576_480, 0, 0},
	{S_7,	 ON,		CH15,	 BUF_CIF,	BUF_FRAME,	H_2160,		SPT_V_576_480, 0, 0},
	{S_8,	 ON,		CH16,	 BUF_CIF,	BUF_FRAME,	H_2520,		SPT_V_576_480, 0, 0},
	{S_9,  OFF,		CH1,	 BUF_CIF,	BUF_FRAME,	H_0,		  SPT_V_1152_960, 0, 0}, 
	{S_10, OFF,		CH10,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_0, 0, 0},
	{S_11, OFF,		CH11,	 BUF_CIF,	BUF_FRAME,	H_1440, 	SPT_V_0, 0, 0},
	{S_12, OFF,		CH12,	 BUF_CIF,	BUF_FRAME,	H_2160,		SPT_V_0, 0, 0},
	{S_13, OFF,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,		  SPT_V_576_480, 0, 0},
	{S_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_576_480, 0, 0},
	{S_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_1440,		SPT_V_576_480, 0, 0},
	{S_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_2160,		SPT_V_576_480, 0, 0},
*/  {NOBUF,0,0,0,0,0,0}
};

//------------------

const u8 NORMAL_REC_PORT_TB[] = {
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,	
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 	
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 	
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 	
0,1,2,3,4,5,6,7,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,	
};

const u8 NORMAL_REC_SPOTBUF_PORT_TB[] = {
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
};


//--------------------------------------------------------------------------
const _RecPortInfo rTbPort_4xD1_27M_Field[] = { 
	{ON,	D1,	FIELD,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[0] },
	{ON,	D1,	FIELD,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[1] },
	{ON,	D1,	FIELD,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[2] },
	{ON,	D1,	FIELD,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[3] },
	{OFF,	D1,	FIELD,	RCLK_27M,	PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_4xD1_27M_IFrame_rPort5_9[] = { 
//	{ON,	CH_9,	IFRAME,	RCLK_27M,	PORT_8BIT,	2, NORMAL_REC_PORT_TB },
//	{ON,	CH_9,	IFRAME,	RCLK_27M,	PORT_8BIT,	9, NORMAL_REC_PORT_TB },
//	{ON,	CH_9,	FIELD,	RCLK_27M,	PORT_8BIT,	9, NORMAL_REC_PORT_TB },
//	{ON,	CH_9,	FIELDSW,	RCLK_27M,	PORT_8BIT,	9, NORMAL_REC_PORT_TB },
	{ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
	{ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[1] },
	{ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[2] },
	{ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , &NORMAL_REC_PORT_TB[3] },
	{OFF,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_2xD1_54M_IFrame[] = { 
	{ ON,	D1,	IFRAME,	RCLK_54M,	PORT_8BIT,	2 , NORMAL_REC_PORT_TB },
	{ ON,	D1,	IFRAME,	RCLK_54M,	PORT_8BIT,	2 , &NORMAL_REC_PORT_TB[2] },
	{ ON,	D1,	IFRAME,	RCLK_54M,	PORT_8BIT,	2 , &NORMAL_REC_PORT_TB[4] },
	{ ON,	D1,	IFRAME,	RCLK_54M,	PORT_8BIT,	2 , &NORMAL_REC_PORT_TB[6] },
	{ OFF,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_D1_108M_IFrame[] = { 
	{ ON,	D1,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
	{ ON,	D1,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , &NORMAL_REC_PORT_TB[4] },
	{ ON,	D1,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , &NORMAL_REC_PORT_TB[8] },
	{ ON,	D1,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , &NORMAL_REC_PORT_TB[12] },
	{ OFF,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_16QUAD_27M_IFrame[] = { 
	{ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , NORMAL_REC_SPOTBUF_PORT_TB },
	{ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , &NORMAL_REC_SPOTBUF_PORT_TB[4] },
	{ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , &NORMAL_REC_SPOTBUF_PORT_TB[8] },
	{ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , &NORMAL_REC_SPOTBUF_PORT_TB[12] },
	{OFF,	QUARD,	IFRAME,RCLK_27M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_16QUAD_54M_IFrame[] = { 
	{OFF,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
	{OFF,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , &NORMAL_REC_PORT_TB[4] },
	{ON,	QUARD,	IFRAME,	RCLK_54M,	PORT_8BIT,	8 , &NORMAL_REC_SPOTBUF_PORT_TB[0] },
	{ON,	QUARD,	IFRAME,	RCLK_54M,	PORT_8BIT,	8 , &NORMAL_REC_SPOTBUF_PORT_TB[8] },
	{OFF,	QUARD,	IFRAME,RCLK_27M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_4D1_108M_IFrame[] = { 
	{ ON,	  D1_4,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
	{ OFF,	D1_4,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
	{ ON,	  D1_4,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , &NORMAL_REC_PORT_TB[8] },
	{ OFF,	D1_4,	IFRAME,	RCLK_108M,	PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
	{ OFF,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
	
};

const _RecPortInfo rTbPort_2_8D1_108M_IFrame[] = { 
	{ ON,	D1,	IFRAME,	RCLK_108M,	  PORT_16BIT,	8 , NORMAL_REC_PORT_TB },
	{ OFF,	D1,	IFRAME,	RCLK_108M,	PORT_8BIT,	0 , 0 },
	{ ON,	D1,	IFRAME,	RCLK_108M,	  PORT_16BIT,	8 , &NORMAL_REC_PORT_TB[8] },
	{ OFF,	D1,	IFRAME,	RCLK_108M,	PORT_8BIT,	0 , 0 },
	{ OFF,	D1,	IFRAME,	RCLK_27M,	    PORT_8BIT,	1 , NORMAL_REC_PORT_TB },
};

const _RecPortInfo rTbPort_2_8D1_108M_Field_n_16D1_IFrame_n_FW[] = { 
	{ OFF,D1,	IFRAME,	  RCLK_108M,	PORT_8BIT,	16 , &NORMAL_REC_PORT_TB[0] },
	{ OFF,D1,	QUARD,	  RCLK_27M,	  PORT_8BIT,	4 , &NORMAL_REC_SPOTBUF_PORT_TB[0] }, //Use SPOTBuf  HR-05312011
	{ ON,	D1,	FIELDSW,	RCLK_108M,	PORT_8BIT,	8 , NORMAL_REC_PORT_TB },
	{ ON,	D1,	FIELDSW,	RCLK_108M,	PORT_8BIT,	8 , &NORMAL_REC_PORT_TB[8] },
	{ OFF,D1,	QUARD,	  RCLK_27M,	  PORT_8BIT,	4 , NORMAL_REC_PORT_TB },
};

//--------------------------------------------------------------------------
const _RecPinInfo rTbPin_27M_8b[] = { 
	{CLK_NORMAL,	PIN_8BIT,	CLK_74M, RECP5_7_0}, // Pin1  ?? 
	{CLK_NORMAL,	PIN_8BIT,	CLK_74M, RECP6_7_0}, // Pin2  Hsync Vsync
//	{CLK_NORMAL,	PIN_8BIT,	CLK_27M, RECP7_7_0}, // Pin3
	{CLK_NORMAL,	PIN_8BIT,	CLK_27M, RECP5_7_0}, // Pin3
	{CLK_NORMAL,	PIN_8BIT,	CLK_27M, RECP6_7_0}, // Pin4
};
const _RecPinInfo rTbPin_54M_8b[] = { 
	{CLK_NORMAL,	PIN_8BIT,	CLK_54M, RECP5_7_0}, // Pin1
	{CLK_NORMAL,	PIN_8BIT,	CLK_54M, RECP6_7_0}, // Pin2
	{CLK_NORMAL,	PIN_8BIT,	CLK_54M, RECP7_7_0}, // Pin3
	{CLK_NORMAL,	PIN_8BIT,	CLK_54M, RECP8_7_0}, // Pin4
};
const _RecPinInfo rTbPin_54M_16b[] = { 
	{CLK_NORMAL,	PIN_16BIT,	CLK_54M, RECP5_7_0},
	{CLK_NORMAL,	PIN_16BIT,	CLK_54M, RECP5_15_8},
	{CLK_NORMAL,	PIN_16BIT,	CLK_54M, RECP7_7_0},
	{CLK_NORMAL,	PIN_16BIT,	CLK_54M, RECP7_15_8},
};

const _RecPinInfo rTbPin_108M_8b_I[] = { 
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP5_7_0},
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP6_7_0},
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP7_7_0},
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP8_7_0},
};

const _RecPinInfo rTbPin_108M_8b[] = { 
	{CLK_NORMAL,	PIN_8BIT,	CLK_108M, RECP5_7_0},
	{CLK_NORMAL,	PIN_8BIT,	CLK_108M, RECP6_7_0},
	{CLK_NORMAL,	PIN_8BIT,	CLK_108M, RECP7_7_0},
	{CLK_NORMAL,	PIN_8BIT,	CLK_108M, RECP8_7_0},
};

const _RecPinInfo rTbPin_54M_BT1120[] = {
	{CLK_NORMAL,	PIN_16BIT_BT1120,	CLK_54M, RECP5_7_0},
	{CLK_NORMAL,	PIN_16BIT_BT1120,	CLK_54M, RECP5_7_0},
	{CLK_NORMAL,	PIN_16BIT_BT1120,	CLK_54M, RECP7_7_0},
	{CLK_NORMAL,	PIN_16BIT_BT1120,	CLK_54M, RECP7_15_8},

};

const _RecPinInfo rTbPin_108M_16b_I[] = { 
	{CLK_INVERT,	PIN_16BIT,	CLK_108M, RECP5_7_0},
	{CLK_INVERT,	PIN_16BIT,	CLK_108M, RECP5_15_8},
	{CLK_INVERT,	PIN_16BIT,	CLK_108M, RECP7_7_0},
	{CLK_INVERT,	PIN_16BIT,	CLK_108M, RECP7_15_8},

};

const _RecPinInfo rTbPin_108M_8b_2[] = { 
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP5_7_0},
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP6_7_0},
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP7_7_0}, 
	{CLK_INVERT,	PIN_8BIT,	CLK_108M, RECP8_7_0}, 

};



//-------------------------------------------------------------------------

_RecAllInfo rTb_Rec_4xD1_27M_8b_IFrame_rport5_8 = {

	rTbBuf_1_8_D1_Frame, 		

//	rTbPort_2xD1_54M_IFrame,
	rTbPort_4xD1_27M_IFrame_rPort5_9,

	rTbPin_27M_8b,
};

_RecAllInfo rTb_Rec_16Quad_27M_8b_IFrame = { 

	rTbBuf_All_CIF_Frame, 		

	rTbPort_16QUAD_27M_IFrame,

	rTbPin_27M_8b,
	
};

_RecAllInfo rTb_Spot_16Quad_54M_8b_IFrame = { 

	sTbBuf_QUAD_Frame, 		

	rTbPort_16QUAD_54M_IFrame,

	rTbPin_54M_8b,
	
};

_RecAllInfo rTb_Rec_4D1_54M_16b_IFrame = {

	pTbBuf_All_D1_Frame, 		

	rTbPort_4D1_108M_IFrame,

	rTbPin_54M_16b,
	
};

_RecAllInfo rTb_Rec_D1_108M_8b_Frame = {

	pTbBuf_All_D1_Frame, 		

	rTbPort_D1_108M_IFrame,

	rTbPin_108M_8b_I,
	
};

_RecAllInfo rTb_Rec_2_8D1_108M_16b_Frame = {

	pTbBuf_All_D1_Frame, 		

	rTbPort_2_8D1_108M_IFrame,

	rTbPin_108M_16b_I,
	
};


//------------------------------------------------
// Config: 
//   rbuf:  sBuf used, 0~16 D1 frame
//   rport: 7,8 - each 8D1 field 108MHz, formatter on
//					1~6 - Disable  
//   rpin : 1,2 - 
//					3,4 - output rport 7,8 108MHz
//------------------------------------------------

_RecAllInfo rTb_Rec_16D1_108M_8b_NewFieldSwitch = {

	pTbBuf_All_D1_Frame, 		

	rTbPort_2_8D1_108M_Field_n_16D1_IFrame_n_FW,

	rTbPin_108M_8b_2,
};




#endif




