
#ifndef _SPOT_TBL_H_
#define _SPOT_TBL_H_

const _RecSpotBuf sTbBuf_1_8_D1_Frame[] = {
	{S_1,  ON,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,	  	SPT_V_0, 0, 0 },
	{S_2,  ON,		CH2,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_0, 0, 0 },
	{S_3,  ON,		CH3,	 BUF_D1,	BUF_FRAME,	H_1440,		SPT_V_0, 0, 0 },
	{S_4,  ON,		CH4,	 BUF_D1,	BUF_FRAME,	H_2160,		SPT_V_0, 0, 0 },
	{S_5,  OFF,		CH5,	 BUF_D1,	BUF_FRAME,	H_0,	  	SPT_V_576_480, 0, 0 },
	{S_6,  OFF,		CH6,	 BUF_D1,	BUF_FRAME,	H_720,    SPT_V_576_480, 0, 0 },
	{S_7,  OFF,		CH7,	 BUF_D1,	BUF_FRAME,	H_1440,		SPT_V_576_480, 0, 0 },
	{S_8,  OFF,		CH8,	 BUF_D1,	BUF_FRAME,	H_2160,   SPT_V_576_480, 0, 0 },
	{S_9,  OFF,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_288_240, 0, 0 },
	{S_10, OFF,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_288_240, 0, 0 },
	{S_11, OFF,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_288_240, 0, 0 },
	{S_12, OFF,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_288_240, 0, 0 },
	{S_13, OFF,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_432_360, 0, 0 },
	{S_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_432_360, 0, 0 },
	{S_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_432_360, 0, 0 },
	{S_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_432_360, 0, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};

const _RecSpotBuf sTbBuf_1_8_8HalfD1_Field[] = {
	{S_1,  ON,		CH1,	 BUF_HALFD1,	BUF_FIELD,	H_0,		  SPT_V_0, 0, 0 }, 
	{S_2,  ON,		CH2,	 BUF_HALFD1,	BUF_FIELD,	H_360,		SPT_V_0, 0, 0 }, 
	{S_3,  OFF,		CH3,	 BUF_HALFD1,	BUF_FIELD,	H_720,		SPT_V_0, 0, 0 }, 
	{S_4,  OFF,		CH4,	 BUF_HALFD1,	BUF_FIELD,	H_1080,		SPT_V_0, 0, 0 }, 
	{S_5,  OFF,		CH5,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_144_120, 0, 0 },
	{S_6,  OFF,		CH6,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_144_120, 0, 0 },
	{S_7,  OFF,		CH7,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_144_120, 0, 0 },
	{S_8,  OFF,		CH8,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_144_120, 0, 0 },
	{S_9,  OFF,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_288_240, 0, 0 },
	{S_10, OFF,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_288_240, 0, 0 },
	{S_11, OFF,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_288_240, 0, 0 },
	{S_12, OFF,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_288_240, 0, 0 },
	{S_13, OFF,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_432_360, 0, 0 },
	{S_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_432_360, 0, 0 },
	{S_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_432_360, 0, 0 },
	{S_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_432_360, 0, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};

const _RecSpotBuf sTbBuf_QUAD_Field[] = {
	{S_1,  ON,		CH1,	 BUF_CIF,	BUF_FIELD,	H_0,		SPT_V_0, 0, 0 },
	{S_2,  ON,		CH2,	 BUF_CIF,	BUF_FIELD,	H_360,  SPT_V_0, 0, 0 },
	{S_3,  ON,		CH3,	 BUF_CIF,	BUF_FIELD,	H_0,		SPT_V_288_240, 0, 0 },
	{S_4,  ON,		CH4,	 BUF_CIF,	BUF_FIELD,	H_360,  SPT_V_288_240, 0, 0 },
	{S_5,  OFF,		CH5,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_144_120, 0, 0 },
	{S_6,  OFF,		CH6,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_144_120, 0, 0 },
	{S_7,  OFF,		CH7,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_144_120, 0, 0 },
	{S_8,  OFF,		CH8,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_144_120, 0, 0 },
	{S_9,  OFF,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_288_240, 0, 0 },
	{S_10, OFF,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_288_240, 0, 0 },
	{S_11, OFF,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_288_240, 0, 0 },
	{S_12, OFF,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_288_240, 0, 0 },
	{S_13, OFF,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_432_360, 0, 0 },
	{S_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_432_360, 0, 0 },
	{S_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_432_360, 0, 0 },
	{S_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_432_360, 0, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};

const _RecSpotBuf sTbBuf_1_4_D1_Frame[] = {
	{S_1,  ON,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,	SPT_V_0, 0, 0 },
	{S_2,  OFF,		CH2,	 BUF_D1,	BUF_FRAME,	H_0,	SPT_V_0, 0, 0 },
	{S_3,  OFF,		CH3,	 BUF_D1,	BUF_FRAME,	H_0,	SPT_V_0, 0, 0 },
	{S_4,  OFF,		CH4,	 BUF_D1,	BUF_FRAME,	H_0,	SPT_V_0, 0, 0 },
	{S_5,  OFF,		CH5,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_144_120, 0, 0 },
	{S_6,  OFF,		CH6,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_144_120, 0, 0 },
	{S_7,  OFF,		CH7,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_144_120, 0, 0 },
	{S_8,  OFF,		CH8,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_144_120, 0, 0 },
	{S_9,  OFF,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_288_240, 0, 0 },
	{S_10, OFF,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_288_240, 0, 0 },
	{S_11, OFF,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_288_240, 0, 0 },
	{S_12, OFF,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_288_240, 0, 0 },
	{S_13, OFF,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_432_360, 0, 0 },
	{S_14, OFF,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_432_360, 0, 0 },
	{S_15, OFF,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_432_360, 0, 0 },
	{S_16, OFF,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_432_360, 0, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};

const _RecSpotBuf sTbBuf_QUAD_Frame[] = {
	{S_1,  ON,		CH1,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_0, 0, 0 },
	{S_2,  ON,		CH2,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_0, 0, 0 },
	{S_3,  ON,		CH3,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_0, 0, 0 },
	{S_4,  ON,		CH4,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_0, 0, 0 },
	{S_5,  ON,		CH5,	 BUF_CIF,	BUF_FRAME,	H_1440,		SPT_V_0, 0, 0 },
	{S_6,  ON,		CH6,	 BUF_CIF,	BUF_FRAME,	H_1880,   SPT_V_0, 0, 0 },
	{S_7,  ON,		CH7,	 BUF_CIF,	BUF_FRAME,	H_2160,		SPT_V_0, 0, 0 },
	{S_8,  ON,		CH8,	 BUF_CIF,	BUF_FRAME,	H_2520,   SPT_V_0, 0, 0 },
	{S_9,  ON,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_288_240, 0, 0 },
	{S_10, ON,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_288_240, 0, 0 },
	{S_11, ON,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_288_240, 0, 0 },
	{S_12, ON,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_288_240, 0, 0 },
	{S_13, ON,		CH13,	 BUF_CIF,	BUF_FRAME,	H_1440,	  SPT_V_288_240, 0, 0 },
	{S_14, ON,		CH14,	 BUF_CIF,	BUF_FRAME,	H_1880,   SPT_V_288_240, 0, 0 },
	{S_15, ON,		CH15,	 BUF_CIF,	BUF_FRAME,	H_2160,	  SPT_V_288_240, 0, 0 },
	{S_16, ON,		CH16,	 BUF_CIF,	BUF_FRAME,	H_2520,   SPT_V_288_240, 0, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};

const _RecSpotBuf sTbBuf_QCIF_Frame[] = {
	{S_1,  ON,		CH1,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_0, 1, 0 },
	{S_2,  ON,		CH2,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_0, 1, 0 },
	{S_3,  ON,		CH3,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_0, 1, 0 },
	{S_4,  ON,		CH4,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_0, 1, 0 },
	{S_5,  ON,		CH5,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_144_120, 1, 0 },
	{S_6,  ON,		CH6,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_144_120, 1, 0 },
	{S_7,  ON,		CH7,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_144_120, 1, 0 },
	{S_8,  ON,		CH8,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_144_120, 1, 0 },
	{S_9,  ON,		CH9,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_288_240, 1, 0 },
	{S_10, ON,		CH10,	 BUF_CIF,	BUF_FRAME,	H_360,   	SPT_V_288_240, 1, 0 },
	{S_11, ON,		CH11,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_288_240, 1, 0 },
	{S_12, ON,		CH12,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_288_240, 1, 0 },
	{S_13, ON,		CH13,	 BUF_CIF,	BUF_FRAME,	H_0,			SPT_V_432_360, 1, 0 },
	{S_14, ON,		CH14,	 BUF_CIF,	BUF_FRAME,	H_360,    SPT_V_432_360, 1, 0 },
	{S_15, ON,		CH15,	 BUF_CIF,	BUF_FRAME,	H_720,		SPT_V_432_360, 1, 0 },
	{S_16, ON,		CH16,	 BUF_CIF,	BUF_FRAME,	H_1080,   SPT_V_432_360, 1, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};


// Note: For 9CH, use D1 format, but position will count CIF
const _RecSpotBuf sTbBuf_9CH_Frame[] = {
// 9CH(CIF) + 3D1 + 1QUAD(CIF)
	{S_1,  ON,		CH1,	 BUF_D1,	BUF_FRAME,	H_0,			SPT_V_0, 1, 1 },
	{S_2,  ON,		CH2,	 BUF_D1,	BUF_FRAME,	H_360,   	SPT_V_0, 1, 1 },
	{S_3,  ON,		CH3,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_0, 1, 1 },
	{S_4,  ON,		CH4,	 BUF_D1,	BUF_FRAME,	H_0,			SPT_V_144_120, 1, 1 },  
	{S_5,  ON,		CH5,	 BUF_D1,	BUF_FRAME,	H_360,    SPT_V_144_120, 1, 1 },  
	{S_6,  ON,		CH6,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_144_120, 1, 1 },  
	{S_7,  ON,		CH7,	 BUF_D1,	BUF_FRAME,	H_0,			SPT_V_288_240, 1, 1 },
	{S_8,  ON,		CH8,	 BUF_D1,	BUF_FRAME,	H_360,   	SPT_V_288_240, 1, 1 },
	{S_9,  ON,		CH9,	 BUF_D1,	BUF_FRAME,	H_720,		SPT_V_288_240, 1, 1 },
	{S_10, ON,		CH10,	 BUF_D1,	BUF_FRAME,	H_1440,   SPT_V_0, 1, 1 },
	{S_11, ON,		CH11,	 BUF_D1,	BUF_FRAME,	H_2160,		SPT_V_0, 0, 0 },
	{S_12, ON,		CH12,	 BUF_D1,	BUF_FRAME,	H_1440,   SPT_V_576_480, 0, 0 },
	{S_13, ON,		CH13,	 BUF_CIF,	BUF_FRAME,	H_2160,		SPT_V_576_480, 0, 0 },
	{S_14, ON,		CH14,	 BUF_CIF,	BUF_FRAME,	H_2520,   SPT_V_576_480, 0, 0 },
	{S_15, ON,		CH15,	 BUF_CIF,	BUF_FRAME,	H_2160,		SPT_V_720_600, 0, 0 },
	{S_16, ON,		CH16,	 BUF_CIF,	BUF_FRAME,	H_2520,    SPT_V_720_600, 0, 0 },
  {NOBUF,0,0,0,0,0,0,0,0}
};


#endif
