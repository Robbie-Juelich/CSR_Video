
#ifndef _TW2828_TBL_H_
#define _TW2828_TBL_H_

#define TW2828PARA					0

static const u16 tw2828tbl_Default[] = {

0xfffe, TW2828PARA,	// Change Slave Address
0x201, 0x28, //0x2c, //Enable BT1120 Display Output to REC1
             // will call TWMux_BT1120_OnOff later to turn on  BT1220
0x210, 0xb4, //DISP_Pitch = 2880 pxl
//0x210, 0x78, //DISP_Pitch = 2880 pxl
0x211, 0x07, //Disp_MEM_Size = 1G
0x212, 0x07, //Rec_MEM_Size = 1G

//0x21f, 0x02, // Select SPOT DAC
//0x21f, 0x01,   // Select select display
0x21f, 0x61,    // bit 7   [0] Port 1, 2 select BT1120 display output
				// bit 6:5 [3] Port 3, 4 select record  output
                // bit 0   [1] select display output 
                 //
0x2b0, 0x1,
0x2b1, 0x1,

0x539,  0x00,  //  Background R
0x53a,  0x00,  //  Background G
0x53b,  0x00,  //  Background B  Changed by Heejeong to make black in no video.

//0x4f6, 0,  // No Video color B
0x4f4, 0x0,
0x4f5, 0x0,
0x4f6, 0x0,  // No Video color B

0x400,  0x1c,  //
0x401,  0x02,  //
0x402,  0x80,
0x403,  0x07,
0x40e,  0x80,  //
0x40f,  0x07,  //
0x410,  0x38,  //
0x411,  0x04,  //

0x4cf, 0x61, //Disable DI_test mode
0x485, 0x10, //BND_EN=1
//0x485, 0x14, //BND_EN=1
0x4da, 0x52, //OSD_CH_EN_SEL=1, BND_CH_EN=1

//0x06fb,  0x0c,  //
//0x03c5,  0x04,  // chid_bit
//0x3c6,   0x01,  // NTSC
0x3c6,   0x00,  // PAL
#if defined(VIDEO_2D1)
0x21b, 0x01,	//IN_RATE, 54M for Live
#else 
#error "Notice, we use 2d1 by default"
0x21b, 0x00,	//IN_RATE, 108M for Live
#endif
//0x21b, 0x02,	//IN_RATE, 27M for Live

//#ifdef SUPPORT_SMPLOSD
// Simple OSD for Main
//0x080, 0x1e, // Default OSD: OFF  
//0x080, 0x1f, // Default OSD: ON  
0x080, 0x3, // only enable ch no
0x081, 0xc0, // 22x22 
0x082, 0x50, 
0x083, 0x59, 
0x084, 0x00, 
//0x085, 0x40, 
0x085, 0x10,   // changed to 0x10
0x086, 0x00, 
0x087, 0x26, 
0x088, 0x00, 
0x089, 0x10, 
0x08a, 0x00, 
0x08b, 0x80, 
0x08c, 0x01, 
0x08d, 0xb0, 
0x08e, 0x03, 
0x08f, 0xa0, 
0x090, 0x01, 
0x091, 0x96, 
0x092, 0x00, 
//#endif
//mouse
0x544, 0x00,

// MD Detection Enable
0x818, 0x08,  // bd cell sens:60%, bd level sens:8
0x819, 0x33,  // nd level sens:3, nd temp sens:3

0x858, 0x08,
0x859, 0x33, 

0x898, 0x08, 
0x899, 0x33,   

0x8d8, 0x08,
0x8d9, 0x33, 

0x918, 0x08,
0x919, 0x33,

0x958, 0x08,
0x959, 0x33, 

0x998, 0x08,
0x999, 0x33, 

0x9d8, 0x08,
0x9d9, 0x33, 

0xa18, 0x08,
0xa19, 0x33, 

0xa58, 0x08,
0xa59, 0x33, 

0xa98, 0x08,
0xa99, 0x33, 

0xad8, 0x08,
0xad9, 0x33, 

0xb18, 0x08,
0xb19, 0x33, 

0xb58, 0x08,
0xb59, 0x33, 

0xb98, 0x08,
0xb99, 0x33, 

0xbd8, 0x08,
0xbd9, 0x33, 

0xcc0, 0x40, //  MD IRQ - negative

//0x3f2, 0xff,	// PB manual, Field On
0x3fc, 0x0f,	// Embedded Field Select = 1

// Motion 
0x48a, 0x00, 		 //Base 
0x48b, 0x00,
0x48c, 0x00,
0x48d, 0x40,
0x48e, 0x38,
0x48f, 0x00,

0x49f, 0x00,	   // Outer boundary
0x4a0, 0x00,
0x4a1, 0x00,

0x4a2, 0x00,	   // Inner boundary and Cursor Color
0x4a3, 0xFF,
0x4a4, 0x00,

0x4a5, 0xff,	   // Mask Plane Color
0x4a6, 0x20,
0x4a7, 0xff,

0x4a8, 0xff,	   // Plane Color
0x4a9, 0x20,
0x4aa, 0xff,

// --- REC OSD -------
0xe00, 0xff,	  // REC OSD Enable
//0xe01, 0xff,
0xe01, 0x0,      // disable time
0xe02, 0xff,
0xe03, 0x0,      // disable status
0xe44, 0x0f,

0xc52, 0x01,
0xc42, 0x08,

0xcc8, 0xff,	// Backgroud enable all channel
0xcc9, 0xff,

0xe08, 0x00, //66,
0xe09, 0x5a,    // REC OSD Position
0xe0a, 0x00, 
0xe0a, 0x00,    
0xe0b, 0x58, 
0xe0c, 0x00, 
0xe0d, 0x30, 
0xe0e, 0x00, 
0xe0f, 0xd6, 
0xe10, 0x00, 
0xe11, 0xa4, //D1 size Time ROSD V-posi
0xe12, 0x01, 
0xe13, 0x70, 
0xe14, 0x00, 
0xe15, 0x0e, 
0xe16, 0x00, 
0xe17, 0x60, 
0xe18, 0x00, 
0xe19, 0x40, 
0xe1a, 0x03, 
0xe1b, 0x08, 
0xe1c, 0x00, 
0xe1d, 0x46, 
0xe1e, 0x00, 
0xe1f, 0x96, 
0xe20, 0x00, 
0xe21, 0x74, 
0xe22, 0x00, 
0xe23, 0x48, 
0xe24, 0x00, 
0xe25, 0x40, 
0xe26, 0x03, 
0xe27, 0x76, 
0xe28, 0x00, 
0xe29, 0x9e,//80, 
0xe2a, 0x00, 
0xe2b, 0xd6, 
0xe2c, 0x01, 
0xe2d, 0xff,
0xe2e, 0x80,
0xe2f, 0x80,
0xe30, 0x10,
0xe31, 0x80,
0xe32, 0x80,
0xe33, 0x10,
0xe34, 0x80,
0xe35, 0x80,
0xe36, 0x10,
0xe37, 0x80,
0xe38, 0x80,
0xe39, 0xeb,
0xe3a, 0x80,
0xe3b, 0x80,
0xe3c, 0xff,

0xe4c, 0xd9, //c9,//dc,
0xe4d, 0xe9,//00,
0xe4e, 0xff,
0xe4f, 0xff,

0x6b6, 0xe0,	// Must be used. + add detection no video

0x6b7, 0x40, //PB_channel_num
0x6b8, 0xc8, 
//0x6b9, 0x9f, // PB_chnum_en
0x6b9, 0x10, // PB_chnum_en off

0x224, 0x00, 		   // Select PB_CLKx, PB_DATx  (Not internal loopback)
0x225, 0x19, //~PB2 clk 0x11,
0x226, 0x2a, //~PB3 clk 0x22,
0x227, 0x3b, //~PB4 clk 0x33,	 

0x3c5, 0x04,	//Channel ID 2bit 

// REC FRC
0xc56, 0x40, //frsc_sel=0 (means rport5 as bias for FRSC_REC)

0xcbf, 0, //0x1e,	 // Change back to 0 by QUAD skipping problem -HR-02072012 ;REC FRC all ref_ports assign to rport5: enable, except rport5.
0xcc0, 0, //0x1f,  // Change back to 0 by QUAD skipping problem -HR-02072012 ;rport page free run en
0xc5c, 0x04,  // nonstd write disable
0xc58, 0x0f,	// No Video Reset

0xCCE, 0xff,  // Spot2REC buf ALTM
0xCCF, 0xff,
	
0xf1a, 0x00,	// Spot ENC Hactive delay 
0xf7a, 0x00,	
//0xf1f, 0xc8,  // Spot1 Enc BW ====> cause lost color, cannot use (HR-12132011)
//0xf7f, 0xc8,  // Spot2 Enc BW
0xf9b, 0x01,
// NonStd Live check for TW2960
0x372, 0xbf,
0x3f8, 0x30,
0x372, 0xdf,
0x3f8, 0x20,
0x372, 0xff,
0x3f8, 0x1f,

0x3DF, 0x60,
0x3E4, 0x60,
0x3E9, 0x60,
0x3EE, 0x60,
0x3B0, 0x60,

0x3DF, 0x00,
0x3E4, 0x00,
0x3E9, 0x00,
0x3EE, 0x00,
0x3B0, 0x00,


//REC NoVideo Mask
0xc52, 0x04,

//DDR Memory priority
0x108, 0x01, // Select SPOT 1 priority
0x10D, 0x00,
0x10C, 0x1F,
0x108, 0x02, // Select SPOT 2 priority
0x10D, 0x00,
0x10C, 0x1F,
0x108, 0x05, // Select RGB priority
0x10D, 0x00,
0x10C, 0x01,

//Mask Update at odd field
0xcbe, 0x1f,

0xffff, 0xff,	  //  End of Data

};

const char *InTypeStr[] =  {
"PAL_720",	//0
"NTSC_720", //1
"PAL_960",	//2
"NTSC_960", //3
"NTSC_4D1",	//4
"PAL_4D1", //5
"HD_1080I", //6
"HD_1080P", //7
"HD_720P",  //8
0
};

const char STR_NTSC[]	      = "NTSC";
const char STR_PAL[]	      = "PAL";
const char STR_1280x1024x60[] = "1280*1024/60";
const char STR_1280x1024x75[] = "1280*1024/75";
const char STR_HD1080P[]      = "HD1080p";
const char STR_HD1080P50[]    = "HD1080p50";
const char STR_1920x1200[]    = "1920*1200";
const char STR_1680x1050[]    = "1680*1050";
const char STR_640x480[]	= "640*480/60";
const char STR_640x480x72[]	= "640*480/72";
const char STR_HD1080PR[]      =  "HD1080p R";
const char STR_720P[]	       =  "720p";
const char STR_1920x1080[]    = "1920*1080";
const char STR_1080I[]        = "1080i";
const char STR_1080I50[]        = "1080i50";
const char STR_1600x900[]        = "1600*900";
const char STR_720P50[]	       =  "720p50";
const char STR_1024x768[]          =  "1024*768";


const _mTimingType mTimingTbl[] = {
#ifdef SUPPORT_SXGA60
//0 1280x1024 60Hz
{ 108000, 1688, 1066, 1280, 1024, 48, 1, 112, 3, HPOSITIVE|VPOSITIVE, (u8 *)STR_1280x1024x60, 4, 15 }, 
#endif

#ifdef SUPPORT_HD1080P
//2 1920x1080p
{ 148500, 2200, 1125, 1920, 1080, 88, 0, 44, 5, HPOSITIVE|VPOSITIVE, (u8 *)STR_HD1080P, 16, 0  },
#endif

#ifdef SUPPORT_1920x1200
//3 1920x1200
{ 193500, 2592, 1245, 1920, 1200, 136, 3, 200, 6, HPOSITIVE|VNEGATIVE, (u8 *)STR_1920x1200, 16, 15 },
#endif

#ifdef SUPPORT_1680x1050
//4 1680x1050 
{ 146570, 2240, 1089, 1680, 1050, 104, 3, 176, 6,  HNEGATIVE|VPOSITIVE, (u8 *)STR_1680x1050, 16, 15 },  
#endif

#ifdef SUPPORT_720P
//9 1280x720 60Hz
{ 74250, 1650, 750, 1280, 720, 110, 0, 40, 5, HPOSITIVE|VPOSITIVE, (u8 *)STR_720P, 4, 0 },
#endif

#ifdef SUPPORT_1080I
//b 1080i 
{ 74250, 2200, 562, 1920, 540, 88, 2, 44, 5, HNEGATIVE|VNEGATIVE|INTERLACE, (u8 *)STR_1080I, 5, 15 },
//{ 77625, 2200, 562, 1920, 540, 88, 2, 44, 5, HNEGATIVE|VNEGATIVE|INTERLACE, (u8 *)STR_1080I, 5, 15 },
#endif

#ifdef SUPPORT_1600x900
//b 1600x900
{ 101250, 1720, 976, 1600, 900, 44, 5, 10, 3,  HPOSITIVE|VNEGATIVE, (u8 *)STR_1600x900, 4, 15 },
#endif

#ifdef SUPPORT_HD1080P50
//c 1920x1080p50
{ 148500, 2640, 1125, 1920, 1080, 528, 4, 44, 5, HPOSITIVE|VPOSITIVE, (u8 *)STR_HD1080P50, 31, 15 },
#endif

#ifdef SUPPORT_SXGA50
//0 1280x1024 50Hz
{ 99000, 1878, 1054, 1280, 1024, 72, 3, 136, 7, HNEGATIVE|VPOSITIVE, (u8 *)STR_1280x1024x50, 4, 15 }, 
#endif

#ifdef SUPPORT_1080I50
//b 1080i 50
{ 74250, 2640, 562, 1920, 540, 484, 2, 44, 5, HNEGATIVE|VNEGATIVE|INTERLACE, (u8 *)STR_1080I50, 20, 15 },
#endif

#ifdef SUPPORT_640x480x60
// 640x480p, 60Hz CEA-861
{ 25310, 800, 525, 640, 480, 16, 9, 96, 2, HNEGATIVE|VNEGATIVE, (u8 *)STR_640x480p, 1, 15 }, 
#endif

#ifdef SUPPORT_480P
	// 720x480p, 60Hz CEA-861, HDMI certification
{ 27000, 858, 516, 720, 480, 16, 0, 62, 6, HNEGATIVE|VNEGATIVE, (u8 *)STR_720x480p, 2, 0 },
#endif

#ifdef SUPPORT_576P
	// 720x576p, 50Hz CEA-861
{ 27000, 864, 625, 720, 576, 12, 5, 64, 5, HNEGATIVE|VNEGATIVE, (u8 *)STR_720x576p, 17, 15 }, 	
#endif

#ifdef SUPPORT_1600x1200x60	
	// 1600x1200p, 60Hz VESA CVB
{ 162000, 2160, 1250, 1600, 1200, 64, 1, 192, 3, HPOSITIVE|VPOSITIVE, (u8 *)STR_1600x1200p60, 0, 15 }, 
#endif

#ifdef SUPPORT_480I
{ 13500, 858, 262, 720, 240, 16, 3, 32, 3, HNEGATIVE|VNEGATIVE|INTERLACE, (u8 *)STR_480I, 2, 0 }, 
#endif

#ifdef SUPPORT_720P50
// 1280x720 50Hz
{ 74250, 1980, 750, 1280, 720, 110, 0, 40, 5, HPOSITIVE|VPOSITIVE, (u8 *)STR_720P50, 4, 0 },
#endif

#ifdef SUPPORT_720P50
// 1280x720 50Hz
{ 74250, 1980, 750, 1280, 720, 110, 0, 40, 5, HPOSITIVE|VPOSITIVE, (u8 *)STR_720P50, 4, 0 },
#endif

#ifdef SUPPORT_1024x768
{ 67500, 1344, 806, 1024, 768, 24, 3, 136, 6,  HNEGATIVE| VNEGATIVE, (u8 *)STR_1024x768, 4, 0 },
//{ 64800, 1344, 806, 1024, 768, 24, 3, 136, 6,  HNEGATIVE| VNEGATIVE, (u8 *)STR_1024x768, 4, 0 },
#endif

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} // End of Data.

};

#endif //_TW2828_TBL_H_

