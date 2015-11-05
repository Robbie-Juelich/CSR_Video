
/*! 
 *************************************************************************************
 * \file  tw2828_pb.c
 *
 * \brief
 *    initialize pb
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    Heejeong Ryu <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */

#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"

/** @defgroup PB /tw2828API/PB 
 *  @ingroup tw2828API
 */

/** @addtogroup PB
 *  @{
 */

//-----------------------------------------------------------------------------
// Playback 
//-----------------------------------------------------------------------------

#define PB_AnalogChidDec_Enable(pb,on)  WriteP(PB0_CHID_DEC+pb*5, (ReadP(PB0_CHID_DEC+pb*5) & 0xfe)|on)
#define PB_DigitalChidDec_Enable(pb,on) WriteP(PB0_CHID_DEC+pb*5, (ReadP(PB0_CHID_DEC+pb*5) & 0xfd)|(on<<1))

// Chanel ID Dec control
#define DEC_ANALOG_EN		  0x01
#define DEC_DIGITAL_EN		0x02
#define DEC_AUTO_DEC_EN		0x04
#define DEC_RUN_IN_CLK_EN	0x08

//! _ChIdDec
typedef struct ChIdDec
{
	u32 ctrl;		/* Control */
	u32 vos;		// vertical data start(even field)
	u32 fos;		// vertical data start offset(odd field) 
	u32 hos;		// Horizontal data start offset
	u32 pixel_w;	// one bit data unit (pixel) for Analog channel ID
	u32 mid_val;	// theshold level for Analog channel ID
	u32 vsize;		// number of lines to get into Analog channel ID decoder

} _ChIdDec;


_ChIdDec Default_Chid_Dec = {
	(0 |  //DEC_RUN_IN_CLK_EN| 
	 DEC_AUTO_DEC_EN| 
	 DEC_DIGITAL_EN| \
	 0), //ctrl //DEC_ANALOG_EN), //ctrl
	0, //vos
	0, //fos
	0, // hos;
	4, //pixel_w
	0x1f, //mid_val
	0x0e,
};

void SetRGBMode(u8 pbmode)
{
	u8 val;

//	#ifdef DEBUG_PB
	printk("\r\n++(SetRGBMode) PBMode = %d", pbmode);
//	#endif
	
	val = ReadP(SYSTEM_MISC_CTRL) & 0xdf;

	if(pbmode==1) {
		WriteP(PB_MODE, 0x08);
		WriteP(SYSTEM_MISC_CTRL, (u8)(val|0xe0) );

		WriteP(PB_FRC_CTRL_0, 0x33); 
		WriteP(PB_FRC_CTRL_1, 0x33);
		WriteP(PB_FRC_CTRL_2, (ReadP(PB_FRC_CTRL_2)&0x0f) | 0x30);

	}
	else {
		WriteP(PB_MODE, 0x00);
		WriteP(SYSTEM_MISC_CTRL, (u8)(val | 0xc0) );
	
		WriteP(PB_FRC_CTRL_0, 0x00); 
		WriteP(PB_FRC_CTRL_1, 0x00);
		WriteP(PB_FRC_CTRL_2, (ReadP(PB_FRC_CTRL_2)&0x0f));

	}

}

void SetPBInputCropStart(u8 pb, u8 nQuad, u8 x, u8 y)
{
	if((ReadP(PB_MODE)&0x08)==0x00) {
		#ifdef DEBUG_PB
		dprintf("\r\n++(SetPBInputCropStart) Error! Check PB mode.)");
		#endif
		return;
}

	WriteP(RGB_XSTART_0+4*pb+nQuad, x); 
	WriteP(RGB_YSTART_0+4*pb+nQuad, y); 

}

void SetInputCropStart(u8 win, u8 x, u8 y)
{
	if((win>=CAS1) && (win<=CAS4)) {
		win = win - CAS1 + 12;
		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)|0x04);
	}
	else if((win==PB5)) {
		win = 12;
		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)|0x04);
	}

	WriteP(RGB_XSTART_0+win, x); 
	WriteP(RGB_YSTART_0+win, y); 

	WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)&~0x04);

}

void PB_auto_channel_enable(u8 pb)
{
	if( pb<8)	
		WriteP(PB_CH_EN0, ReadP(PB_CH_EN0)|(0x01<<pb));
	else 
		WriteP(PB_CH_EN1, ReadP(PB_CH_EN1)|(0x01<<(pb-8)));
}

void PB_auto_channel_disable(u8 pb)
{
	if(pb<8)
		WriteP(PB_CH_EN0, ReadP(PB_CH_EN0)&~(0x01<<pb));
	else 
		WriteP(PB_CH_EN1, ReadP(PB_CH_EN1)&~(0x01<<(pb-8)));
}

//--

void PbPortDnsTarget(u8 pb, u16 tw, u16 th)
{
	#ifdef 	DEBUG_PB
	dprintf("\r\nPB DNS %d: (tw,th)=(%d,%d)",pb,tw,th);
	#endif

	switch(pb) {
	case 0: 
			WriteP(PB01_TG_XDNS_H,  (ReadP(PB01_TG_XDNS_H)&0xf0) | ((u8)(tw>>8)));
			WriteP(PB0_TG_XDNS_L, (u8)tw);
			WriteP(PB01_TG_YDNS_H,  (ReadP(PB01_TG_YDNS_H)&0xf0) | ((u8)(th>>8)));
			WriteP(PB0_TG_YDNS_L, (u8)th);
	break;
	case 1: 
			WriteP(PB01_TG_XDNS_H,  (ReadP(PB01_TG_XDNS_H)&0x0f) | ((u8)((tw>>4)&0xf0)));
			WriteP(PB1_TG_XDNS_L, (u8)tw);
			WriteP(PB01_TG_YDNS_H,  (ReadP(PB01_TG_YDNS_H)&0x0f) | ((u8)((th>>4)&0xf0)));
			WriteP(PB1_TG_YDNS_L, (u8)th);
	break;
	case 2: 
			WriteP(PB23_TG_XDNS_H,  (ReadP(PB23_TG_XDNS_H)&0xf0) | ((u8)(tw>>8)));
			WriteP(PB2_TG_XDNS_L, (u8)tw);
			WriteP(PB23_TG_YDNS_H,  (ReadP(PB23_TG_YDNS_H)&0xf0) | ((u8)(th>>8)));
			WriteP(PB2_TG_YDNS_L, (u8)th);
	break;
	case 3: 
			WriteP(PB23_TG_XDNS_H,  (ReadP(PB23_TG_XDNS_H)&0x0f) | ((u8)((tw>>4)&0xf0)));
			WriteP(PB3_TG_XDNS_L, (u8)tw);
			WriteP(PB23_TG_YDNS_H,  (ReadP(PB23_TG_YDNS_H)&0x0f) | ((u8)((th>>4)&0xf0)));
			WriteP(PB3_TG_YDNS_L, (u8)th);
	break;
	case 4: 
			WriteP(PB4_SRTG_XDNS_H,  (ReadP(PB4_SRTG_XDNS_H)&0x0f) | ((u8)((tw>>4)&0xf0)));
			WriteP(PB4_TG_XDNS_L, (u8)tw);
			WriteP(PB4_SRTG_YDNS_H,  (ReadP(PB4_SRTG_YDNS_H)&0x0f) | ((u8)((th>>4)&0xf0)));
			WriteP(PB4_TG_YDNS_L, (u8)th);
	break;
	}

}

void PbPortDnsSource(u8 pb, u16 sw, u16 sh)
{
	#ifdef 	DEBUG_PB
	dprintf("\r\nPB DNS %d: (sw,sh)=(%d,%d)",pb,sw,sh);
	#endif

	switch(pb) {
	case 0: 
			WriteP(PB01_SR_XDNS_H,  (ReadP(PB01_SR_XDNS_H)&0xf0) | ((u8)(sw>>8)));
			WriteP(PB0_SR_XDNS_L, (u8)sw);
			WriteP(PB01_SR_YDNS_H,  (ReadP(PB01_SR_YDNS_H)&0xf0) | ((u8)(sh>>8)));
			WriteP(PB0_SR_YDNS_L, (u8)sh);
	break;
	case 1: 
			WriteP(PB01_SR_XDNS_H,  (ReadP(PB01_SR_XDNS_H)&0x0f) | ((u8)((sw>>4)&0xf0)));
			WriteP(PB1_SR_XDNS_L, (u8)sw);
			WriteP(PB01_SR_YDNS_H,  (ReadP(PB01_SR_YDNS_H)&0x0f) | ((u8)((sh>>4)&0xf0)));
			WriteP(PB1_SR_YDNS_L, (u8)sh);
	break;
	case 2: 
			WriteP(PB23_SR_XDNS_H,  (ReadP(PB23_SR_XDNS_H)&0xf0) | ((u8)(sw>>8)));
			WriteP(PB2_SR_XDNS_L, (u8)sw);
			WriteP(PB23_SR_YDNS_H,  (ReadP(PB23_SR_YDNS_H)&0xf0) | ((u8)(sh>>8)));
			WriteP(PB2_SR_YDNS_L, (u8)sh);
	break;
	case 3: 
			WriteP(PB23_SR_XDNS_H,  (ReadP(PB23_SR_XDNS_H)&0x0f) | ((u8)((sw>>4)&0xf0)));
			WriteP(PB3_SR_XDNS_L, (u8)sw);
			WriteP(PB23_SR_YDNS_H,  (ReadP(PB23_SR_YDNS_H)&0x0f) | ((u8)((sh>>4)&0xf0)));
			WriteP(PB3_SR_YDNS_L, (u8)sh);
	break;
	case 4: 
			WriteP(PB4_SRTG_XDNS_H,  (ReadP(PB4_SRTG_XDNS_H)&0xf0) | ((u8)(sw>>8)));
			WriteP(PB4_SR_XDNS_L, (u8)sw);
			WriteP(PB4_SRTG_YDNS_H,  (ReadP(PB4_SRTG_YDNS_H)&0xf0) | ((u8)(sh>>8)));
			WriteP(PB4_SR_YDNS_L, (u8)sh);
	break;
	}

}

void PbPortDnScaleBypassEnable(u8 pb, u8 on)
{
	u8 val;

	if( pb == 4 ) {
		if(on==1)
			WriteP(PB5_CTRL, ReadP(PB5_CTRL) | 0x80);
		else
			WriteP(PB5_CTRL, ReadP(PB5_CTRL) & 0x7f);
	}
	else {
		val = ReadP(PB_CTRL_MODE) & ~(0x01<<pb);
		WriteP(PB_CTRL_MODE, val|(on<<pb));
	}

}

void Pb16bitInputEnable(u8 pb, u8 on)
{
	u8 val1;

	switch(pb){
	case 0:
	case 1:
	case 2:
	case 3:
		val1 = ReadP(PB_CTRL_MISC);
		WriteP(PB_CTRL_MISC, ((val1&~(0x01<<pb))|(on<<pb)));
		break;
	case 4:
		val1 = ReadP(PB_CTRL_MISC1);
		WriteP(PB_CTRL_MISC1, (val1&0xfd)|(on<<1));
		break;
	}
}

//-----------------------------------------------------------------------------
// Playback Channel Decode 
//-----------------------------------------------------------------------------

void SetDecChid(u8 pb, _ChIdDec chdec)
{
	u8 val;
	if(pb>4) return;

	#ifdef DEBUG_PB
	dprintf("\r\nSetDecChid: %d", pb);
	#endif
	WriteP(PB0_CHID_DEC+pb*5, chdec.ctrl);
	WriteP(PB0_VOS+pb*5, ((ReadP(PB0_VOS+pb*5) & 0xe0)) | chdec.vos);  
	WriteP(PB0_FOS+pb*5, chdec.fos|(chdec.pixel_w<<4));
	WriteP(PB0_HOS+pb*5, chdec.hos);
	WriteP(PB0_MID+pb*5, chdec.mid_val);

	switch( pb ) {
	case 0: val = ReadP(PB10_VSIZE)&0xf0;
			WriteP(PB10_VSIZE, val|chdec.vsize);
			break;
	case 1: val = ReadP(PB10_VSIZE)&0x0f;
			WriteP(PB10_VSIZE, val|(chdec.vsize<<4));
			break;
	case 2: val = ReadP(PB32_VSIZE)&0xf0;
			WriteP(PB32_VSIZE, val|chdec.vsize);
			break;
	case 3: val = ReadP(PB32_VSIZE)&0x0f;
			WriteP(PB32_VSIZE, val|(chdec.vsize<<4));
			break;
	}
}

void SetChidDecInit(void)
{
	u8 pb;

	for(pb=0; pb<4; pb++)
		SetDecChid(pb, Default_Chid_Dec);

}


/*! 
 * \brief
 *    Set PB Internal Loop Back On/OFF
 *
 * \param [in] id  : chip no
 * \param [in] on  : 1=ON, 0=OFF
 *
 * \return
 *        <NONE> 
 * \note
 *    
 */
void TWMux_SetPBLoopback(u8 id, int on)
{
	if(on) {
			WriteP(0x224, 0x48); 
			WriteP(0x225, 0x5a); 
			WriteP(0x226, 0x6c); 
			WriteP(0x227, 0x7e); 
	}
	else {
		//Normal Mode
			WriteP(0x224, 0x00); 
			WriteP(0x225, 0x19); 
			WriteP(0x226, 0x2a); 
			WriteP(0x227, 0x3b); 
	}

}

/*! 
 * \brief
 *    Get current PB Source type 
 *
 * \param [in] id  : chip no
 * \param [in] pb  : pb no
 *
 * \return
 *        PB Source type for pb 
 * \note
 *    
 */
_IN_Type TWMux_GetPBInType(u8 id, int pb)
{
	return tw2828devs[id].ipbtype[pb];	

}

/*! 
 * \brief
 *    Set PB YC Swap
 *
 * \param [in] id 
 * \param [in] pb 
 * \param [in] swap
 * 
 * \return
 *        <NONE> 
 */
void TWMux_SetPBYCSwap(u8 id, int pb, int swap)
{
	u8 val;
  
 	WriteP(PB_CTRL_MISC, ReadP(PB_CTRL_MISC)|0x80);

	val = ReadP(PB_CTRL_FLD) & ~(0x01<<pb);
	WriteP(PB_CTRL_FLD, val | (swap<<pb));
}

/*! 
 * \brief
 *    Set PB Source type into pb port
 *
 * \param [in] no   :  chip number
 * \param [in] pb   :  Playback Port Number
 * \param [in] mode :  Input Source Type
 * \param [in] EmChId :  Embedded Channel Id in blanking area
 * \param [in] YCSwap :  YC swap for 16bit mode
 * 
 * \return
 *        <NONE> 
 * \note
 *        (1) xxx_720 or xxx_960 setting expect 8bit BT-656 input  \n
 *              Also REC Function will turn on. Forced to use YCSwap=0, EmChId=1 in the function.\n
 *        (2) xxx_4D1 setting expect 16bit BT-1120 input  \n
 *              Also REC Function will turn on.         \n 
 *        (3) HD_xxx setting expect 16bit BT-1120 input  \n
 *              Display BT1120 output mode will turn on.         
 */
void TWMux_SetPBSource(u8 no, int pb, _IN_Type mode, int EmChId, int YCSwap)
{
	tw2828devs[no].ipbtype[pb] = mode;	
	tw2828devs[no].ipbp[pb].EmChId = EmChId;
	tw2828devs[no].ipbp[pb].YCSwap = YCSwap;
	
	switch(mode) {
	case NTSC_720: tw2828devs[no].ipbp[pb].IHR = 720;	tw2828devs[no].ipbp[pb].IVR = 240;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case PAL_720:  tw2828devs[no].ipbp[pb].IHR = 720;	tw2828devs[no].ipbp[pb].IVR = 288;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case NTSC_960: tw2828devs[no].ipbp[pb].IHR = 960;	tw2828devs[no].ipbp[pb].IVR = 240;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case PAL_960:  tw2828devs[no].ipbp[pb].IHR = 960;	tw2828devs[no].ipbp[pb].IVR = 288;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case NTSC_4D1: tw2828devs[no].ipbp[pb].IHR = 1440;	tw2828devs[no].ipbp[pb].IVR = 480;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case PAL_4D1:  tw2828devs[no].ipbp[pb].IHR = 1440;	tw2828devs[no].ipbp[pb].IVR = 576;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case HD_1080I: tw2828devs[no].ipbp[pb].IHR = 1920;	tw2828devs[no].ipbp[pb].IVR = 540;	tw2828devs[no].ipbp[pb].Interlaced=1; break;
	case HD_720P:  tw2828devs[no].ipbp[pb].IHR = 1280;	tw2828devs[no].ipbp[pb].IVR = 720;	tw2828devs[no].ipbp[pb].Interlaced=0; break;
	case HD_1080P: tw2828devs[no].ipbp[pb].IHR = 1920;	tw2828devs[no].ipbp[pb].IVR = 1080; tw2828devs[no].ipbp[pb].Interlaced=0;break;
	case SD_480I:  tw2828devs[no].ipbp[pb].IHR = 720;	  tw2828devs[no].ipbp[pb].IVR = 240;  tw2828devs[no].ipbp[pb].Interlaced=1;break;
	case SD_576I:  tw2828devs[no].ipbp[pb].IHR = 720;	  tw2828devs[no].ipbp[pb].IVR = 288;  tw2828devs[no].ipbp[pb].Interlaced=1;break;
	case SD_480P:  tw2828devs[no].ipbp[pb].IHR = 720;	  tw2828devs[no].ipbp[pb].IVR = 480;  tw2828devs[no].ipbp[pb].Interlaced=0;break;
	case SD_576P:  tw2828devs[no].ipbp[pb].IHR = 720;	  tw2828devs[no].ipbp[pb].IVR = 576;  tw2828devs[no].ipbp[pb].Interlaced=0;break;
	case VGA_640x480:  tw2828devs[no].ipbp[pb].IHR = 640;	  tw2828devs[no].ipbp[pb].IVR = 480;  tw2828devs[no].ipbp[pb].Interlaced=0;break;
	case VGA_1024x768: tw2828devs[no].ipbp[pb].IHR = 1024;	tw2828devs[no].ipbp[pb].IVR = 768;  tw2828devs[no].ipbp[pb].Interlaced=0;break;
	case VGA_1280x768: tw2828devs[no].ipbp[pb].IHR = 1280;	tw2828devs[no].ipbp[pb].IVR = 768;  tw2828devs[no].ipbp[pb].Interlaced=0;break;
	default:
		printk("\n[%d] PB%d No Input or Wrong Input!!", no, pb);
		return;
	}
 
  //force to control
  if(mode <= PAL_960) YCSwap=0;
  if(mode <= PAL_4D1) EmChId=1;

	SetChidDecInit();

  PbPortDnsSource(pb, tw2828devs[no].ipbp[pb].IHR, tw2828devs[no].ipbp[pb].IVR);

   WriteP(PB_FLD_SEL, 0x0f);
   WriteP(PB5_CTRL, ReadP(PB5_CTRL) | 0x40); //FLD_SEL_PB5

	 WriteP(0x23d, 0x44); // LP_Cnt PB_xx5 = PB5
	 
	 //field embedded
   if(EmChId==1){
	   if(pb==4) WriteP(PB5_CTRL, ReadP(PB5_CTRL) | 0x0c); 
	   else WriteP(PB_FLD_CTRL, ReadP(PB_FLD_CTRL) & ~(0x10<<pb));
   }
   else {
   	if(tw2828devs[no].ipbp[pb].Interlaced==1){
	   if(pb==4) WriteP(PB5_CTRL, ReadP(PB5_CTRL) | 0x0c); 
	   else WriteP(PB_FLD_CTRL, (ReadP(PB_FLD_CTRL)|0xf0) | (0x01<<pb));
    }
    else {
	   if(pb==4) WriteP(PB5_CTRL, (ReadP(PB5_CTRL) & 0xf7) | 0x04); 
	   else WriteP(PB_FLD_CTRL, (ReadP(PB_FLD_CTRL)|0xf0) & ~(0x01<<pb));

   }
  }

  //data_bit
   if(mode >= NTSC_4D1)
	   Pb16bitInputEnable(pb, ON);
	 else  
	   Pb16bitInputEnable(pb, OFF);

  //yc swap
  if(YCSwap==1)
     WriteP(PB_CTRL_FLD, ReadP(PB_CTRL_FLD)|(0x01<<pb)); //YC SWAP Enable
  else   
     WriteP(PB_CTRL_FLD, ReadP(PB_CTRL_FLD)&~(0x01<<pb)); //YC SWAP Disable
  
   if(mode >= HD_1080I){
	   WriteP(0x3ff, ReadP(0x3ff) | 0x60);
	   
	   if(pb==4) WriteP(PB_FRC_CTRL_2, ReadP(PB_FRC_CTRL_2) | 0x80);
	   else	WriteP(PB_CHID_INSERT, ReadP(PB_CHID_INSERT)|(0x01<<pb));	

	   //if(GET_OPTION(IO_OPTN2)) //???
	   { //HD mode 
				WriteP(0x21f, ReadP(0x21f) & 0x9f);				 
				WriteP(0x24c, 0x00); //GPIO=INPUT
				WriteP(0x24d, 0x00);
		 }
   }
   else { 	
  	 WriteP(0x3ff, ReadP(0x3ff) & 0x9f);

	   if(pb==4) WriteP(PB_FRC_CTRL_2, ReadP(PB_FRC_CTRL_2) & 0x7f);
	   else	WriteP(PB_CHID_INSERT, ReadP(PB_CHID_INSERT)&~(0x01<<pb));	
	   
	   //if(GET_OPTION(IO_OPTN2)==0) //???
	   { //REC mode 
				WriteP(0x21f, ReadP(0x21f) | 0x60);				 
				WriteP(0x24c, 0xff); //GPIO=OUT
				WriteP(0x24d, 0xff);
		 }

   }

	 WriteP(PB_CTRL_MISC, ReadP(PB_CTRL_MISC)|0x80);
   WriteP(PB_CTRL_FLD, (ReadP(PB_CTRL_FLD)&0x1f)|(pb<<5));
	 WriteP(PB_NSTD_LIMIT_UPPER_L, (u8)(tw2828devs[no].ipbp[pb].IVR+20));
	 WriteP(PB_NSTD_LIMIT_UPPER_H, (u8)((tw2828devs[no].ipbp[pb].IVR+20)>>8));
	 WriteP(PB_NSTD_LIMIT_LOWER_L, (u8)(tw2828devs[no].ipbp[pb].IVR-20));
	 WriteP(PB_NSTD_LIMIT_LOWER_H, (u8)((tw2828devs[no].ipbp[pb].IVR-20)>>8));

   printk("\nPB %d = %s(%d) IHR=%d IVR=%d", pb, InTypeStr[mode], mode, tw2828devs[no].ipbp[pb].IHR,tw2828devs[no].ipbp[pb].IVR);


}


void TWMux_SetPBInType(u8 no, int pb, _IN_Type mode)
{
	int EmChId, YCSwap;
	
	EmChId = tw2828devs[no].ipbp[pb].EmChId;
	YCSwap = tw2828devs[no].ipbp[pb].YCSwap;
	
	TWMux_SetPBSource(no, pb, mode, EmChId, YCSwap);
}


/** @} */ // end of group PB

