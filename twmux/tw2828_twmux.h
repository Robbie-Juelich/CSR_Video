
#ifndef _TWMUX_H_
#define _TWMUX_H_

#include "tw2828_config.h"

#include "tw2828_hostio.h"
#include "tw2828_twmux.h"

#define SUPPORT_SXGA60
#define SUPPORT_HD1080P
#define SUPPORT_1920x1200
#define SUPPORT_1680x1050
#define SUPPORT_720P
#define SUPPORT_1080I
#define SUPPORT_1080I50
#define SUPPORT_1600x900
#define SUPPORT_HD1080P50
#define SUPPORT_720P50
#define SUPPORT_1024x768


#define NTSC	1
#define PAL   0

#define MAINON			0x80
#define AUTOMODEON	0x40
#define MDBOXON			0x20
#define INSERTCHID	0x10

#define HPOSITIVE 0x02
#define HNEGATIVE 0x00
#define VPOSITIVE 0x01
#define VNEGATIVE 0x00
#define INTERLACE 0x10
#define VGA_ON	  0x04
#define PAL4DM	  0x08

// Channel ID Enc control
#define ENC_EVEN_EN			0x01
#define ENC_RUN_IN_CLK_EN	0x02
#define ENC_DIGITAL_EN		0x04
#define ENC_USR_EN			0x08
#define ENC_ANALOG_EN		0x10
#define ENC_RPT_EN			0x20
#define ENC_AUTO_EN			0x40
#define ENC_ODD_EN			0x80

// Display memory upscaler source window maxium size
//#define MHR_UPS			1280
//#define MVR_UPS			1024
#define MHR_UPS			1920  //test
#define MVR_UPS			1080

#define IHR_704			704
#define IHR_720			720
#define IHR_960			960
#define IVR_NTSC		480
#define IVR_PAL			576
#define IVRI_NTSC		240
#define IVRI_PAL		288
#define IHR_HD			1920
#define IVR_HD			960

#define IHT_NTSC		858 // NTSC Htotal
#define IHT_PAL			864 // PAL  Htotal
#define IVT_NTSC		262
#define IVT_PAL			312

#define TW2828_MMAP_TOTAL_SIZE		(1920*1080*2) 


//typedef
typedef int HResult;

//! _TWMuxDevID
typedef enum {
	TW2828_0, TW2828_1,	/*TW2828_2, TW2828_3,	*/	TW2828_MAX,
} _TWMuxDevID;

enum {
MseCursor_arrow, MseCursor_beam_i, MseCursor_busy_i, MseCursor_busy_ii,
MseCursor_cross_i, MseCursor_cross_r, MseCursor_help_i, MseCursor_hold,
MseCursor_im, MseCursor_imove, MseCursor_move_m, MseCursor_no_im,
MseCursor_point, MseCursor_size2_im, MseCursor_size3_im, MseCursor_size4_im	
};


//! _IN_Type
typedef enum {
// 8bit Byte Interleave for Live IN, 8bit Frame Interleave for Playback.
PAL_720, NTSC_720, 
PAL_960,	NTSC_960, //WD1
// below 16bit Frame Interleave for Playback
NTSC_4D1, //1440x480 (I) 
PAL_4D1,  //1440x576 (I)
HD_1080I, HD_1080P, HD_720P, 
SD_480I, SD_576I,
SD_480P, SD_576P,
VGA_640x480, VGA_1024x768, VGA_1280x768,
NOVIDEO,
MAX_PB_IN,
} _IN_Type;

//! _DIType
typedef enum { DI_OFF, DI_2D, DI_3D,	DI_WEAVE } _DIType;

//! _ChannelNo
typedef enum {
// Normal Mode	
 CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16,
 PB1, PB2, PB3, PB4, PB5,
// Auto Mode
 CHx1, CHx2, CHx3, CHx4, CHx5, CHx6, CHx7, CHx8, CHx9, CHx10, CHx11, CHx12, CHx13, CHx14, CHx15, CHx16, // Live channel in PB Auto Mode
 PBx1, PBx2, PBx3, PBx4, PBx5, PBx6, PBx7, PBx8, PBx9, PBx10, PBx11, PBx12, PBx13, PBx14, PBx15, PBx16,
// Cascade channel
 CAS1, CAS2, CAS3, CAS4,
} _ChannelNo;	

//! _InputPort
typedef enum {
	PORT_LIVE_0, PORT_LIVE_1, PORT_LIVE_2, PORT_LIVE_3, PORT_LIVE_4, PORT_LIVE_5, PORT_LIVE_6,
	PORT_LIVE_7, PORT_LIVE_8, PORT_LIVE_9, PORT_LIVE_10, PORT_LIVE_11, PORT_LIVE_12, PORT_LIVE_13,
	PORT_LIVE_14, PORT_LIVE_15, PORT_PB_1, PORT_PB_2, PORT_PB_3, PORT_PB_4, PORT_PB_5, MAX_PORT,
} _InputPort;

//! enumerate mdrs
typedef enum{
#ifdef SUPPORT_SXGA60
rSXGA60,
#endif
#ifdef SUPPORT_HD1080P
rHD1080P,
#endif
#ifdef SUPPORT_1920x1200
r1920x1200,
#endif
#ifdef SUPPORT_1680x1050
r1680x1050,
#endif
#ifdef SUPPORT_720P
r720P,
#endif
#ifdef SUPPORT_1080I
r1080I,
#endif
#ifdef SUPPORT_1600x900
r1600x900,
#endif
#ifdef SUPPORT_HD1080P50
rHD1080P50,
#endif
#ifdef SUPPORT_1080I50
r1080I50,
#endif
#ifdef SUPPORT_640x480x60
r640x480x60,
#endif
#ifdef SUPPORT_480P
r480P,
#endif
#ifdef SUPPORT_576P
r576P,
#endif
#ifdef SUPPORT_1600x1200x60
r1600x1200x60,
#endif
#ifdef SUPPORT_480I
r480I,
#endif
#ifdef SUPPORT_720P50
r720P50,
#endif
#ifdef SUPPORT_1024x768
r1024x768,
#endif


#ifndef _TWMUX_H_
#define _TWMUX_H_

#include "tw2828_config.h"

#include "tw2828_hostio.h"
#include "tw2828_twmux.h"

#define SUPPORT_SXGA60
#define SUPPORT_HD1080P
#define SUPPORT_1920x1200
#define SUPPORT_1680x1050
#define SUPPORT_720P
#define SUPPORT_1080I
#define SUPPORT_1080I50
#define SUPPORT_1600x900
#define SUPPORT_HD1080P50
#define SUPPORT_720P50



#define NTSC	1
#define PAL   0

#define MAINON			0x80
#define AUTOMODEON	0x40
#define MDBOXON			0x20
#define INSERTCHID	0x10

#define HPOSITIVE 0x02
#define HNEGATIVE 0x00
#define VPOSITIVE 0x01
#define VNEGATIVE 0x00
#define INTERLACE 0x10
#define VGA_ON	  0x04
#define PAL4DM	  0x08

// Channel ID Enc control
#define ENC_EVEN_EN			0x01
#define ENC_RUN_IN_CLK_EN	0x02
#define ENC_DIGITAL_EN		0x04
#define ENC_USR_EN			0x08
#define ENC_ANALOG_EN		0x10
#define ENC_RPT_EN			0x20
#define ENC_AUTO_EN			0x40
#define ENC_ODD_EN			0x80

// Display memory upscaler source window maxium size
//#define MHR_UPS			1280
//#define MVR_UPS			1024
#define MHR_UPS			1920  //test
#define MVR_UPS			1080

#define IHR_704			704
#define IHR_720			720
#define IHR_960			960
#define IVR_NTSC		480
#define IVR_PAL			576
#define IVRI_NTSC		240
#define IVRI_PAL		288
#define IHR_HD			1920
#define IVR_HD			960

#define IHT_NTSC		858 // NTSC Htotal
#define IHT_PAL			864 // PAL  Htotal
#define IVT_NTSC		262
#define IVT_PAL			312

#define TW2828_MMAP_TOTAL_SIZE		(1920*1080*2) 


//typedef
typedef int HResult;

//! _TWMuxDevID
typedef enum {
	TW2828_0, TW2828_1,	/*TW2828_2, TW2828_3,	*/	TW2828_MAX,
} _TWMuxDevID;

enum {
MseCursor_arrow, MseCursor_beam_i, MseCursor_busy_i, MseCursor_busy_ii,
MseCursor_cross_i, MseCursor_cross_r, MseCursor_help_i, MseCursor_hold,
MseCursor_im, MseCursor_imove, MseCursor_move_m, MseCursor_no_im,
MseCursor_point, MseCursor_size2_im, MseCursor_size3_im, MseCursor_size4_im	
};


//! _IN_Type
typedef enum {
// 8bit Byte Interleave for Live IN, 8bit Frame Interleave for Playback.
PAL_720, NTSC_720, 
PAL_960,	NTSC_960, //WD1
// below 16bit Frame Interleave for Playback
NTSC_4D1, //1440x480 (I) 
PAL_4D1,  //1440x576 (I)
HD_1080I, HD_1080P, HD_720P, 
SD_480I, SD_576I,
SD_480P, SD_576P,
VGA_640x480, VGA_1024x768, VGA_1280x768,
NOVIDEO,
MAX_PB_IN,
} _IN_Type;

//! _DIType
typedef enum { DI_OFF, DI_2D, DI_3D,	DI_WEAVE } _DIType;

//! _ChannelNo
typedef enum {
// Normal Mode	
 CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16,
 PB1, PB2, PB3, PB4, PB5,
// Auto Mode
 CHx1, CHx2, CHx3, CHx4, CHx5, CHx6, CHx7, CHx8, CHx9, CHx10, CHx11, CHx12, CHx13, CHx14, CHx15, CHx16, // Live channel in PB Auto Mode
 PBx1, PBx2, PBx3, PBx4, PBx5, PBx6, PBx7, PBx8, PBx9, PBx10, PBx11, PBx12, PBx13, PBx14, PBx15, PBx16,
// Cascade channel
 CAS1, CAS2, CAS3, CAS4,
} _ChannelNo;	

//! _InputPort
typedef enum {
	PORT_LIVE_0, PORT_LIVE_1, PORT_LIVE_2, PORT_LIVE_3, PORT_LIVE_4, PORT_LIVE_5, PORT_LIVE_6,
	PORT_LIVE_7, PORT_LIVE_8, PORT_LIVE_9, PORT_LIVE_10, PORT_LIVE_11, PORT_LIVE_12, PORT_LIVE_13,
	PORT_LIVE_14, PORT_LIVE_15, PORT_PB_1, PORT_PB_2, PORT_PB_3, PORT_PB_4, PORT_PB_5, MAX_PORT,
} _InputPort;

//! enumerate mdrs
typedef enum{
#ifdef SUPPORT_SXGA60
rSXGA60,
#endif
#ifdef SUPPORT_HD1080P
rHD1080P,
#endif
#ifdef SUPPORT_1920x1200
r1920x1200,
#endif
#ifdef SUPPORT_1680x1050
r1680x1050,
#endif
#ifdef SUPPORT_720P
r720P,
#endif
#ifdef SUPPORT_1080I
r1080I,
#endif
#ifdef SUPPORT_1600x900
r1600x900,
#endif
#ifdef SUPPORT_HD1080P50
rHD1080P50,
#endif
#ifdef SUPPORT_1080I50
r1080I50,
#endif
#ifdef SUPPORT_640x480x60
r640x480x60,
#endif
#ifdef SUPPORT_480P
r480P,
#endif
#ifdef SUPPORT_576P
r576P,
#endif
#ifdef SUPPORT_1600x1200x60
r1600x1200x60,
#endif
#ifdef SUPPORT_480I
r480I,
#endif
#ifdef SUPPORT_720P50
r720P50,
#endif
#ifdef SUPPORT_1024x768
r1024x768,
#endif

rMAX_MDRS,
rERR_MDRS = 0xff,
} _Mdrs;

enum { 
R_1,R_2,R_3,R_4,R_5,R_6,R_7,R_8,  
R_PB1,R_PB2,R_PB3,R_PB4,R_PB5,  // Use HD allocated into PB
R_14,R_15,R_16,// Don't use  
S_1=0x10,S_2,S_3,S_4,S_5,S_6,S_7,S_8,S_9,S_10,S_11,S_12,S_13,S_14,S_15,S_16,  // Support 16QCIF
NOBUF=0xff,};

enum { V_0, V_144_120, V_288_240, V_432_360, V_576, V_720_696, V_864_816, V_540 };
enum { SPT_V_0, SPT_V_144_120, SPT_V_288_240, SPT_V_432_360, SPT_V_576_480, \
       SPT_V_720_600, SPT_V_1152_960, SPT_V_1296_1272 };
enum { BUF_FRAME, BUF_FIELD, BUF_LINE, BUF_u8	};
//BUF_LINE= Not support
//BUF_u8= Not support
enum { BUF_D1, BUF_HALFD1, BUF_CIF, BUF_VGA };
enum { H_0, H_360, H_720, H_1080, H_1440, H_1880, H_2160, H_2520, H_640,  H_1280 };
//-----------------
// Record Port 
// CIF should always be progressive(Frame Interleave).
enum { PORT5, PORT6, PORT7, PORT8, PORT9 };
enum { D1, CIF,	D1_4, BT1120, HALFD1, QUARD, CH_9, QCIF };
enum { RCLK_27M, RCLK_54M, RCLK_108M };
enum { FRAME, FIELD, IFRAME, FIELDSW };
enum { PORT_8BIT, PORT_16BIT };
//IFRAME=  Interlaced Frame Interleave
//FIELDSW= New Field Switch
//-------------------------------
// Record Pin Control
enum { REC_PIN_1, REC_PIN_2, REC_PIN_3, REC_PIN_4 };
enum { CLK_NORMAL, CLK_INVERT};
enum { PIN_8BIT, PIN_16BIT, PIN_16BIT_BT1120 };
enum { CLK_27M, CLK_54M, CLK_108M, CLK_74M };
enum { ALL_D1_FIELD, ALL_D1_FRAME, ALL_CIF, ALL_HALFD1_FRAME };
enum { RECP1_7_0, RECP1_15_8, RECP2_7_0, RECP2_15_8, RECP3_7_0, RECP3_15_8, \
       RECP4_7_0, RECP4_15_8, RECP5_7_0, RECP5_15_8, RECP6_7_0, RECP6_15_8, RECP7_7_0, \
       RECP7_15_8, RECP8_7_0, RECP8_15_8 };
enum { SPOT1, SPOT2 };

//! _In_Port_Info
typedef struct In_Port_Info {
	_InputPort p;
	_IN_Type intype;
	int IHR, IVR;
	int Interlaced;
	int EmChId;         //channel id embedded
	int YCSwap;
} _In_Port_Info;

//! _mTimingType
typedef struct mTimingType {
	u32 vclk;
	u16 htt, vtt; // Htotal-1, VTotal-1
	u16 hde, vde; // HActive, VActive
	u16 hfp, vfp; // frontporch
	u8  hs, vs;   // Syncwidth
	u8  ctrl;	    // bit0:Vsync, bit1:Hsync, bit2:interlace/VGA
	u8  *str;
	u8  vid; 	    //video ID for HDMI avi info
	u8  vsdelay;
} _mTimingType;


//! _ups
typedef struct ups {
	_DIType di;
	int    ups_on;
	int			sx, sy;//, sw, sh;  // Upscale  source window info
	      	                 //! Note: Upscale  target window info should be same as display screen
} _ups;

//! _ChannelInfo
typedef struct ChannelInfo {
	u8  chno; //VO window number
	u8  ch;   //VI channel number, in DNS module.
	u8	port;
	u16 tw,th;	// PB target image size after downscaler
	u16 cx, cy, cx_unit, cy_unit; // add for cropping start x, y
	u16 ix,iy,iw,ih; // input image  // after downscaler
	u16 x,y,w,h;  // after scale; x,w: /4
	u8  ctrl;	
		//bit7  On
		//bit6  DUALON
		//bit5  MDBOXON
	  //u8  level;	
		//bit4-0 MD LEVEL
		//bit6-5 MD SUBCELL LEVEL
} _ChannelInfo;


typedef struct LogoInfo {
	int x;
	int y;
	int w;
	int h;
	unsigned char* addr;
} _LogoInfo;


//! _DrawOsd
typedef struct DrawOsd {
  u8 *Str;
	u16	addr;
} _DrawOsd;

//! _OSD_HEADER
typedef struct OSD_HEADER {
	u8  *osdfont;  
  u16 *osdpic;                
	u8 *colortb;
} _OSD_HEADER;

typedef struct RecSpotBuf {
	u8  bf;
	u8  en;	
	u8  chno;
	u8	res;
	u8  format;
	u8  hposi;
	u8  vposi;
	u8  qcif_en;
	u8  ch9_en;
} _RecSpotBuf;


typedef struct RecPortInfo {
	u8  en;	
	u8	 res;
	u8  format;
	u8  clk;
	u8	 databit;
	u8	 qmax;
	//const u8  *que;
	u8  *que;
} _RecPortInfo;

typedef struct twSpot_port {
	u32   rp;
	u8  en;	// rport_en, 9CH_OSD_en, QCIF_en
	u8	 res;
	u8  format;
	u8  clk;
	u8	 databit;
	u8	 qmax;
	const u8  *que;
	u32   sp;
} _twSpot_port;

typedef struct RecPinInfo {
	u8   clkinvert;
	u8	 databit;
  u8	 clk;
	u8	 dataport;
} _RecPinInfo;

typedef struct RecAllInfo {
	const _RecSpotBuf  *buf; // Record/SPOT 32 Buf
	const _RecPortInfo *port;
	const _RecPinInfo  *pin;
} _RecAllInfo;

typedef struct ChIdEnc
{
	u32 ctrl;		// Control
	u32 vos;		// vertical data start(odd field)
	u32 fos;		// vertical data start offset(even field) 
	u32 hos;		// Horizontal data start offset
//	u32 pixel_w;	// one bit data unit (pixel) for Analog channel ID
	u32 min_val, max_val;	// min & max value for Analog channel ID

} _ChIdEnc;


//! _twmux
typedef struct twmux {
	int      id;
	
	unsigned char ctrl;
	_IN_Type ilvtype;
	_IN_Type ipbtype[5];

	_In_Port_Info ilvp; 
	_In_Port_Info ipbp[5]; 
	
	unsigned char mdrs;
  int      PHR, PVR;					 // display target size 	  
  int      UHR, UVR;					 // upscale source size
  int     INTERLACED;     
  int			ZoonScreenOnFlag;                  
	_ups 		 dsp_ups;
	
  _ChannelInfo *pChannelDB;
	
}_twmux;

extern const char *InTypeStr[];
extern const _mTimingType mTimingTbl[];
extern _twmux tw2828devs[];

HResult TWMux_Init(int no, _twmux Cfg);
void TWMux_MakeFullScreen(int no, unsigned char winno);
HResult TWMux_SetMainDisplayResolution(int no, _Mdrs mdrs);
_Mdrs TWMux_GetMainDisplayResolution(int no);
void TWMux_GetMainDisplayInfo(int no, int *phr, int *pvr, int *uhr, int *uvr);
void TWMux_MakeMainChannelWindows( u8 id, _ChannelInfo *C);
void TWMux_SetMainChannel(u8 id, _ChannelInfo *CInfo);
void TWMux_SetLiveSource(u8 no, _IN_Type mode);
_IN_Type TWMux_GetLiveSource(u8 id);
void TWMux_SetPBSource(u8 no, int pb, _IN_Type mode, int EmChId, int YCSwap);
void TWMux_SetPBInType(u8 no, int pb, _IN_Type mode);
_IN_Type TWMux_GetPBInType(u8 id, int pb);
void TWMux_SetPBLoopback(u8 id, int on);
void TWMux_SetPBYCSwap(u8 id, int pb, int swap);
void TWMux_SetYUVImageToLogoWindow( int no, int x, int y, int w, int h, u32 addr);
void TWMux_SetSingleBoxWindowOn(u8 id, u8 no, u8 r, u8 g, u8 b, int bufno);
void TWMux_DisplayOsd_LoadOSDFontData(u8 no, _OSD_HEADER ptr);
void TWMux_DisplayOsd_LoadOSDDispData(u8 no, _DrawOsd *ptrOsd);
void TWMux_RecOsd_LoadOSDFontData(u8 no, _OSD_HEADER ptr);
void TWMux_RecOsd_LoadOSDDispData(u8 no, _DrawOsd *ptrOsd);
void TWMux_SetFreezeChannelMain(u8 id, u8 pbmode, u8 a, u8 on);
void TWMux_SetFreezeChannelRec(u8 id, u8, u8);
void TWMux_SetFreezeChannelSpot(u8 id, u8, u8);
int TWMux_ChangeMclk(u8 id, u32 mclk);
int TWMux_GetMclk(u8 id);
void TWMux_LoadRecBuf(u8 id, u8 no, _RecSpotBuf r);
void TWMux_LoadSpotBuf(u8 host, u8 no, _RecSpotBuf s);
void TWMux_SetRecPortForSpot(u8 id, _twSpot_port p);
int TWMux_SetRecPortTable_Update(u8 id, u8 p, u8 *buf, u8 inx_st, u8 inx_ed);
void TWMux_SetRecordChannelIDEnc(u8 id, u8 p, _ChIdEnc chenc);
void TWMux_LoadRecordDataAll(u8 id, _RecAllInfo tb);
void TWMux_SetZoomScreen(u8 id, u16 x, u16 y, u16 w, u16 h);
void TWMux_SetZoomScreenOff(u8 id);
void TWMux_WriteDisplayToMemory(u32 sAddr, u32 x, u32 y);
void TWMux_InitMotionBox(u8 id);
void TWMux_AllMotionBoxOff(u8 id);
u32 TWMux_ReadNVStatus(u8 id);
void TWMux_MotionBox_OneChannel(u8 id, _ChannelNo i, int forced_off);
void TWMux_ChannelWindowOff(u8 id, _ChannelNo ch);
void TWMux_ChannelWindowOn(u8 id, _ChannelNo ch);
void TWMux_GetChannelDB(u8 id, u8 bufno, _ChannelInfo *Channeldb);
void TWMux_ResetDisplay(u8 id);
void TWMux_printChannelDB(u8 id);
void TWMux_ChannelWindowForcedOff(u8 id, _ChannelNo ch);
void TWMux_SelectDecClock(u8 no, u8 flg960);
void TWMux_SetVIChno(u8 no, u32 winno, u32 ch);
void TWMux_DisplayOsd_OnOff(u8 no, u8 on);
void TWMux_BT1120_OnOff(int no, u8 on);
void TWMux_Display_Boundary_OnOff(int no, u8 on);


void DCM_GateCalibration(void);
int DCM_EyeCalibration(void);
void ReadMemory(u32 addr, u8 length);
void WriteMemory(u32 addr, u8 *ptr, u8 length);
void WriteNDStatus(u32 dat);
void WriteBDStatus(u32 dat);
void WriteNVStatus(u32 dat);
void WriteMDStatus(u32 dat);
u32 ReadNDStatus(void);
u32 ReadBDStatus(void);
u32 ReadMDStatus(void);
void SpotDacOnOff(u8 on);


#endif  //_TWMUXAPI_H_
rMAX_MDRS,
rERR_MDRS = 0xff,
} _Mdrs;

enum { 
R_1,R_2,R_3,R_4,R_5,R_6,R_7,R_8,  
R_PB1,R_PB2,R_PB3,R_PB4,R_PB5,  // Use HD allocated into PB
R_14,R_15,R_16,// Don't use  
S_1=0x10,S_2,S_3,S_4,S_5,S_6,S_7,S_8,S_9,S_10,S_11,S_12,S_13,S_14,S_15,S_16,  // Support 16QCIF
NOBUF=0xff,};

enum { V_0, V_144_120, V_288_240, V_432_360, V_576, V_720_696, V_864_816, V_540 };
enum { SPT_V_0, SPT_V_144_120, SPT_V_288_240, SPT_V_432_360, SPT_V_576_480, \
       SPT_V_720_600, SPT_V_1152_960, SPT_V_1296_1272 };
enum { BUF_FRAME, BUF_FIELD, BUF_LINE, BUF_u8	};
//BUF_LINE= Not support
//BUF_u8= Not support
enum { BUF_D1, BUF_HALFD1, BUF_CIF, BUF_VGA };
enum { H_0, H_360, H_720, H_1080, H_1440, H_1880, H_2160, H_2520, H_640,  H_1280 };
//-----------------
// Record Port 
// CIF should always be progressive(Frame Interleave).
enum { PORT5, PORT6, PORT7, PORT8, PORT9 };
enum { D1, CIF,	D1_4, BT1120, HALFD1, QUARD, CH_9, QCIF };
enum { RCLK_27M, RCLK_54M, RCLK_108M };
enum { FRAME, FIELD, IFRAME, FIELDSW };
enum { PORT_8BIT, PORT_16BIT };
//IFRAME=  Interlaced Frame Interleave
//FIELDSW= New Field Switch
//-------------------------------
// Record Pin Control
enum { REC_PIN_1, REC_PIN_2, REC_PIN_3, REC_PIN_4 };
enum { CLK_NORMAL, CLK_INVERT};
enum { PIN_8BIT, PIN_16BIT, PIN_16BIT_BT1120 };
enum { CLK_27M, CLK_54M, CLK_108M, CLK_74M };
enum { ALL_D1_FIELD, ALL_D1_FRAME, ALL_CIF, ALL_HALFD1_FRAME };
enum { RECP1_7_0, RECP1_15_8, RECP2_7_0, RECP2_15_8, RECP3_7_0, RECP3_15_8, \
       RECP4_7_0, RECP4_15_8, RECP5_7_0, RECP5_15_8, RECP6_7_0, RECP6_15_8, RECP7_7_0, \
       RECP7_15_8, RECP8_7_0, RECP8_15_8 };
enum { SPOT1, SPOT2 };

//! _In_Port_Info
typedef struct In_Port_Info {
	_InputPort p;
	_IN_Type intype;
	int IHR, IVR;
	int Interlaced;
	int EmChId;         //channel id embedded
	int YCSwap;
} _In_Port_Info;

//! _mTimingType
typedef struct mTimingType {
	u32 vclk;
	u16 htt, vtt; // Htotal-1, VTotal-1
	u16 hde, vde; // HActive, VActive
	u16 hfp, vfp; // frontporch
	u8  hs, vs;   // Syncwidth
	u8  ctrl;	    // bit0:Vsync, bit1:Hsync, bit2:interlace/VGA
	u8  *str;
	u8  vid; 	    //video ID for HDMI avi info
	u8  vsdelay;
} _mTimingType;


//! _ups
typedef struct ups {
	_DIType di;
	int    ups_on;
	int			sx, sy;//, sw, sh;  // Upscale  source window info
	      	                 //! Note: Upscale  target window info should be same as display screen
} _ups;

//! _ChannelInfo
typedef struct ChannelInfo {
	u8  chno; //VO window number
	u8  ch;   //VI channel number, in DNS module.
	u8	port;
	u16 tw,th;	// PB target image size after downscaler
	u16 cx, cy, cx_unit, cy_unit; // add for cropping start x, y
	u16 ix,iy,iw,ih; // input image  // after downscaler
	u16 x,y,w,h;  // after scale; x,w: /4
	u8  ctrl;	
		//bit7  On
		//bit6  DUALON
		//bit5  MDBOXON
	  //u8  level;	
		//bit4-0 MD LEVEL
		//bit6-5 MD SUBCELL LEVEL
} _ChannelInfo;


typedef struct LogoInfo {
	int x;
	int y;
	int w;
	int h;
	unsigned char* addr;
} _LogoInfo;


//! _DrawOsd
typedef struct DrawOsd {
  u8 *Str;
	u16	addr;
} _DrawOsd;

//! _OSD_HEADER
typedef struct OSD_HEADER {
	u8  *osdfont;  
  u16 *osdpic;                
	u8 *colortb;
} _OSD_HEADER;

typedef struct RecSpotBuf {
	u8  bf;
	u8  en;	
	u8  chno;
	u8	res;
	u8  format;
	u8  hposi;
	u8  vposi;
	u8  qcif_en;
	u8  ch9_en;
} _RecSpotBuf;


typedef struct RecPortInfo {
	u8  en;	
	u8	 res;
	u8  format;
	u8  clk;
	u8	 databit;
	u8	 qmax;
	//const u8  *que;
	u8  *que;
} _RecPortInfo;

typedef struct twSpot_port {
	u32   rp;
	u8  en;	// rport_en, 9CH_OSD_en, QCIF_en
	u8	 res;
	u8  format;
	u8  clk;
	u8	 databit;
	u8	 qmax;
	const u8  *que;
	u32   sp;
} _twSpot_port;

typedef struct RecPinInfo {
	u8   clkinvert;
	u8	 databit;
  u8	 clk;
	u8	 dataport;
} _RecPinInfo;

typedef struct RecAllInfo {
	const _RecSpotBuf  *buf; // Record/SPOT 32 Buf
	const _RecPortInfo *port;
	const _RecPinInfo  *pin;
} _RecAllInfo;

typedef struct ChIdEnc
{
	u32 ctrl;		// Control
	u32 vos;		// vertical data start(odd field)
	u32 fos;		// vertical data start offset(even field) 
	u32 hos;		// Horizontal data start offset
//	u32 pixel_w;	// one bit data unit (pixel) for Analog channel ID
	u32 min_val, max_val;	// min & max value for Analog channel ID

} _ChIdEnc;


//! _twmux
typedef struct twmux {
	int      id;
	
	unsigned char ctrl;
	_IN_Type ilvtype;
	_IN_Type ipbtype[5];

	_In_Port_Info ilvp; 
	_In_Port_Info ipbp[5]; 
	
	unsigned char mdrs;
  int      PHR, PVR;					 // display target size 	  
  int      UHR, UVR;					 // upscale source size
  int     INTERLACED;     
  int			ZoonScreenOnFlag;                  
	_ups 		 dsp_ups;
	
  _ChannelInfo *pChannelDB;
	
}_twmux;

extern const char *InTypeStr[];
extern const _mTimingType mTimingTbl[];
extern _twmux tw2828devs[];

HResult TWMux_Init(int no, _twmux Cfg);
void TWMux_MakeFullScreen(int no, unsigned char winno);
HResult TWMux_SetMainDisplayResolution(int no, _Mdrs mdrs);
_Mdrs TWMux_GetMainDisplayResolution(int no);
void TWMux_GetMainDisplayInfo(int no, int *phr, int *pvr, int *uhr, int *uvr);
void TWMux_MakeMainChannelWindows( u8 id, _ChannelInfo *C);
void TWMux_SetMainChannel(u8 id, _ChannelInfo *CInfo);
void TWMux_SetLiveSource(u8 no, _IN_Type mode);
_IN_Type TWMux_GetLiveSource(u8 id);
void TWMux_SetPBSource(u8 no, int pb, _IN_Type mode, int EmChId, int YCSwap);
void TWMux_SetPBInType(u8 no, int pb, _IN_Type mode);
_IN_Type TWMux_GetPBInType(u8 id, int pb);
void TWMux_SetPBLoopback(u8 id, int on);
void TWMux_SetPBYCSwap(u8 id, int pb, int swap);
void TWMux_SetYUVImageToLogoWindow( int no, int x, int y, int w, int h, u32 addr);
void TWMux_SetSingleBoxWindowOn(u8 id, u8 no, u8 r, u8 g, u8 b, int bufno);
void TWMux_DisplayOsd_LoadOSDFontData(u8 no, _OSD_HEADER ptr);
void TWMux_DisplayOsd_LoadOSDDispData(u8 no, _DrawOsd *ptrOsd);
void TWMux_RecOsd_LoadOSDFontData(u8 no, _OSD_HEADER ptr);
void TWMux_RecOsd_LoadOSDDispData(u8 no, _DrawOsd *ptrOsd);
void TWMux_SetFreezeChannelMain(u8 id, u8 pbmode, u8 a, u8 on);
void TWMux_SetFreezeChannelRec(u8 id, u8, u8);
void TWMux_SetFreezeChannelSpot(u8 id, u8, u8);
int TWMux_ChangeMclk(u8 id, u32 mclk);
int TWMux_GetMclk(u8 id);
void TWMux_LoadRecBuf(u8 id, u8 no, _RecSpotBuf r);
void TWMux_LoadSpotBuf(u8 host, u8 no, _RecSpotBuf s);
void TWMux_SetRecPortForSpot(u8 id, _twSpot_port p);
int TWMux_SetRecPortTable_Update(u8 id, u8 p, u8 *buf, u8 inx_st, u8 inx_ed);
void TWMux_SetRecordChannelIDEnc(u8 id, u8 p, _ChIdEnc chenc);
void TWMux_LoadRecordDataAll(u8 id, _RecAllInfo tb);
void TWMux_SetZoomScreen(u8 id, u16 x, u16 y, u16 w, u16 h);
void TWMux_SetZoomScreenOff(u8 id);
void TWMux_WriteDisplayToMemory(u32 sAddr, u32 x, u32 y);
void TWMux_InitMotionBox(u8 id);
void TWMux_AllMotionBoxOff(u8 id);
u32 TWMux_ReadNVStatus(u8 id);
void TWMux_MotionBox_OneChannel(u8 id, _ChannelNo i, int forced_off);
void TWMux_ChannelWindowOff(u8 id, _ChannelNo ch);
void TWMux_ChannelWindowOn(u8 id, _ChannelNo ch);
void TWMux_GetChannelDB(u8 id, u8 bufno, _ChannelInfo *Channeldb);
void TWMux_ResetDisplay(u8 id);
void TWMux_printChannelDB(u8 id);
void TWMux_ChannelWindowForcedOff(u8 id, _ChannelNo ch);
void TWMux_SelectDecClock(u8 no, u8 flg960);
void TWMux_SetVIChno(u8 no, u32 winno, u32 ch);
void TWMux_DisplayOsd_OnOff(u8 no, u8 on);
void TWMux_BT1120_OnOff(int no, u8 on);
void TWMux_Display_Boundary_OnOff(int no, u8 on);


void DCM_GateCalibration(void);
int DCM_EyeCalibration(void);
void ReadMemory(u32 addr, u8 length);
void WriteMemory(u32 addr, u8 *ptr, u8 length);
void WriteNDStatus(u32 dat);
void WriteBDStatus(u32 dat);
void WriteNVStatus(u32 dat);
void WriteMDStatus(u32 dat);
u32 ReadNDStatus(void);
u32 ReadBDStatus(void);
u32 ReadMDStatus(void);
void SpotDacOnOff(u8 on);


#endif  //_TWMUXAPI_H_
