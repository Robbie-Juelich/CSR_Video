
#ifndef _EVB_TBL_H_
#define _EVB_TBL_H_


#include "tw2828_twmux.h"

const _twmux Default_NTSC_960_1080i = {
  .id      = TW2828_1,
	ctrl    : 0x00,
	ilvtype : NTSC_960,
	ipbtype : { HD_1080I, HD_1080I, HD_1080I, HD_1080I, HD_1080I },
	ilvp    : {.p=PORT_LIVE_0,intype:NTSC_960, IHR:IHR_960, IVR:IVR_NTSC, Interlaced:1, EmChId:0, YCSwap:0 },
	ipbp    :{{.p=PORT_PB_1, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_2, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_3, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_4, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_5, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:0},},
	mdrs    : rHD1080P,	
	PHR			: 1920,	PVR     : 1080,	UHR			: 1920,	UVR     : 1080,
};                                                                            

const _twmux Default_PAL_960_1080i = {
  id      : TW2828_1,
	ctrl    : 0x00,
	ilvtype : PAL_960,
	ipbtype : { HD_1080I, HD_1080I, HD_1080I, HD_1080I, HD_1080I },
	ilvp    : {.p=PORT_LIVE_0,intype:NTSC_960, IHR:IHR_960, IVR:IVR_NTSC, Interlaced:1, EmChId:0, YCSwap:0 },
	ipbp    :{{.p=PORT_PB_1, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_2, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_3, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_4, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:1},
            {.p=PORT_PB_5, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:0},},
	mdrs    : rHD1080P,	
	PHR			: 1920,	PVR     : 1080,	UHR			: 1920,	UVR     : 1080,
};

const _twmux Default_NTSC_720_1080i = {
  id      : TW2828_0,
	ctrl    : 0x00,
	ilvtype : NTSC_720,
	ipbtype : { HD_1080I, HD_1080I, HD_1080I, HD_1080I, HD_1080I },
	ilvp    : {.p=PORT_LIVE_0,intype:NTSC_720, IHR:IHR_720, IVR:IVR_NTSC, Interlaced:1, EmChId:0, YCSwap:0 },
	ipbp    :{{.p=PORT_PB_1, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:0, YCSwap:0},
            {.p=PORT_PB_2, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_3, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_4, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_5, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},},
	mdrs    : r1080I,	
	PHR			: 1920,	PVR     : 540,	UHR			: 1920,	UVR     : 540,
};

const _twmux Default_PAL_720_1080i = {
  id      : TW2828_0,
	ctrl    : 0x00,
	ilvtype : PAL_720,
	ipbtype : { HD_1080I, HD_1080I, HD_1080I, HD_1080I, HD_1080I },
	ilvp    : {.p=PORT_LIVE_0,intype:NTSC_720, IHR:IHR_720, IVR:IVR_NTSC, Interlaced:1, EmChId:0, YCSwap:0 },
	ilvp    : {.p=PORT_LIVE_0,intype:PAL_720, IHR:IHR_720, IVR:IVR_PAL, Interlaced:1, EmChId:0, YCSwap:0 },
#if 1
	ipbp    :{{.p=PORT_PB_1, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_2, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_3, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_4, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_5, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},},
#endif
	mdrs    : r1080I,	
	PHR			: 1920,	PVR     : 1080,	UHR			: 1920,	UVR     : 1080,
};

const _twmux Default_PAL_720_1080P = {
  id      : TW2828_0,
	ctrl    : 0x00,
	ilvtype : PAL_720,
	ipbtype : { HD_1080I, HD_1080I, HD_1080I, HD_1080I, HD_1080I },
	ilvp    : {.p=PORT_LIVE_0,intype:NTSC_720, IHR:IHR_720, IVR:IVR_NTSC, Interlaced:1, EmChId:0, YCSwap:0 },
	ilvp    : {.p=PORT_LIVE_0,intype:PAL_720, IHR:IHR_720, IVR:IVR_PAL, Interlaced:1, EmChId:0, YCSwap:0 },
#if 1
	ipbp    :{{.p=PORT_PB_1, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_2, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_3, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_4, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},
            {.p=PORT_PB_5, intype:HD_1080I, IHR:1920, IVR:540, Interlaced:TRUE, EmChId:FALSE, YCSwap:FALSE},},
#endif
	mdrs    : rHD1080P,	
	PHR			: 1920,	PVR     : 1080,	UHR			: 1920,	UVR     : 1080,
};


const _twmux Default_PAL_720_1024x768 = {
  id      : TW2828_0,
    ctrl    : 0x00,
    ilvtype : PAL_720,
    ipbtype : { HD_1080I, HD_1080I, HD_1080I, HD_1080I, HD_1080I },
    ilvp    : {.p=PORT_LIVE_0,intype:NTSC_720, IHR:IHR_720, IVR:IVR_NTSC, Interlaced:1, EmChId:0, YCSwap:0 },
    ilvp    : {.p=PORT_LIVE_0,intype:PAL_720, IHR:IHR_720, IVR:IVR_PAL, Interlaced:1, EmChId:0, YCSwap:0 },
    mdrs    : r1024x768,
    PHR         : 1024, PVR     : 768,  UHR         : 1024, UVR     : 768,
};


_actFunc msdpTbl[] = {
	{ MODE_20CH, 					     "MODE_20CH", 					    LoadMdspDb_20CH, 0, 0, 0   },
	{ MODE_6CH,  					     "MODE_6CH",  					    LoadMdspDb_6CH,  0, 0, 0   },
	{ MODE_12CH, 					     "MODE_12CH", 					    LoadMdspDb_12CH, 0, 0, 0   },
	{ MODE_HD_FULL,            "MODE_HD_FULL",            LoadMdspDb_HD_Full, 0, 0, 0   },
	{ MODE_HD_HALF,            "MODE_HD_HALF",            LoadMdspDb_HD_Half, 0, 0, 0   },
	{ MODE_16PB_4LIVE,         "MODE_16PB_4LIVE",         LoadMdspDb_16D1PB4Live, 0, 0, 0   },
	{ MODE_16QUADPB,           "MODE_16QUADPB",           LoadMdspDb_16QuadPB, 0, 0, 0   },
	{ MODE_32CH,               "MODE_32CH",               LoadMdspDb_32CH, 0, 0, 0   },
	{ MODE_2_8D1PB_4LIVE,      "MODE_2_8D1PB_4LIVE",      LoadMdspDb_2_8D1PB4Live, 0, 0, 0   },
	{ MODE_4D1_4CUT,           "MODE_4D1_4CUT",           LoadMdspDb_2_4D1_4Cut, 0, 0, 0   },
	{ MODE_4HD_PB,             "MODE_4HD_PB",             LoadMdspDb_4HD_PB, 0, 0, 0   },
	{ MODE_6VGA_16CUT,         "MODE_6VGA_16CUT",         LoadMdspDb_6VGA_16Cut, 0, 0, 0   },
	{ MODE_32CH_ONLY,          "MODE_32CH_ONLY",          LoadMdspDb_16PB_16Live, 0, 0, 0   },
	{ MODE_32CH_ONLY_2,        "MODE_32CH_ONLY_2",        LoadMdspDb_16LiveN16PB, 0, 0, 0   },
	{ MODE_12CH_CASCADE_Master,"MODE_12CH_CASCADE_Master",LoadMdspDb_12Live_Master, 0, 0, 0   },
	{ MODE_1HD_8D1PB,          "MODE_1HD_8D1PB",          LoadMdspDb_1HDn8D1PBn4Live, 0, 0, 0   },
	{ MODE_16D1_FS_NEW,        "MODE_16D1_FS_NEW",        LoadMdspDb_16D1_NewFieldSwitching, 0, 0, 0   },
	{ MODE_32CH_ONLY_3,        "MODE_32CH_ONLY_3",        LoadMdspDb_16LiveN16PB_FieldSwitch, 0, 0, 0   },
	{ MODE_1HD_16CUT,          "MODE_1HD_16CUT",          LoadMdspDb_1HD_16Cut, 0, 0, 0   },
	{ MODE_1HD_8CUT_8Live,     "MODE_1HD_8CUT_8Live",     LoadMdspDb_1HD_8Cut_8Live, 0, 0, 0   },
	{ MODE_16LIVE_4HD,         "MODE_16LIVE_4HD",         LoadMdspDb_16LIVE_4HD, 0, 0, 0   },
	{ MODE_5PB_5LIVE,          "MODE_5PB_5LIVE",          LoadMdspDb_5PB_5LIVE, 0, 0, 0   },
	{ MODE_5PB_1LIVE,          "MODE_5PB_1LIVE",          LoadMdspDb_5PB_1LIVE, 0, 0, 0   },
	{ MODE_36CH,               "MODE_36CH",               LoadMdspDb_16Live_20PB, 0, 0, 0   },
	{ MODE_CASCADE_PB5_LIVE,   "MODE_CASCADE_PB5_LIVE",   LoadMdspDb_PB5toLive_4HD, 0, 0, 0   },
	{ MODE_16LIVE_HDSPOT,      "MODE_16LIVE_HDSPOT",      LoadMdspDb_16Live_HDSpot, 0, 0, 0   },
	{ MODE_16LIVE_HDSPOT1,     "MODE_16LIVE_HDSPOT1",     LoadMdspDb_16Live_HDSpot1, 0, 0, 0   },
	{ MODE_4CH_HDSPOT_DEMO,    "MODE_4CH_HDSPOT_DEMO",    LoadMdspDb_4CH_HDSpot_Demo, 0, 0, 0   },
	{ MODE_4CH,                "MODE_4CH",                LoadMdspDb_4CH, 0, 0, 0   },
	{ MODE_16LIVE_HDSPOT2,     "MODE_16LIVE_HDSPOT2",     LoadMdspDb_16Live_HDSpot2, 0, 0, 0   },
	{ MODE_9CH,                "MODE_9CH",                LoadMdspDb_9CH, 0, 0, 0   },
	{ MODE_16LIVE_HDSPOT3,     "MODE_16LIVE_HDSPOT3",     LoadMdspDb_16Live_HDSpot2, 0, 0, 0   },
	{ MODE_8CH,                "MODE_8CH",                LoadMdspDb_8CH, 0, 0, 0   },
  
  { MAXMODE, 0, 0, 0, 0 ,0 }  //end of data 
	
};


#endif

