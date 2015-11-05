#ifndef _DISP_TBL_H_
#define _DISP_TBL_H_

#ifdef __cplusplus
 extern "C" {
 #endif 

#include "tw2828_twmux.h"

//------------------------------------------------------------------------
// MDSP Mode channel to port table

static const _DataChannelInfo tb_36CH[] = {
// 16Live + 16Quad 27Mhz-8bit : 4 Quad*4Port, Each port runs 27Mhz.
{ CHx1 ,	PORT_LIVE_0},	
{ CHx2 ,	PORT_LIVE_1},	
{ CHx3 ,	PORT_LIVE_2},	
{ CHx4 ,	PORT_LIVE_3},	
{ CHx5 ,	PORT_LIVE_4},	
{ CHx6 ,	PORT_LIVE_5},	
{ CHx7 ,	PORT_LIVE_6},	
{ CHx8 ,	PORT_LIVE_7},	
{ CHx9 ,	PORT_LIVE_8},	
{ CHx10,	PORT_LIVE_9},	
{ CHx11,	PORT_LIVE_10},	
{ CHx12,	PORT_LIVE_11},	
{ CHx13,	PORT_LIVE_12},	
{ CHx14,	PORT_LIVE_13},	
{ CHx15,	PORT_LIVE_14},	
{ CHx16,	PORT_LIVE_15},	
{ PBx1,   PORT_PB_1},	
{ PBx2,   PORT_PB_1},	
{ PBx3,   PORT_PB_1},	
{ PBx4,   PORT_PB_1},	
{ PBx5,   PORT_PB_2},	
{ PBx6,   PORT_PB_2},	
{ PBx7,   PORT_PB_2},	
{ PBx8,   PORT_PB_2},	
{ PBx9,   PORT_PB_3},	
{ PBx10,  PORT_PB_3},	
{ PBx11,  PORT_PB_3},	
{ PBx12,  PORT_PB_3},	
{ PBx13,  PORT_PB_4},	
{ PBx14,  PORT_PB_4},	
{ PBx15,  PORT_PB_4},	
{ PBx16,  PORT_PB_4},	
{ CAS1,   PORT_PB_5},	
{ CAS2,   PORT_PB_5},	
{ CAS3,   PORT_PB_5},	
{ CAS4,   PORT_PB_5},	
};

static const _DataChannelInfo tb_32CH_FS[] = {
// 16Live + 16D1 onefield 108Mhz-8bit : 8 D1 half*2Port, Each port runs 108Mhz.
{ CHx1 ,	PORT_LIVE_0},	
{ CHx2 ,	PORT_LIVE_1},	
{ CHx3 ,	PORT_LIVE_2},	
{ CHx4 ,	PORT_LIVE_3},	
{ CHx5 ,	PORT_LIVE_4},	
{ CHx6 ,	PORT_LIVE_5},	
{ CHx7 ,	PORT_LIVE_6},	
{ CHx8 ,	PORT_LIVE_7},	
{ CHx9 ,	PORT_LIVE_8},	
{ CHx10,	PORT_LIVE_9},	
{ CHx11,	PORT_LIVE_10},	
{ CHx12,	PORT_LIVE_11},	
{ CHx13,	PORT_LIVE_12},	
{ CHx14,	PORT_LIVE_13},	
{ CHx15,	PORT_LIVE_14},	
{ CHx16,	PORT_LIVE_15},	
/*{ PBx1,   PORT_PB_1},	
{ PBx2,   PORT_PB_1},	
{ PBx3,   PORT_PB_1},	
{ PBx4,   PORT_PB_1},	
{ PBx5,   PORT_PB_1},	
{ PBx6,   PORT_PB_1},	
{ PBx7,   PORT_PB_1},	
{ PBx8,   PORT_PB_1},	
{ PBx9,   PORT_PB_2},	
{ PBx10,  PORT_PB_2},	
{ PBx11,  PORT_PB_2},	
{ PBx12,  PORT_PB_2},	
{ PBx13,  PORT_PB_2},	
{ PBx14,  PORT_PB_2},	
{ PBx15,  PORT_PB_2},	
{ PBx16,  PORT_PB_2},	*/
{ PBx1,   PORT_PB_3},	
{ PBx2,   PORT_PB_3},	
{ PBx3,   PORT_PB_3},	
{ PBx4,   PORT_PB_3},	
{ PBx5,   PORT_PB_3},	
{ PBx6,   PORT_PB_3},	
{ PBx7,   PORT_PB_3},	
{ PBx8,   PORT_PB_3},	
{ PBx9,   PORT_PB_4},	
{ PBx10,  PORT_PB_4},	
{ PBx11,  PORT_PB_4},	
{ PBx12,  PORT_PB_4},	
{ PBx13,  PORT_PB_4},	
{ PBx14,  PORT_PB_4},	
{ PBx15,  PORT_PB_4},	
{ PBx16,  PORT_PB_4},	

};

#ifdef __cplusplus
 }
#endif 

#endif
