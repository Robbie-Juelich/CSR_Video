#ifndef _DISP_H_
#define _DISP_H_

#ifdef __cplusplus
 extern "C" {
 #endif 

#include "tw2828_twmux.h"

typedef struct DataChannelInfo {
	u8  chno;
	u8	port;   // [7:6] : Crop position 1/4,2/4,3/4,4/4
              // [5:4] : PB
      				// [3:0] : Live
} _DataChannelInfo;

//-------------------
// Main Display Channel option
typedef enum MDP_Type {
MODE_20CH,  //0
MODE_6CH,	 //1
MODE_12CH,	//2
MODE_4HD_PB,//3  4xHD Test
MODE_HD_FULL,//4
MODE_HD_HALF,//5
MODE_16PB_4LIVE,//6
MODE_16LIVE_4HD,//7 
MODE_16QUADPB,//8
NONE9, //MODE_16CIFPB,//9 --> Not support
MODE_32CH,//a
MODE_2_8D1PB_4LIVE,//b
MODE_4D1_4CUT,//c
NONE_D,//d
NONE_E, //MODE_16HALFD1_FS,//e --> Not Support
NONE_F, //MODE_16HALFD1,//f ---> Not Support
// 6VGA 16cut
MODE_6VGA_16CUT,//10
MODE_32CH_ONLY,//11
NONE12,//12   ---> Not support
MODE_32CH_ONLY_2,//13
NONE14,//MODE_16LIVE, //14
MODE_12CH_CASCADE_Master,//15
// Complex 
MODE_1HD_8D1PB,//16
// New Field Switch mode
MODE_16D1_FS_NEW,//17
MODE_32CH_ONLY_3, //18 New Field Switch mode
// HD pb
MODE_1HD_16CUT, //19
MODE_1HD_8CUT_8Live, //1a
MODE_5PB_5LIVE,//1b
MODE_5PB_1LIVE,//1c
MODE_36CH, //1d
MODE_CASCADE_PB5_LIVE, //1e
MODE_16LIVE_HDSPOT, //1f
MODE_16LIVE_HDSPOT1, //20
MODE_4CH_HDSPOT_DEMO, //21 //Created to support Sechu-Tech Show.
MODE_4CH, //22 
MODE_16LIVE_HDSPOT2, //23
MODE_9CH, //24 
MODE_16LIVE_HDSPOT3, //25
MODE_8CH,
MAXMODE
} MDPS_t;

typedef  void (*pfunc)(u8 id);
typedef  void (*pGetFn)(void);

typedef struct {
	MDPS_t mdsp;
	char *mstr;
	void (*getChannelDB)(void);	
	pfunc ActSpot1;
	pfunc ActSpot2;
	pfunc ActRec;
} _actFunc;

extern u8 CurDisplayMode;
extern u8 PBAutoMode;
extern u32 IHR, IVR, PHR, PVR, UHR, UVR, INTERLACED;	
extern _actFunc msdpTbl[];

void DisplayMainChannelWindowsAll(void);
void  InitMainWindow(u8 winno, u16 x, u16 y, u16 w, u16 h);
void DisplayMainWindows(u8 id, _ChannelInfo *pC);
void MdspAction(u8 id, MDPS_t m);

void LoadMdspDb_20CH(void);
void LoadMdspDb_18CH(void);
void LoadMdspDb_6CH(void);
void LoadMdspDb_12CH(void);
void LoadMdspDb_HD_Full(void);
void LoadMdspDb_HD_Half(void);
void LoadMdspDb_16D1PB4Live(void);
void LoadMdspDb_32CH(void);
void LoadMdspDb_2_8D1PB4Live(void);
void LoadMdspDb_4CH_HDSpot_Demo(void);
void LoadMdspDb_1HDn8D1PBn4Live(void);
void LoadMdspDb_16PB_16Live(void);
void LoadMdspDb_16LiveN16PB(void);
void LoadMdspDb_16LiveN16PB_FieldSwitch(void);
void LoadMdspDb_16D1PB4Live(void);
void LoadMdspDb_16QuadPB(void);
void LoadMdspDb_2_4D1_4Cut(void);
void LoadMdspDb_16D1_NewFieldSwitching(void);
void LoadMdspDb_16HalfD1(void);
void LoadMdspDb_4HD_PB(void);
void LoadMdspDb_6VGA_16Cut(void);
void LoadMdspDb_1HD_16Cut(void);
void LoadMdspDb_1HD_8Cut_8Live(void);
void LoadMdspDb_16Live(void);
void LoadMdspDb_16Live_HDSpot(void);
void LoadMdspDb_16Live_HDSpot1(void);
void LoadMdspDb_16Live_HDSpot2(void);
void LoadMdspDb_12Live_Master(void);
void LoadMdspDb_16LIVE_4HD(void);
void LoadMdspDb_5PB_5LIVE(void);
void LoadMdspDb_5PB_1LIVE(void);
void LoadMdspDb_16Live_20PB(void);
void LoadMdspDb_PB5toLive_4HD(void);
void LoadMdspDb_4CH(void);
void LoadMdspDb_9CH(void);
void LoadMdspDb_8CH(void);


int MainDisplayTest(int argc, char **argv);
int ChangeMdRsTest(int argc, char **argv);
int PrintChannelDataBase(int argc, char **argv);

#ifdef __cplusplus
 }
#endif 

#endif
