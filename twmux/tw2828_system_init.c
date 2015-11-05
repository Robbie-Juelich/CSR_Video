
/*! 
 *************************************************************************************
 * \file  system.c
 *
 * \brief
 *    bringup system and test misc.
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    - Heejeong Ryu  <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
#if 0
#include <stdio.h>

#include "config.h"
#include "disp.h"
#include "system.h"
#include "tw2828_hostif.h"
#include "string.h"
#include "time.h"
#include "gv7601.h"
#include "sil9135.h"
#include "osd.h"
#include "mem_map.h"
#include "twmux.h"
#include "evb_tbl.h"
#endif

#include <linux/kernel.h>
#include <linux/string.h>
#include "tw2828_hostio.h"
#include "tw2828_config.h"
#include "tw2828_twmux.h"
#include "tw2828_mse.h"
#include "tw2828_test.h"

_twmux usrMuxs[TW2828_MAX];

int LOOPBACK[TW2828_MAX];

/** @defgroup System /tw2828Test/System 
 *  @ingroup tw2828Test
 *  @addtogroup System
 *  \{
*/

int Check_HDSdi_rx_Status(u8 p)
{
	int i=0, dat, pbt;
	enum {HDSDI_IN_0,	HDSDI_IN_1,	HDSDI_IN_2,	HDSDI_IN_3 };

	switch (p){
	case HDSDI_IN_0: i=1; break;
	case HDSDI_IN_1: i=2; break;
	case HDSDI_IN_2: i=3; break;
	case HDSDI_IN_3: i=0; break;
  }	
	
	//dat = GetHD_VD_STD(i);
	dat = -1;
	if(dat != -1) pbt = dat;
	else { 
		//printk("[1] pb%d HDSDI-Rx - No Input or Wrong Input", p);	 
		pbt = NOVIDEO; //HD_1080I; 
	}
	
	return pbt;
}

/*! 
 * \brief
 *    Check HD-SDI Receiver input source resolution 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      (1) Check HDSDI input \n
 *      (2) put PBT value to PB 1-4 for TW2828_1. 
 * \par Usage:
 *      ge 
 */
int CheckHDSdi_Rx(int argc, char **argv)
{
	int i, pbt ;		
	
	for(i=0; i<4; i++) {
		pbt = Check_HDSdi_rx_Status(i);
	  printk("\n[1] PB%d HDSDI-Rx -- PBT=%d", i, (u16)pbt);
	  
 	  TWMux_SetPBInType(TW2828_1, i, pbt); 
	  
  }
  return 0;
}
#if 0
int GetHDMI_Input_Type(int pb)
{
	int no=0, ha, va, pbt, intr;
	enum {HDMI_IN_0,	HDMI_IN_1,	HDMI_IN_2,	HDMI_IN_3 };
	
    switch (pb){
    case HDMI_IN_0: no=2; break;
    case HDMI_IN_1: no=1; break;
    case HDMI_IN_2: no=0; break;
    case HDMI_IN_3: no=3; break;
    }	

		ha = GetHDInputHDE(no);
		va = GetHDInputVDE(no);
		intr = GetHDInputInterlaced(no);
				
		if(va == 720) 
			pbt = HD_720P;
		else if( va == 1080 )
			pbt = HD_1080P;
		else if(( va == 540 ) && (intr == 1))
			pbt = HD_1080I;	
		else { 
			 //printk("\n[0] PB%d HDMI-Rx : Pls, Check Input!", pb);	 
			 pbt = NOVIDEO; //HD_1080I; 
			 return pbt;
			}

	     //printk("\n[0] PB%d HDMI-Rx : detected: PBT=%d", pb, (u16)pbt);
    return pbt;
}

/*! 
 * \brief
 *    Check HDMI Receiver input source
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      (1) Check HDMI Rx input \n
 *      (2) Init HDMI Rx which is PB source for TW2828_0. 
 * \par Usage:
 *      si [init] 
 */
int CheckHDMI_Rx_Input(int argc, char **argv)
{
		u8	chipno = 0, pb, pbt;
		u16	ha, va, itr;
		enum {HDMI_IN_0,	HDMI_IN_1,	HDMI_IN_2,	HDMI_IN_3 };

		if( argc==1 ) 
		{
			printk("\n--- CheckHDInputStatus \r");
			
			for(pb=0; pb<4; pb++) {	
				
       	switch (pb){
       	case HDMI_IN_0: chipno=2; break;
       	case HDMI_IN_1: chipno=1; break;
       	case HDMI_IN_2: chipno=0; break;
       	case HDMI_IN_3: chipno=3; break;
        }	
        
				ha = GetHDInputHDE(chipno);
				va = GetHDInputVDE(chipno);
				itr = GetHDInputInterlaced(chipno);
				
				if(va == 720) 
					pbt = HD_720P;
				else if( va == 1080 )
					pbt = HD_1080P;
				else 
					pbt = HD_1080I;	
					
				printk("\n[0] pb#%d HDMI-Rx - HA:%d, VA:%d, Interlaced:%d, pbt=%d", pb, ha, va, itr, pbt);

	 	    TWMux_SetPBInType(TW2828_0, pb, pbt); 

			}	
		}
		else if( argc == 2 )
		{
			HDMI_rx_Init();			
		}	
	return 0;
}
#endif

#if 0
void CheckHDInput(void)
{
	u32 ct;
	u8  val, host, i, id, pb;
	int newpbt;
	static _In_Port_Info pbinfo[TW2828_MAX][4] = { {0,0,0,0},{0,0,0,0}};
	static int cnt[TW2828_MAX][4], pbt[TW2828_MAX][4];
	extern _ChannelInfo ChannelDB[];
	_ChannelInfo C;
		
	ct = get_tic01();
	if((ct%20)!=0) 	return;

	host = HostBusNo; 
	
	for(id=0; id<TW2828_MAX; id++)
	{
		HostBusNo = id; 

		for(pb=0; pb<4; pb++) {
			if( TWMux_GetPBInType(id, pb) < HD_1080I) continue;
			
			#ifdef SUPPORT_TW2828_HDNOVIDEO_CHECK
			val = (TWMux_ReadNVStatus(id)>>16) & 0x0F;
			if((val & (0x01<<pb)) != 0x00) {
				newpbt = NOVIDEO;
			}	
			#else
			if(id == TW2828_0) 
				newpbt = GetHDMI_Input_Type(pb); 	
			else 
				newpbt = Check_HDSdi_rx_Status(pb);
			#endif
			
			if(pbt[id][pb] == newpbt) {
				cnt[id][pb]++;
				if(cnt[id][pb] < 5) continue;
				cnt[id][pb] = 0;	
			}	
			else {
				cnt[id][pb] = 0;	
				pbt[id][pb] = newpbt;				
			}
			
			if(pbt[id][pb] != pbinfo[id][pb].intype) {

				printk("\n[%d] Changed HD PBT[%d] = %d -----------------", id, pb, pbt[id][pb]);
	
				pbinfo[id][pb].intype = pbt[id][pb];	
				
				if(pbt[id][pb]!= NOVIDEO)
					TWMux_SetPBInType(id, pb, pbt[id][pb]);
					
				for(i=0; i<36; i++) {
	 				TWMux_GetChannelDB(id, i, (_ChannelInfo *)&C);
    			memcpy(&ChannelDB[i], &C, sizeof(_ChannelInfo));
				}

				if((ChannelDB[pb+16].ctrl & AUTOMODEON) == AUTOMODEON)
  				DisplayMainWindows(id, (_ChannelInfo *)&ChannelDB);
  			else	
  				TWMux_SetMainChannel(id, &ChannelDB[pb+16]);
	  			
				if(pbt[id][pb]!= NOVIDEO)
					TWMux_MotionBox_OneChannel(id, pb+PB1, 0);
				else	{
					TWMux_ChannelWindowForcedOff(id, pb+PB1);
					TWMux_MotionBox_OneChannel(id, pb+PB1, 1); //forced_off = 1
				}
					
			}
				
		}		
	}
	
	HostBusNo = host; 

}

#endif

void sdelay(void)
{
	int i;
	for (i=0;i<0xffffff ;i++ )
	{
	}
}

#ifdef __FIRMWARE__
void ResetDevice(void)
{
	IO_DAT_nRESET(0);

	delay(10);					
	IO_DAT_nRESET(1);

	printk("\r\nReset Devices");
	delay(200);					
}

void ResetTW2828(void)
{
	IO_DAT_nRST_28(0);
	delay(20);					
	IO_DAT_nRST_28(1);
	printk("\r\nReset TW2828");
	delay(10);					
}
#endif

/*! 
 * \brief
 *    Reset All of devices by control nReset 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *      resetd 
 */
int TestResetDevice(int argc, char **argv)
{
	//ResetDevice();
	
	return 1;
}

/*! 
 * \brief
 *    Hardware Reset of TW2828 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *      reset 
 */
int TestTW2880Reset(int argc, char **argv)
{
	//ResetTW2828();
	
	return 1;
}

void SetMainDisplayResolution (int no, int mdrs)
{
	mdrs = TWMux_SetMainDisplayResolution(no, mdrs);
	delay(100);	
	//HDMI_tx_Init(no, mdrs);
	 
	usrMuxs[no].PHR=mTimingTbl[mdrs].hde; 
	usrMuxs[no].PVR=mTimingTbl[mdrs].vde;

	if( (mTimingTbl[mdrs].ctrl & INTERLACE) == INTERLACE) {
		usrMuxs[no].PVR *= 2;
		usrMuxs[no].UHR = usrMuxs[no].PHR;
		usrMuxs[no].UVR = usrMuxs[no].PVR;
	}
	else
	{	
		if(usrMuxs[no].PHR > MHR_UPS) 
		{
			usrMuxs[no].UHR = MHR_UPS;
			usrMuxs[no].UVR	= MVR_UPS;
		}
		else if(usrMuxs[no].PVR > MVR_UPS) {  // to use memory effective for 1600*900
			usrMuxs[no].UHR = usrMuxs[no].PHR;
			usrMuxs[no].UVR	= MVR_UPS;
		}
		else {
			usrMuxs[no].UHR = usrMuxs[no].PHR;
			usrMuxs[no].UVR	= usrMuxs[no].PVR;
		}
  }
  
  

}

void GetIOInfoData (int no)
{	
	u8 mdrs, ilvtype;
	
	mdrs = TWMux_GetMainDisplayResolution(no);
	ilvtype = TWMux_GetLiveSource(no);
	TWMux_GetMainDisplayInfo(no, &PHR, &PVR, &UHR, &UVR);
	
  INTERLACED = ( (mTimingTbl[mdrs].ctrl & INTERLACE) == INTERLACE) ? 1 : 0;
  	
	switch (ilvtype) {
	case 	NTSC_720: IHR = IHR_720;  IVR = IVR_NTSC; break;
  case  PAL_720:  IHR = IHR_720;  IVR = IVR_PAL; 	break;
  case  NTSC_960: IHR = IHR_960;  IVR = IVR_NTSC; break;
  case  PAL_960:  IHR = IHR_960;  IVR = IVR_PAL; 	break;
  default: break;	
	}

  printk("\n[%d] IHR=%d IVR=%d PHR=%d PVR=%d UHR=%d UVR=%d INTERLACED=%d", no, IHR, IVR, PHR, PVR, UHR, UVR,INTERLACED  );

}

void printkUsrCfg(u8 n, _twmux Cfg)
{
    printk("\n -- Info User Cfg TW2828_%d ", n);
    printk("\n\tlive type: %d (%s)", (u16)Cfg.ilvtype, InTypeStr[Cfg.ilvtype]);
    printk("\n\tpb 1 type: %d (%s)", Cfg.ipbtype[0], InTypeStr[Cfg.ipbtype[0]]);
    printk("\n\tpb 2 type: %d (%s)", Cfg.ipbtype[1], InTypeStr[Cfg.ipbtype[1]]);
    printk("\n\tpb 3 type: %d (%s)", Cfg.ipbtype[2], InTypeStr[Cfg.ipbtype[2]]);
    printk("\n\tpb 4 type: %d (%s)", Cfg.ipbtype[3], InTypeStr[Cfg.ipbtype[3]]);
    printk("\n\tpb 5 type: %d (%s)", Cfg.ipbtype[4], InTypeStr[Cfg.ipbtype[4]]);
    printk("\n\tmdrs     : %d (%s)", Cfg.mdrs, mTimingTbl[Cfg.mdrs].str);
		printk("\n\tpb 1 EmChId:%d  YCSwap:%d", (u16)Cfg.ipbp[0].EmChId, (u16)Cfg.ipbp[0].YCSwap);
		printk("\n\tpb 2 EmChId:%d  YCSwap:%d", (u16)Cfg.ipbp[1].EmChId, (u16)Cfg.ipbp[1].YCSwap);
		printk("\n\tpb 3 EmChId:%d  YCSwap:%d", (u16)Cfg.ipbp[2].EmChId, (u16)Cfg.ipbp[2].YCSwap);
		printk("\n\tpb 4 EmChId:%d  YCSwap:%d", (u16)Cfg.ipbp[3].EmChId, (u16)Cfg.ipbp[3].YCSwap);
		printk("\n\tpb 5 EmChId:%d  YCSwap:%d", (u16)Cfg.ipbp[4].EmChId, (u16)Cfg.ipbp[4].YCSwap);
}

int TW2828Init(int no, _twmux usrmux)
{
  int host, HResult;
  
	memcpy( &usrMuxs[no], &usrmux, sizeof(_twmux));
    printkUsrCfg( no, usrMuxs[no]);
   
	host = HostBusNo;
 	SetHostBus(no);

	HResult = TWMux_Init(no, usrMuxs[no]);
  
	if(HResult)  
		printk("\ntw2828_%d init error.", no);
	else {

	  TWMux_SetLiveSource(no, usrmux.ilvtype);
	 #if 0  // already done in TWMux_Init 
		int i;
		for(i=0; i<4; i++) { 
	    if(no==TW2828_0) {
 		    usrMuxs[0].ipbp[i].intype = GetHDMI_Input_Type(i); 
   	     printk("\n[0] PB%d HDMI-Rx : detected: PBT=%d", i, (u16)usrMuxs[0].ipbp[i].intype);
   	    if( usrMuxs[0].ipbp[i].intype == NOVIDEO) usrMuxs[0].ipbp[i].intype = HD_1080I;
 		  }
	  	else  {
	 			usrMuxs[1].ipbp[i].intype = Check_HDSdi_rx_Status(i);
   	     printk("\n[1] PB%d HDSDI-Rx : detected: PBT=%d", i, (u16)usrMuxs[1].ipbp[i].intype);
   	    if( usrMuxs[1].ipbp[i].intype == NOVIDEO) usrMuxs[1].ipbp[i].intype = HD_1080I;
	 		}
			
			TWMux_SetPBSource(no, i, usrMuxs[no].ipbp[i].intype, usrMuxs[no].ipbp[i].EmChId, usrMuxs[no].ipbp[i].YCSwap);
		}	
	 #endif
	
	SetMainDisplayResolution(no, usrMuxs[no].mdrs);

//	InitMse(no);   // do not do it now
	InitOSD(no, FONT_22x22);
	InitRecOSD(no, FONT_22x22);

	GetIOInfoData(no);
//	MdspAction(no, MODE_20CH);
//		MdspAction(no, MODE_16LIVE_4HD);
//		MdspAction(no, MODE_4CH_HDSPOT_DEMO);

//	SpotInit(no, ON,ON);  // turn on spot
	}
	
	SetHostBus(host);

	return HResult;
}

#if 0
int System_Init(void)
{
	ResetDevice(); 

	Extern_AnEncoder_Init();
	HDMI_rx_Init();
	HDSDI_rx_Init();
	
	ResetTW2828(); 

	TWMux_SelectDecClock(TW2828_0, 0);
	TWMux_SelectDecClock(TW2828_1, 1);
                      
	InitExtDec0(GET_VDO());
	InitExtDec1(GET_VDO());


  if(GET_VDO()) {
		 printk("\nOPTION SW 3 = NTSC");
	   TW2828Init(TW2828_0, Default_NTSC_720_1080i);
     TW2828Init(TW2828_1, Default_NTSC_960_1080i);
  }	
  else
  {
		 printk("\nOPTION SW 3 = PAL");
	   TW2828Init(TW2828_0, Default_PAL_720_1080i);
     TW2828Init(TW2828_1, Default_PAL_960_1080i);
  }   

	return 0;
}

/*! 
 * \brief
 *    init system 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *      init [hostbusno]   : init tw2828_#  \n
 *      init               : init tw2828_0 and tw2828_1
 */
int TestSystemInit(int argc, char **argv)
{
	int i;
	if( argc==2 ) {
		i = (unsigned int) Asc2Bin( argv[1] );
		if(i==TW2828_0) {
		  if(GET_VDO()) {
			  printk("\nOPTION SW 3 = NTSC");
	    	TW2828Init(TW2828_0, Default_NTSC_720_1080i);
  		}	
  		else
		  {
		  	printk("\nOPTION SW 3 = PAL");
	    	TW2828Init(TW2828_0, Default_PAL_720_1080i);
  		}	
		}
		else {
		  if(GET_VDO()) {
			  printk("\nOPTION SW 3 = NTSC");
	    	TW2828Init(TW2828_1, Default_NTSC_960_1080i);
  		}	
  		else
		  {
		  	printk("\nOPTION SW 3 = PAL");
	    	TW2828Init(TW2828_1, Default_PAL_960_1080i);
  		}	
		}		  	
  }
  else {
  	System_Init();
  }
  
	return 1;
}

#endif
/*! 
 * \brief
 *    Set Live Input type 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *      LVT <n>   \n
        - n : 1=720NTSC, 0=720PAL, 2=960PAL, 3=960NTSC
 */
int SetLiveInputType(int argc, char **argv)
{
	static const char *Str[] =  {
		"PAL_720",	"NTSC_720", "PAL_960", "NTSC_960", 0
  };

	int i;
	
	if( argc==2 ) 
		i = (unsigned int) Asc2Bin( argv[1] );
	else	{
		i = TWMux_GetLiveSource(HostBusNo);
	  printk("\n[%d] Current LVT = %s(%d)", HostBusNo, Str[i],i );
		printk("\nUsage: LVT n  : 1=720NTSC, 0=720PAL, 2=960PAL, 3=960NTSC");
		return FALSE;
	}
	
	if(i>3) i=1;
		
	TWMux_SetLiveSource(HostBusNo, i);
	printk("\n[%d] LVT = %s(%d)", HostBusNo, Str[i],i );

	return 1;
}

/*! 
 * \brief
 *    Set Playback Input type 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *    PBT <pb> <n>     : pb=0-4:all,  n=0-6 \n 
 *		 - PAL_720=0, NTSC_720=1, PAL_960=2, NTSC_960=3,\n 
 *       HD_1080I=4, HD_1080P=5, HD_720P=6, NTSC_4D1, PAL_4D1 \n 
 */
int SetPBInputType(int argc, char **argv)
{
	int pb,i;
	if( argc==3 ) {
		pb = Asc2Bin( argv[1] )<5? Asc2Bin( argv[1] ): 0;
		i = Asc2Bin( argv[2] )<MAX_PB_IN? Asc2Bin( argv[2] ):HD_1080I;
	}
	else	{
		printk("\nUsage: PBT pb n  : pb=0-4:all,  n=0-6; \n \
		\tPAL_720=0, NTSC_720=1, PAL_960=2, NTSC_960=3, NTSC_4D1=4, PAL_4D1=5\n \
        \t\tHD_1080I=6, HD_1080P=7, HD_720P=8 )\n");
		for(pb=0; pb<5; pb++) {
			i = TWMux_GetPBInType(HostBusNo, pb);
			printk("\n[%d] Current PBT[%d] = %s(%d)",HostBusNo, pb, InTypeStr[i], i);
    }

		return FALSE;
	}

	if ( !strcmp( (char *)argv[1], "all" ) ) {
  		printk("\n[%d] All PBT = %d", HostBusNo, i);

		for(pb=0; pb<5; pb++)
			TWMux_SetPBInType(HostBusNo, pb, i);

    if(i<HD_1080I) {
   	  LOOPBACK[HostBusNo] = 1;
			TWMux_SetPBLoopback(HostBusNo, 1);
			printk("\n[%d] PB LOOP Back = %s", HostBusNo, LOOPBACK[HostBusNo]? "ON":"OFF");

	    printk("\n For SD internal loopback from REC34, you should set SW1.3=OFF");
    }
    else {
    	
    }
		return 1;
	}
	
	printk("\n[%d] PBT[%d] = %s(%d)", HostBusNo, pb, InTypeStr[i], i);
	TWMux_SetPBInType(HostBusNo, pb, i);

  if((i<HD_1080I) && (pb==4))
	   printk("\n For SD internal loopback from REC34, you should set SW1.3=OFF");

	return 1;
}

/*! 
 * \brief
 *    Set Playback loop back ON/OFF for Record test 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *    PL <1/0>     
 */
int SetPBLoopBack(int argc, char **argv)
{
	int on;
	if( argc==2 ) {
		on = (int)(Asc2Bin( argv[1]) & 0x01);
	}
	else	{
		printk("\nUsage: PL <1/0>  : 1=Loop_back ON, 0=Normal mode");
		printk("\n[%d] Current LOOPBACK = %s", HostBusNo, LOOPBACK[HostBusNo]? "ON":"OFF");
		return FALSE;
	}
	
  LOOPBACK[HostBusNo] = on;
	TWMux_SetPBLoopback(HostBusNo, on);

	printk("\n[%d] PB LOOP Back = %s", HostBusNo, LOOPBACK[HostBusNo]? "ON":"OFF");

	return 1;
}

/*! 
 * \brief
 *    Mouse test
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *    mse init      : mouse image load
 */
int MseTest(int argc, char **argv)
{

	if ( !strcmp( (char *)argv[1], "init" ) ) {	
		InitMse(HostBusNo);
	}
  else
  {
			printk("\nUsage:");
			printk("\nmse init : load mse image" );
	}

	return 1;

}

//--------------------------------------------------------------------------

/*! 
 * \brief
 *    Freeze test 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *    freeze main <pbautomode> <bufno> <on/off>    : pbautomode=1/0, bufno=0-F \n 
 *    freeze rec  <bufno> <on/off>   : bufno=0-F \n 
 *    freeze spot <bufno> <on/off>   : bufno=0-F \n 
 */
int Freeze_test(int argc, char **argv)
{
	u8 bufno, automode=0, er=0;

	if(argc != 4) er=1;
	bufno = Asc2Bin(argv[2])&0x1f;
	if(bufno>20) bufno=0;
	if(Asc2Bin(argv[2])&0x20) automode=1;


	if ( !strcmp( (char *)argv[1], "main" ) ) {
		
		if ( !strcmp( (char *)argv[3], "on" ) ) {
			TWMux_SetFreezeChannelMain(HostBusNo, automode, bufno, ON);
		}
		else if ( !strcmp( (char *)argv[3], "off" ) ) {
			TWMux_SetFreezeChannelMain(HostBusNo, automode, bufno, OFF);
		}
		else {
			er=1;
		}

	}
	else if ( !strcmp( (char *)argv[1], "rec" ) ) {
		if ( !strcmp( (char *)argv[3], "on" ) ) {
			TWMux_SetFreezeChannelRec(HostBusNo, bufno, ON);
		}
		else if ( !strcmp( (char *)argv[3], "off" ) ) {
			TWMux_SetFreezeChannelRec(HostBusNo, bufno, OFF);
		}
		else {
			er=1;
		}

	}
	else if ( !strcmp( (char *)argv[1], "spot" ) ) {
		if ( !strcmp( (char *)argv[3], "on" ) ) {
			TWMux_SetFreezeChannelSpot(HostBusNo, bufno, ON);
		}
		else if ( !strcmp( (char *)argv[3], "off" ) ) {
			TWMux_SetFreezeChannelSpot(HostBusNo, bufno, OFF);
		}
		else if ( !strcmp( (char *)argv[3], "allon" ) ) {
			for(bufno=0; bufno<16; bufno++)
				TWMux_SetFreezeChannelSpot(HostBusNo, bufno, ON);
		}
		else if ( !strcmp( (char *)argv[3], "alloff" ) ) {
			for(bufno=0; bufno<16; bufno++)
				TWMux_SetFreezeChannelSpot(HostBusNo, bufno, OFF);
		}
		else {
			er=1;
		}

	}

	else {
			printk("\r\nUsage:");
			printk("\r\nfreeze main <bufno> <on/off> : ");
			printk("\r\n\t bufno-  bit[4]:automode(Reg6b4[3]), bit[3:0]:ch \r\n\t example: freeze main 16 on  : main pb6 freeze enable");
			printk("\r\nfreeze rec <bufno> <on/off>  : bufno:0-F");
			printk("\r\nfreeze spot <bufno> <on/off>" );
	}

	return 1;
}

/** @} */ 
