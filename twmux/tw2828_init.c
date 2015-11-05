
/*! 
 *************************************************************************************
 * \file  tw2828_init.c
 *
 * \brief
 *    initialize PLL, Reset, etc.
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    - Heejeong Ryu  <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
//#include "config.h"

#include <linux/kernel.h>
#include <linux/string.h>
#include "tw2828_hostio.h"
#include "tw2828_reg.h"

#include "tw2828_twmux.h"
#include "tw2828_tbl.h"


_twmux tw2828devs[TW2828_MAX];


extern int InitDCM(u8);

inline int chipid(void)
{
	int id;

	id = ReadP(0x216); id<<=8;
	id |= ReadP(0x215);
	return id;
}

#define END_OF_DATA     0xff
#define CHANGE_ADDR     0xfe
#define END_OF_DATA2    0xffff
#define CHANGE_ADDR2    0xfffe

void WritePnTbl(u16 const *ptr)
{
    u16 addr, dat;
    u16 slave = 0x00ff;

    printk("\r\n++WritePnTbl----");
    while(1){
        addr = *ptr++;
//        printk("\r\n%04x", addr);
        if(addr == END_OF_DATA2) break;
        if(addr == CHANGE_ADDR2) {
          slave = *ptr++;
          printk(" %04x --", slave);
          continue;
        }
        dat = *ptr++;
        WriteP(addr,dat);
  //      printk(" %02x", dat);
    }
}

# if 0
int TWMux_Init()
{
	int  id;
	unsigned char val;
	if ((id = chipid()) != TW2828_CHIP_ID) return -1;
		printk("\r\nDetected tw2828  = %x", id);
	if(InitDCM()) return -1;
	WritePnTbl(tw2828tbl_Default);

	WriteP(0xc7c, (u8)(RECBASE));
	WriteP(0xc7d, (u8)(RECBASE>>8));
	WriteP(0xc80, (u8)(SPOTBASE));
	WriteP(0xc81, (u8)(SPOTBASE>>8));
	printk("\n");
	val  = ReadP(0xc7c);
	printk("RECBASE[0x%2x] 0x%2x\n", (u8)RECBASE, val);
	val  = ReadP(0xc7d);
	printk("RECBASE>>8[0x%2x] 0x%2x\n", RECBASE >>8, val);
	val  = ReadP(0xc80);
	printk("SPOTBASE[0x%2x] 0x%2x\n", (u8)SPOTBASE, val);
	val  = ReadP(0xc81);
	printk("SPOTBASE>>8[0x%2x] 0x%2x\n", SPOTBASE>>8, val);
#if 0
	TWMux_SetLiveSource(no, tw2828devs[no].ilvtype);
    for(i=0; i<5; i++) {
        TWMux_SetPBSource(no, i, tw2828devs[no].ipbtype[i], tw2828devs[no].ipbp[i].EmChId, tw2828devs[no].ipbp[i].YCSwap);
        //printk("\n---------- %d %d", (u16)tw2828devs[no].ipbp[i].EmChId, (u16)tw2828devs[no].ipbp[i].YCSwap);
    }
#endif

	return 0;	
}

#endif

/** @defgroup tw2828API /tw2828API 
 *  @defgroup Init /tw2828API/Init 
 *  @ingroup tw2828API
 *  @addtogroup Init
 *  \{
*/


void SetSpotVideoStdToEnc(u8 lvtype)
{
	if( (lvtype==NTSC_720) || (lvtype==NTSC_960) ) {
			WriteP(SPT1_ENC, 0x00);
			WriteP(SPT1_ENC_CTRL, 0x01);
			WriteP(SPT2_ENC, 0x00);
			WriteP(SPT2_ENC_CTRL, 0x01);
	}
	else {
			WriteP(SPT1_ENC, 0x41);
			WriteP(SPT1_ENC_CTRL, 0x05);
			WriteP(SPT2_ENC, 0x41);
			WriteP(SPT2_ENC_CTRL, 0x05);
	}
}

/*! 
 * \brief
 *    Select Decoder clock  
 *
 * \param [in] id  : chip no
 * \param [in] flg960: 1=36MHz, 0:108MHz
 *
 * \return
 *        <NONE>
 * \note
 *    
 */
void TWMux_SelectDecClock(u8 no, u8 flg960)
{
	if(flg960)
	{
   	WriteP(0x228, 0x80);
   	WriteP(0x229, 0x89);
	}
	else
	{
   	WriteP(0x228, 0);
   	WriteP(0x229, 0x88);
	}	
}

/*! 
 * \brief
 *    Set Live Source type 
 *
 * \param [in] id  : chip no
 * \param [in] mode: live source type
 *
 * \return
 *        <NONE>
 * \note
 *    
 */
void TWMux_SetLiveSource(u8 no, _IN_Type mode)
{
	u8 i;
	static const char *Str[] =  {
		"PAL_720",	"NTSC_720", "PAL_960", "NTSC_960", 0
  };

	switch (mode) {
	case 	NTSC_720: tw2828devs[no].ilvp.IHR = IHR_720;  tw2828devs[no].ilvp.IVR = IVR_NTSC; 
				   	WriteP(0x228, 0);
				   	WriteP(0x229, 0x88);
				   	WriteP(SYSTEM_SEL, 0x01);
						WriteP(0xc4d, 0x09);
						break;
    case  PAL_720:  tw2828devs[no].ilvp.IHR = IHR_720;  tw2828devs[no].ilvp.IVR = IVR_PAL; 
				   	WriteP2(0x228, 0);
				   	WriteP2(0x229, 0x88);
						WriteP2(SYSTEM_SEL, 0x00);
						WriteP2(0xc4d, 0x0b);
						break;
    case  NTSC_960: tw2828devs[no].ilvp.IHR = IHR_960;  tw2828devs[no].ilvp.IVR = IVR_NTSC; 
				   	WriteP(0x228, 0x80);
				   	WriteP(0x229, 0x89);
						WriteP(SYSTEM_SEL, 0x03);
						WriteP(0xc4d, 0x09);
						break;
    case  PAL_960:  tw2828devs[no].ilvp.IHR = IHR_960;  tw2828devs[no].ilvp.IVR = IVR_PAL; 
				   	WriteP(0x228, 0x80);
				   	WriteP(0x229, 0x89);
						WriteP(SYSTEM_SEL, 0x02);
						WriteP(0xc4d, 0x0b);
						break;
		default:
			      printk("\n[%d] Wrong LVT!", no); 			return;	
	}

	tw2828devs[no].ilvtype = mode;

  printk("\n[%d]TWMux_SetLiveSource = %s(%d)", no, Str[mode], mode); 				
	
	for(i=0; i<16; i++) {
		if((mode == NTSC_960)|| (mode==PAL_960) ) {
			WriteP(MDDETECT_CTRL_0 + 0x40*i, ReadP(MDDETECT_CTRL_0 + 0x40*i) | 0x40);
		}
		else {
			WriteP(MDDETECT_CTRL_0 + 0x40*i, ReadP(MDDETECT_CTRL_0 + 0x40*i) & ~0x40);
		}
  }
	
	SetSpotVideoStdToEnc(mode);
	
	if(tw2828devs[no].ilvp.IHR == IHR_960) {							
			WriteP(REC_DNS_CIF_0, (ReadP(REC_DNS_CIF_0)&0x0f)	| 0x50);
			WriteP(REC_DNS_CIF_1, (ReadP(REC_DNS_CIF_1)&0x0f)	| 0xf0);
			WriteP(REC_DNS_CIF_2, (ReadP(REC_DNS_CIF_2)&0x0f)	| 0xf0);
			WriteP(REC_DNS_CIF_3, (ReadP(REC_DNS_CIF_3)&0x0f)	| 0xf0);
	}	
	else {
	  	WriteP(REC_DNS_CIF_0, (ReadP(REC_DNS_CIF_0)&0x0f)	| 0x70);
			WriteP(REC_DNS_CIF_1, (ReadP(REC_DNS_CIF_1)&0x0f)	| 0xf0);
			WriteP(REC_DNS_CIF_2, (ReadP(REC_DNS_CIF_2)&0x0f)	| 0xf0);
			WriteP(REC_DNS_CIF_3, (ReadP(REC_DNS_CIF_3)&0x0f)	| 0xf0);
	}

}

/*! 
 * \brief
 *    Get current Live Source type 
 *
 * \param [in] id  : chip no
 *
 * \return
 *        Live Source type
 * \note
 *    
 */
_IN_Type TWMux_GetLiveSource(u8 id)
{
	return tw2828devs[id].ilvtype;	

}


void printcfginfo(u8 n)
{
    printk("\n -- Info Cfg TW2828_%d ", n);
    printk("\n\tlive type: %d (%s)", (u16)tw2828devs[n].ilvtype, InTypeStr[tw2828devs[n].ilvtype]);
    printk("\n\tpb 1 type: %d (%s)", tw2828devs[n].ipbtype[0], InTypeStr[tw2828devs[n].ipbtype[0]]);
    printk("\n\tpb 2 type: %d (%s)", tw2828devs[n].ipbtype[1], InTypeStr[tw2828devs[n].ipbtype[1]]);
    printk("\n\tpb 3 type: %d (%s)", tw2828devs[n].ipbtype[2], InTypeStr[tw2828devs[n].ipbtype[2]]);
    printk("\n\tpb 4 type: %d (%s)", tw2828devs[n].ipbtype[3], InTypeStr[tw2828devs[n].ipbtype[3]]);
    printk("\n\tpb 5 type: %d (%s)", tw2828devs[n].ipbtype[4], InTypeStr[tw2828devs[n].ipbtype[4]]);
    printk("\n\tmdrs     : %d (%s)", tw2828devs[n].mdrs, mTimingTbl[tw2828devs[n].mdrs].str);
		printk("\n\tpb 1 EmChId:%d  YCSwap:%d", (u16)tw2828devs[n].ipbp[0].EmChId, (u16)tw2828devs[n].ipbp[0].YCSwap);
		printk("\n\tpb 2 EmChId:%d  YCSwap:%d", (u16)tw2828devs[n].ipbp[1].EmChId, (u16)tw2828devs[n].ipbp[1].YCSwap);
		printk("\n\tpb 3 EmChId:%d  YCSwap:%d", (u16)tw2828devs[n].ipbp[2].EmChId, (u16)tw2828devs[n].ipbp[2].YCSwap);
		printk("\n\tpb 4 EmChId:%d  YCSwap:%d", (u16)tw2828devs[n].ipbp[3].EmChId, (u16)tw2828devs[n].ipbp[3].YCSwap);
		printk("\n\tpb 5 EmChId:%d  YCSwap:%d", (u16)tw2828devs[n].ipbp[4].EmChId, (u16)tw2828devs[n].ipbp[4].YCSwap);
   
}

/* 
  \brief
		Reset Record Encoder module 
	\retval <NONE> 
*/
void Reset_Enc_S(void)
{
	u8 val;

	val = ReadP(DGTBLK4RESET); //reset_enc, dec
	WriteP(DGTBLK4RESET, (val|0x03));

//	printk("\r\nReset Enc_S+M - ");
	delay(10); //100);
	
	val = ReadP(DGTBLK4RESET);
	WriteP(DGTBLK4RESET, (val&0xfc));

}

/* 
  \brief
		Reset RGB_Interface module 
	\retval <NONE> 
*/
void Reset_rgb_interface(void)
{
	u8 val;

	val = ReadP(DGTBLK1RESET);
	WriteP(DGTBLK1RESET, (val|0x01));

	delay(1);
	//printk("\r\nReset_rgb_interface - ");

	val = ReadP(DGTBLK1RESET);
	WriteP(DGTBLK1RESET, (val&0xfe));

}

/* 
  \brief
		Reset Frame Rate Control module 
	\retval <NONE> 
*/
void Reset_frsc(void)
{
	u8 val;

	val = ReadP(DGTBLK2RESET);
	WriteP(DGTBLK2RESET, (val|0x08));

	delay(1);
	//printk("\r\nReset_frsc - ");

	val = ReadP(DGTBLK2RESET);
	WriteP(DGTBLK2RESET, (val&0xf7));

}

/*!TWMux_Init 
  \brief
		init tw2828 
	\retval HResult
	          if HResult==-1, error   
	\par Update history:
		Mar/08/2012 - Created  \n
		JUNE/20/2012 - Changed for ASIC
	\par Description: 
	  (1) Check chip id \n
	  (2) Download basic TW2828 register value for initial working \n
	  (3) Set VPLL, MPLL, MCLK,  Calibrate DRAM Control Module \n
	  (4) Define Memory Map \n 
	      Motion     = 0  \n
	      Display Memory start address (DISPAYBASE) = Motion + 40  \n
	      Record Memory Start Address = DISPAYBASE + 1080 \n
	      SPOT Memory Start Address SPOTBASE	= (768) 2nd page \n
	  (4) Define SD/HD input resolution for each port  \n
	  
*/
HResult TWMux_Init(int no, _twmux Cfg)
{
  int id;
  
  printk("\n++Init Tw2828_%d", no); 
  
  if((id=chipid()) != TW2828_CHIP_ID ) return -1;
	printk("\r\nDetected tw2828_%d! = %x", no, id);

	if(InitDCM(no)) return -1; 

  	memcpy( &tw2828devs[no], &Cfg, sizeof(_twmux));
	printcfginfo(no);     

	WritePnTbl(tw2828tbl_Default);  

	//WriteP(0xc7c, (u8)(DISPLAYBASE));
	//WriteP(0xc7d, (u8)(DISPLAYBASE>>8));

	WriteP2(0xc7c, (u8)(RECBASE));
	WriteP2(0xc7d, (u8)(RECBASE>>8));
	WriteP(0xc80, (u8)(SPOTBASE));
	WriteP(0xc81, (u8)(SPOTBASE>>8));

	TWMux_SetLiveSource(no, tw2828devs[no].ilvtype);
	printk("we do not care PB now\n");
#if 0
	int i;
	for(i=0; i<5; i++) {
		TWMux_SetPBSource(no, i, tw2828devs[no].ipbtype[i], tw2828devs[no].ipbp[i].EmChId, tw2828devs[no].ipbp[i].YCSwap);
		printk("\n---------- %d %d", (u16)tw2828devs[no].ipbp[i].EmChId, (u16)tw2828devs[no].ipbp[i].YCSwap);
	}
#endif
	return 0;
}	


void TWMux_BT1120_OnOff(int no, u8 on)
{
	if(on) 	{
			WriteP(0x201, ReadP(0x201)&0xfb);
			WriteP(0x485, ReadP(0x485)&0x7f);
	}
	else {
			WriteP(0x201, ReadP(0x201)|0x04);
			WriteP(0x485, ReadP(0x485)|0x80);
	
	}
}

void TWMux_Display_Boundary_OnOff(int no, u8 on)
{
	if(on) 	{
			WriteP(0x485, ReadP(0x485)|0x10);
	}
	else {
			WriteP(0x485, ReadP(0x485)&0xef);
	
	}
}

/** @} */

