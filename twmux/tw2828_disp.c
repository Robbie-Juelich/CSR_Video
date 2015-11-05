
/*! 
 \file  disp.c
 
 \brief
    Main display path channel windows control, output timing, Upscale, Zoom

 
 \date
    02/20/2012
    
 \par Description: 
     
     
 \author
    - Heejeong Ryu  <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 */

#include <linux/string.h> 
#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_reg.h"
#include "tw2828_twmux.h"



_ChannelInfo chDB[TW2828_MAX][36];

#define ChannelWindowEn(a)	WriteP( (WIN0_CTRL+a), ReadP(WIN0_CTRL+a) | 0x04)
#define ChannelWindowDis(a) WriteP( (WIN0_CTRL+a), ReadP(WIN0_CTRL+a) & 0xfb)
#define Cascade_Channel_En(pb) WriteP(CAS_CH_EN, ReadP(CAS_CH_EN)|(0x01<<(pb+4)))
#define Cascade_Channel_Dis(pb) WriteP(CAS_CH_EN, ReadP(CAS_CH_EN)&~(0x01<<(pb+4)))

#define MainWin37FreezeEn()	WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL) | 0x04) //Added freeze
#define MainWin37BoundryEn()		WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL) | 0x02)
#define MainWin37BoundryDis()	WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL) & 0xfd)

#define SetRGB_PB(on)		WriteP(RGB_MODE, (ReadP(RGB_MODE)&0xfe) | on)
#define SetAutoChidInsert(en)           WriteP(RGB_MODE, (ReadP(RGB_MODE)&0xfd) | (0x01<<en))

extern void SetRGBMode(u8 pbmode);
extern void SetPBInputCropStart(u8 pb, u8 nQuad, u8 x, u8 y);
extern void PB_auto_channel_enable(u8 pb);
extern void PB_auto_channel_disable(u8 pb);
extern void PbPortDnScaleBypassEnable(u8 pb, u8 on);
extern void SingleBoxOff(u8 a);
extern void MakeMainUpScaleScreen(_ups *s, u32 phr, u32 pvr, u32 sw, u32 sh);
extern void Reset_rgb_interface(void);
extern void Reset_frsc(void);
extern void MainUpscale(u16 h, u16 v);
extern void MainPosUpWindow(u16 h, u16 v);
extern void DISize(u16 h, u16 v);
extern void MainUpscaleWindow(u16 h, u16 v);
extern void EnablePosUpScale(void);
extern void PbPortDnsTarget(u8 pb, u16 tw, u16 th);
extern void SetInputCropStart(u8 win, u8 x, u8 y);
void Select_LIVE_OR_PB(u8 ch);


/*! @defgroup Display /tw2828API/Display 
*   @ingroup tw2828API
*/
/*!
\addtogroup Display
\{
*/

void MainWin37En(void)
{
	WriteP( (WIN37_CTRL), 0x01); 
	MainWin37BoundryEn();
	MainWin37FreezeEn(); // Keep the order.
}

void MainWin37Dis(void)
{
		WriteP2( (WIN37_CTRL), ReadP(WIN37_CTRL) & 0xf8);
}


void MainMemoryStart(u32 x, u32 y)
{
	WriteP(MNHSTART_L, (u8)x);
	WriteP(MNHSTART_H, (u8)(x>>8));
	WriteP(MNVSTART_L, (u8)y);
	WriteP(MNVSTART_H, (u8)(y>>8));
}

/*! 
 * \brief
 *    Main Display Boundry Enable for each channel 
 *
 * \param [in] chWindow
 *
 * \return
 *    <NONE>
 * \note
 *    
 */
void SetMainBoudaryChannelEnable(_ChannelNo chWindow)
{
  u16 val=0;
  u8 ch=0, pb=0;	
	
  switch(chWindow) {
  case CH1...CH16: ch=chWindow; pb=0; break;
  case PB1...PB5: ch=chWindow-PB1; pb=1; break;
  case CHx1...CHx16: ch=chWindow-CHx1; pb=0; break;
  case PBx1...PBx16: ch=chWindow-PBx1; pb=1; break;
  case CAS1...CAS4: ch=chWindow-CAS1;  
    val = 0x01<<ch;
		WriteP(BND_PB_CH_EN_4, ReadP(BND_PB_CH_EN_4)|(u8)val);
  	return;
  }
   val = 0x01<<ch;
  
  if(pb==0)
	{
	  if(ch<8)
			WriteP(BND_LV_CH_EN_0, ReadP(BND_LV_CH_EN_0)|(u8)val);
		else
			WriteP(BND_LV_CH_EN_1, ReadP(BND_LV_CH_EN_1)|(u8)(val>>8));
	}
	else {
	  if(ch<8)
			WriteP(BND_PB_CH_EN_2, ReadP(BND_PB_CH_EN_2)|(u8)val);
		else
			WriteP(BND_PB_CH_EN_3, ReadP(BND_PB_CH_EN_3)|(u8)(val>>8));
	}
}

/*! 
 *************************************************************************************
 * \brief
 *    Main Display Boundry Disable for each channel 
 *
 * \param ch      
 *
 * \return
 *    <NONE>
 * \note
 *
 *************************************************************************************
 */
void SetMainBoudaryChannelDisable(u8 i)
{
  u16 val=0;
  u8 ch=0, pb=0;	
	
  switch(i) {
  case CH1...CH16: ch=i; pb=0; break;
  case PB1...PB5: ch=i-PB1; pb=1; break;
  case CHx1...CHx16: ch=i-CHx1; pb=0; break;
  case PBx1...PBx16: ch=i-PBx1; pb=1; break;
  case CAS1...CAS4: ch=i-CAS1;  
    val = 0x01<<ch;
		WriteP(BND_PB_CH_EN_4, ReadP(BND_PB_CH_EN_4) & ~(u8)val);
  	return;
  }

   val = 0x01<<ch;

  if(pb==0)
	{
	    if(ch<8)
			WriteP(BND_LV_CH_EN_0, ReadP(BND_LV_CH_EN_0) & ~((u8)val));
		else
			WriteP(BND_LV_CH_EN_1, ReadP(BND_LV_CH_EN_1) & ~((u8)(val>>8)));

	}
	else {
	    if(ch<8)
			WriteP(BND_PB_CH_EN_2, ReadP(BND_PB_CH_EN_2) & ~((u8)val));
		else
			WriteP(BND_PB_CH_EN_3, ReadP(BND_PB_CH_EN_3) & ~((u8)(val>>8)));
	}
}

/*! 
 * \brief
 *    Main Display All Boundry Forced Disable 
 *
 * \param chWindow
 *    _ChannelNo defined at disp.h
 *
 * \return
 *    <NONE>
 * \note
 *    
 */
void AllBoudaryChannelDisable(void)
{
	WriteP(BND_LV_CH_EN_0, 0x00);
	WriteP(BND_LV_CH_EN_1, 0x00);

	WriteP(BND_PB_CH_EN_2, 0x00);
	WriteP(BND_PB_CH_EN_3, 0x00);
	WriteP(BND_PB_CH_EN_4, 0x00);

	MainWin37BoundryDis();
}

void SetCroppingStartUnit(u8 ux, u8 uy)
{
	if(ux==2) 
		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)|0x02);
	else
		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)&~0x02);
	
	if(uy==4) 
		WriteP(FIFO_LIMIT_CTRL, ReadP(FIFO_LIMIT_CTRL)|0x01);
	else
		WriteP(FIFO_LIMIT_CTRL, ReadP(FIFO_LIMIT_CTRL)&~0x01);
	
}

void  InitMainWindow(u8 winno, u16 x, u16 y, u16 w, u16 h)
{
	if((winno>=CAS1) && (winno<=CAS4)){
		winno = winno - CAS1 + 12;
		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)|0x04);
	}
	else if(winno==PB5){
		winno = 12;
		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)|0x04);
	}
	
	y += DISPLAYBASE;
	
		WriteP((WIN0_HPOSI_H+winno*2), (u8)(x>>8));
		WriteP((WIN0_HPOSI_L+winno*2), (u8)x);

		WriteP((WIN0_VPOSI_H+winno*2), (u8)(y>>8));
		WriteP((WIN0_VPOSI_L+winno*2), (u8)y);

		WriteP((WIN0_HSIZE_H+winno*2), (u8)(w>>8));
		WriteP((WIN0_HSIZE_L+winno*2), (u8)w);

		WriteP((WIN0_VSIZE_H+winno*2), (u8)(h>>8));
		WriteP((WIN0_VSIZE_L+winno*2), (u8)h);

		WriteP(SYSTEM_MISC_CTRL, ReadP(SYSTEM_MISC_CTRL)&~0x04);

}

void LivePortDnscale(u8 winno, u32 h, u32 v)
{
	if( h == 0x10000 ) {
		WriteP((WIN0_XDNS_H+winno*2), 0xff);
		WriteP((WIN0_XDNS_L+winno*2), 0xff);
	}
	else
	{
		WriteP((WIN0_XDNS_H+winno*2), (u8)(h>>8));
		WriteP((WIN0_XDNS_L+winno*2), (u8)h);
	}

	if( v == 0x10000 ) {
		WriteP((WIN0_YDNS_H+winno*2), 0xff);
		WriteP((WIN0_YDNS_L+winno*2), 0xff);
	}
	else
	{
		WriteP((WIN0_YDNS_H+winno*2), (u8)(v>>8));
		WriteP((WIN0_YDNS_L+winno*2), (u8)v);
	}

}


void  InitMainWin37(u16 x, u16 y, u16 w, u16 h)
{
		WriteP((WIN37_HPOSI_L), (u8)x);
		WriteP((WIN37_VPOSI_L), (u8)y);
		WriteP((WIN37_POSI_H), (u8)( ((x>>4)&0x30) | ((y>>8)&0x0f) ));

		WriteP((WIN37_HSIZE_L), (u8)w);
		WriteP((WIN37_VSIZE_L), (u8)h);
		WriteP((WIN37_SIZE_H), (u8)( ((w>>4)&0x10) | ((h>>8)&0x07) ));
}


void BypassDownScale(u8 port)
{
	if(port <= PORT_LIVE_15) 
	  LivePortDnscale(port, 0xffff, 0xffff);
	else{
		PbPortDnScaleBypassEnable((port-PORT_PB_1), 1);
	}
}

void ChannelWindowOn(u8 win, u8 ch, u8 port)
{
		if((ch>=PBx1) && (ch<=PBx16)){
			PB_auto_channel_enable(ch-PBx1);
			ChannelWindowEn((port&0x3f)-PORT_PB_1+16);
			SetRGB_PB(1);

		    #ifdef DEBUG_MDSP
			printk("\r\nEn %d-PBx%d-Win%d", win, ch-PBx1, ((port&0x3f)-PORT_PB_1)+16);
			#endif
		}
		else if((ch>=CHx1) && (ch<= CHx16)){
			ChannelWindowEn(ch-CHx1);
			SetRGB_PB(0);

			#ifdef DEBUG_MDSP
			printk("\r\nEn %d-Win%d", win, ch-CHx1);
			#endif
		}
		else if((ch>=CAS1) && (ch<= CAS4)){
			WriteP(CAS_WIN_EN, ReadP(CAS_WIN_EN)|0x04);
			Cascade_Channel_En(ch-CAS1);
		}
		else if(ch==PB5){
			WriteP(CAS_WIN_EN, ReadP(CAS_WIN_EN)|0x04);
	 	  SetRGB_PB(0);
		}
		else {
			ChannelWindowEn(ch);
  		SetRGB_PB(0);
		}
	

}

void ChannelWindowOff(u8 id, _ChannelNo ch)
{
		if(ch==PB5){
			WriteP(CAS_WIN_EN, ReadP(CAS_WIN_EN)&~0x04);
		}
		else if((ch>=PBx1) && (ch<= PBx16)){
			PB_auto_channel_disable(ch-PBx1);
		}
		else if((ch>=CAS1) && (ch<= CAS4)){
			Cascade_Channel_Dis(ch-CAS1);
		}
		else if((ch>=CHx1) && (ch<= CHx16)){
			ChannelWindowDis(ch-CHx1);
		}
		else
			ChannelWindowDis(ch);
			
		SetMainBoudaryChannelDisable(ch);
}

u8 GetWriteBufNo(u8 i)
{
	if((i>=CHx1) && (i<=CHx16)) {
		i = i-CHx1;
	}
	else if((i>=PBx1) && (i<=PBx16)) { 
		i = i-PBx1;
	}
	return i;
}

u8 GetWriteWinNo(u8 i)
{
	if((i>=CHx1) && (i<=CHx16)) {
		i = i-CHx1;
	}
	else if((i>=PBx1) && (i<=PBx16)) { 
		i = i-PBx1+16;
	}
	else if((i>=CAS1) && (i<=CAS4)) {
		i = i-CAS1+32;
	}

	return i;
}

u8 IsInputfromPbPort(u8 i)
{
	u8 ret = 0;
	if((i>=PB1) && (i<=PB5)) {
		ret = 1;
	}
	else if((i>=PBx1) && (i<=PBx16)) { 
		ret = 1;
	}
	else if((i>=CAS1) && (i<=CAS4)) { 
		ret = 1;
	}

	return ret;
}

extern void UpdateOSD(u8 winno, u32 ch);
void SetVIChno(u8 winno, u32 ch)
{

	if(winno%2) {
		WriteP(WINCHSEL_1_0+winno/2, (ReadP(WINCHSEL_1_0+winno/2)&0x0f)|(ch<<4));
	}
	else {
		WriteP(WINCHSEL_1_0+winno/2, (ReadP(WINCHSEL_1_0+winno/2)&0xf0)|ch);
	}
//`	UpdateOSD(winno, ch);
}

//-------------------------------------------------------
/*! 
 * \brief
 *    Set Live Channel No for a Window, in DNS module
 *
 * \param [in] id 
 * \param [in] winno
 * \param [in] ch
 */
void TWMux_SetVIChno(u8 id, u32 winno, u32 ch)
{
	printk("\n[TW2828MUX] SetVIChno: %d -- %d", winno, ch);
	
	if(IsInputfromPbPort(winno)) return; 
	
	winno = GetWriteWinNo(winno);
	tw2828devs[id].pChannelDB[winno].ch = ch;
		 
	SetVIChno(winno, ch);
}

//-------------------------------------------------------
/*! 
 * \brief
 *    Set One Channel Window
 *
 * \param [in] id 
 * \param [in] *pChannelDB
 * 
 * \return
 *        <NONE> 
 * \par   Description:
 *        Set Channel window size, position, Downscaler, Cropping starts  \n
 * \note
 *        =Set Down scaler= \n
 *        -Live DNS :       \n  
 *        xr = (w<<2) * 65536 / IHR \n
 *        yr = h * 65536 / IVR \n
 *        -PB DNS : \n
 *        Source window, Target window \n
 */
void TWMux_SetMainChannel(u8 id, _ChannelInfo *CInfo)
{
	u32 xr, yr, x, y,w, h;
	u8 pb, i, p, pbauto;

	i = GetWriteWinNo(CInfo->chno);
	memcpy( &tw2828devs[id].pChannelDB[i], CInfo, sizeof(_ChannelInfo));

	i = GetWriteBufNo(CInfo->chno);
	pb = IsInputfromPbPort(CInfo->chno);
	p = CInfo->port;
	x = CInfo->ix;
	y = CInfo->iy;
	w = CInfo->iw;
	h = CInfo->ih;
  
	printk("\n[%d] - %d(%d):%d-- %xh- %d,%d,%d,%d - %d,%d", id,CInfo->chno,CInfo->ch,pb,CInfo->ctrl,x,y,w,h, CInfo->tw, CInfo->th);

	if( (CInfo->ctrl & MAINON) == MAINON ) {
		if( (CInfo->ctrl & AUTOMODEON) == AUTOMODEON)  { SetRGBMode(1); pbauto=1;}
		else   { SetRGBMode(0); pbauto=0; }

		if( (CInfo->ctrl & INSERTCHID) == INSERTCHID)  
			SetAutoChidInsert(1);	// Enable insertion of Channel ID
		else 
			SetAutoChidInsert(0);	// Disable insertion of Channel ID

		Select_LIVE_OR_PB(CInfo->chno);
		InitMainWindow(i, x, y, w, h);
	
		if( pb == 0) { //live
			xr = (w<<2) * 65536 / tw2828devs[id].ilvp.IHR;
			yr = h * 65536 / tw2828devs[id].ilvp.IVR;
			printk("\nSetMainChannel - winno[%02x] ch[%02x] - xr,yr(%04x,%04x)", i, CInfo->ch, xr, yr);
			LivePortDnscale(i, xr, yr);
			SetVIChno(i, CInfo->ch);
			SetCroppingStartUnit(CInfo->cx_unit, CInfo->cy_unit); 
			SetInputCropStart(i, CInfo->cx, CInfo->cy);
		}
		else { //pb
      
			PbPortDnScaleBypassEnable(p-PORT_PB_1, OFF);

			if(tw2828devs[id].ipbp[p-PORT_PB_1].Interlaced == 0)
				PbPortDnsTarget(p-PORT_PB_1, CInfo->tw*4, CInfo->th);
			else
				PbPortDnsTarget(p-PORT_PB_1, CInfo->tw*4, CInfo->th/2);

			SetCroppingStartUnit(CInfo->cx_unit, CInfo->cy_unit); 
			SetInputCropStart(i, CInfo->cx, CInfo->cy);

 	  	} 
 	  
		SetMainBoudaryChannelEnable(CInfo->chno); 
		ChannelWindowOn(i, CInfo->chno, CInfo->port&0x3f);		
	}
	else {
		ChannelWindowOff(id, CInfo->chno);
	}
}

void Select_LIVE_OR_PB(u8 ch)
{
		if((ch>=PBx1) && (ch<=PBx16)){
			SetRGB_PB(1);

		    #ifdef DEBUG_MDSP
			dprintk("\r\n PB Select");
			#endif

		}
		else if((ch>=CHx1) && (ch<= CHx16)){
			SetRGB_PB(0);

			#ifdef DEBUG_MDSP
			dprintk("\r\n Live Select");
			#endif
		   printk("\r\n Live Select ch: %d\n", ch);
		}
		else if((ch>=CAS1) && (ch<= CAS4)){
			SetRGB_PB(0);

			#ifdef DEBUG_MDSP
			dprintk("\r\n PB5 Auto Select");
			#endif
		}
	  else {
		   SetRGB_PB(0);

	//	   #ifdef DEBUG_MDSP
		   printk("\r\n Live Select ch: %d\n", ch);
	//	   #endif
	  }

}

void TWMux_printChannelDB(u8 id)
{
	int i;
	printk("\n[%d] ---- Display Current Display DB ----", (u16)id);
	
/*	for(i=0; i<20; i++) {
		printk("\n - %02x - %02x - %02x - %02x : ",i,chDB[id][i].chno,chDB[id][i].port,(u8)chDB[id][i].ctrl);
		printk("- %04d - %04d - %04d - %04d -",chDB[id][i].x,chDB[id][i].y,chDB[i].w,chDB[id][i].h);
	}*/

	for(i=0; i<36; i++) {
		printk("\n = %02d - %02x - %02x - %02x : ",i,tw2828devs[id].pChannelDB[i].chno,tw2828devs[id].pChannelDB[i].port,(u8)tw2828devs[id].pChannelDB[i].ctrl);
		printk("- %04d - %04d - %04d - %04d -",tw2828devs[id].pChannelDB[i].x,tw2828devs[id].pChannelDB[i].y,tw2828devs[id].pChannelDB[i].w,tw2828devs[id].pChannelDB[i].h);
	}

}


/*! 
 * \brief
 *    Set Display all of main channel windows
 *
 * \param [in] id 
 * \param [in] *pChannelDB
 * 
 * \return
 *        <NONE> 
 * \note
 *        (1) Create channel window buffer (ix, iy, iw, ih)  on the display memory, \n
 *                    and downscale Input source size (pw, ph)  into (iw, ih) \n
 *        (2) Enable or Disable Display Channel Window. \n
 *        (3) Enable Boundary for enabled channel window 
 */
void TWMux_MakeMainChannelWindows(u8 id, _ChannelInfo *C)
{
	//u8 i, pbauto, checkch=0;
	u8 i,  checkch=0;
	_ups  srn;

	for(i=0; i<36; i++) {
		memcpy( &chDB[id][i], &C[i], sizeof(_ChannelInfo));
		checkch += C[i].ch;
	}

	if(checkch == 0) { // ch errou
		for(i=0; i<36; i++) 
			C[i].ch = C[i].chno; //Default VI ch : VO chno = 1:1 
  		chDB[id][i].ch = C[i].chno;
	}
  
	tw2828devs[id].pChannelDB = (_ChannelInfo *)chDB[id];

	tw2828devs[id].ZoonScreenOnFlag = OFF;

	// Close all
	for(i=CAS1; i<=CAS4; i++)
		ChannelWindowOff(id, i);
	for( i=PBx1; i<=PBx16; i++) 
		ChannelWindowOff(id, i);
	for( i=0; i<20; i++)  // ??  when set it on 
		ChannelWindowOff(id, i);
	ChannelWindowOff(id, PB5);
	MainWin37Dis();

	for( i=0; i<36; i++)
	{
		if( (C[i].ctrl & MAINON) == MAINON ) {
			TWMux_SetMainChannel(id, &C[i] );
		}
	}    

  //printChannelDB(id);
	
	if( (mTimingTbl[tw2828devs[id].mdrs].ctrl & INTERLACE) == INTERLACE) 
	  srn.di = DI_WEAVE;
	else
  srn.di = DI_2D;
  //srn.ups_on = FALSE;
  //srn.id = DI_WEAVE;
  srn.ups_on = TRUE;
  srn.sx  = 0;
  srn.sy  = 0;
  MakeMainUpScaleScreen(&srn, tw2828devs[id].PHR, tw2828devs[id].PVR, tw2828devs[id].UHR, tw2828devs[id].UVR);

	if((ReadP(PB_MODE)&0x08) == 0) { 
		printk("\n cover tearing and stop&go ?????");
     WriteP(0x6FC, 0x20);
     WriteP(0x6E8, 0x00);
     WriteP(0x6E9, 0x00);
     WriteP(0x6EA, 0x00);
     WriteP(0x6EB, 0x00);
     WriteP(0x6EC, 0x00);
     WriteP(0x6ED, 0x00);
     WriteP(0x6EE, 0x00);
     WriteP(0x6EF, 0x00);
     WriteP(0x6F0, 0x00);
     WriteP(0x6F1, 0x00);

     WriteP(0x6FF, 0x00);
     WriteP(0x6F9, 0x00);

     WriteP(0x3DF, 0x60);  // To cover mode change channel missing????
     WriteP(0x3E4, 0x60);
     WriteP(0x3E9, 0x60);
     WriteP(0x3EE, 0x60);
     WriteP(0x3B0, 0x60);

     WriteP(0x3DF, 0x00);
     WriteP(0x3E4, 0x00);
     WriteP(0x3E9, 0x00);
     WriteP(0x3EE, 0x00);
     WriteP(0x3B0, 0x00);

  }

	Reset_rgb_interface(); //Added for TW2828
	Reset_frsc();

}

/*! 
  \brief
		Make Full Screen with chno Window 
	\retval  <NONE>
	\par Decription
       Make full screen with the channel window on the Display Output
*/
void TWMux_MakeFullScreen(int id, u8 winno)
{
	u32  xr, yr;
	u8 pb, i, pbauto;
	u16 pw, ph;
  static u32 phr, pvr;
  static int mdrs;
  _ChannelInfo CInfo;
 
  phr = tw2828devs[id].PHR;
  pvr = tw2828devs[id].PVR - 80; 
  mdrs = tw2828devs[id].mdrs; 
  memcpy(&CInfo, &tw2828devs[id].pChannelDB[winno], sizeof(_ChannelInfo));
 
 if( ( CInfo.ctrl & MAINON ) != MAINON ) 
 	{
 		printk("\n Window #%d is not turned ON. Please check Window status!!",winno );
 		return;	
 	}
 	
	AllBoudaryChannelDisable();
	for( i=0; i<20; i++)  {
			if( ( tw2828devs[id].pChannelDB[i].ctrl & MAINON ) == MAINON ) 
	 				ChannelWindowOff(id, i);
	}
	for( i=PBx1; i<=PBx16; i++) {
			if( ( tw2828devs[id].pChannelDB[i-CHx1].ctrl & MAINON ) == MAINON ) 
	 				ChannelWindowOff(id, i); 
	}

	#ifdef SUPPORT_MDBOX
	TWMux_AllMotionBoxOff(id);
	#endif

	i = GetWriteBufNo(CInfo.chno);
	pb = IsInputfromPbPort(CInfo.chno);
  if( (CInfo.ctrl & AUTOMODEON) == AUTOMODEON)  pbauto=1;
	else  pbauto=0; 

	if(pb==1) {
		pw = tw2828devs[id].ipbp[CInfo.port - PORT_PB_1].IHR;
		ph = tw2828devs[id].ipbp[CInfo.port - PORT_PB_1].IVR;
		if (tw2828devs[id].ipbp[CInfo.port - PORT_PB_1].Interlaced==1 )
					ph *= 2;
	}
	else {
		pw = tw2828devs[id].ilvp.IHR;
		ph = tw2828devs[id].ilvp.IVR;
	}

	if(pb==1) {
	}
	
	printk("\n -(TWMux_MakeFullScreen) %d pb=%d- %d %d - %d %d", winno, pb, pw, ph, phr, pvr);

	if((pbauto==1) && (pb==1)) SetRGB_PB(ON);
	else SetRGB_PB(OFF);

  if((pw>phr) && (ph<pvr) ) {
		pw = phr;
	}
	else if((pw>phr) && (ph>pvr) ) {
		pw = phr;
		ph = pvr;
	}
	else if((pw<phr) && (ph>pvr) ) {
		ph = pvr;
	}

	{
		if( pb == 1 ) {	
			/* Need to work!!!!!!
			if((CurDisplayMode == MODE_4D1_4CUT ) || (CurDisplayMode == MODE_16QUADPB) || \
				(CurDisplayMode == MODE_32CH) || \
				(CurDisplayMode == MODE_6VGA_16CUT) || (CurDisplayMode == MODE_1HD_16CUT || (CurDisplayMode==MODE_1HD_8CUT_8Live)) )
			{
				pw = pw>>1; ph = ph>>1;
			}*/
			
			InitMainWindow(i, 0, 0, pw>>2, ph);
 		}
		else 
			InitMainWindow(i, 0, 0, pw>>2, ph);

		BypassDownScale(tw2828devs[id].pChannelDB[winno].port&0x3f);	 
	}

	MainMemoryStart(0,DISPLAYBASE);

   printk("\r\n -(TWMux_MakeFullScreen) - pw %d  ph %d", pw, ph);


	xr = (pw*4096 + phr/2 )/phr;
	yr = (ph*1024 + pvr/2 )/pvr;

  if(tw2828devs[id].INTERLACED == 1)
  	yr = yr/2;
//    #ifdef DEBUG_MDSP
	printk("\r\n TWMux_MakeFullScreen --- win:%d ch:%d:%d - pw,ph(%d,%d)", winno, pb, i, pw, ph);
//	#endif
	MainUpscale(xr, yr);
    printk("\r\n TWMux_MakeFullScreen --- pos xr,yr(%04x,%04x)", xr, yr);

	xr = (phr*4096 + pw-1 )/pw;
	yr = (pvr*4096 + ph/2 )/ph;
//	#ifdef DEBUG_MDSP
    printk("\r\n TWMux_MakeFullScreen --- pos xr,yr(%04x,%04x)", xr, yr);
//	#endif
	MainPosUpWindow(xr, yr);

	MainUpscaleWindow(phr, pvr);

	DISize(pw,ph/2);

  //weave
  if((xr!=4096) || (yr!=4096)) {
    if((ReadP(DI_ENABLE)&0x01) == 0)
		  WriteP(SEL_2D, ReadP(SEL_2D)|0x08);
  }

	// set baudary
	SetMainBoudaryChannelEnable(CInfo.chno);
	
	ChannelWindowOn(winno,CInfo.chno, CInfo.port&0x3f);
 
	EnablePosUpScale();

	SingleBoxOff(0);

}


/*! 
  \brief
		Load YUV Image into logo window 
	\retval  
	     <NONE>
	\par Update history:
		Mar/08/2012 - Created
	\par Decription
       (1)Load image into DDR memory \n
       (2)Enable logo window \n
*/
void TWMux_SetYUVImageToLogoWindow( int no, int x, int y, int w, int h, u32 addr)
{
#define MainWin37FreezeDis()    WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL)&(~0x04))
	// I add it
//	MainWin37FreezeDis();
//	TWMux_WriteDisplayToMemory(addr, x ,y+DISPLAYBASE);
	TWMux_WriteDisplayToMemory(addr, x ,y+DISPLAYBASE);

	InitMainWin37(x/4,y+DISPLAYBASE,w/4,h);
	MainWin37En();
	MainWin37BoundryEn();
	MainWin37FreezeEn(); // Keep the order.
#if 1
//	MainWin37FreezeDis();
#endif
//	MainWin37FreezeEn(); // Keep the order.

}

/*! 
  \brief
		Turn On one channel window
	\param [in] id    : chip id
	\param [in] ch    : channel id
	\note
	       ChannelWindow DataBase will be updated.
*/
void TWMux_ChannelWindowOn(u8 id, _ChannelNo ch)
{
	  int winno, pbauto;
	  winno = GetWriteWinNo(ch);
	  
		tw2828devs[id].pChannelDB[winno].ctrl |= MAINON; 	
	  if( (tw2828devs[id].pChannelDB[winno].ctrl & AUTOMODEON) == AUTOMODEON)  { pbauto=1;}
		  else   { pbauto=0; }

		ChannelWindowOn(id, ch, tw2828devs[id].pChannelDB[winno].port&0x3f );
}

/*! 
  \brief
		Turn Off one channel window
	\param [in] id    : chip id
	\param [in] ch    : channel id
	\note
	       ChannelWindow DataBase will be updated.
*/
void TWMux_ChannelWindowOff(u8 id, _ChannelNo ch)
{
	  int winno;
	  winno = GetWriteWinNo(ch);
	  
		tw2828devs[id].pChannelDB[winno].ctrl &= ~MAINON; 	
		ChannelWindowOff(id, ch);
}

/*! 
  \brief
		Forced turn Off one channel window 
	\param [in] id    : chip id
	\param [in] ch    : channel id
	\note
	       Only Window will be off. DataBase won't be updated.
*/
void TWMux_ChannelWindowForcedOff(u8 id, _ChannelNo ch)
{
		if(ch==PB5){
			WriteP(CAS_WIN_EN, ReadP(CAS_WIN_EN)&~0x04);
		}
		else if((ch>=PBx1) && (ch<= PBx16)){
			PB_auto_channel_disable(ch-PBx1);
		}
		else if((ch>=CAS1) && (ch<= CAS4)){
			Cascade_Channel_Dis(ch-CAS1);
		}
		else if((ch>=CHx1) && (ch<= CHx16)){
			ChannelWindowDis(ch-CHx1);
		}
		else
			ChannelWindowDis(ch);
			
}

void TWMux_GetChannelDB(u8 id, u8 bufno, _ChannelInfo *Channeldb)
{
	 *Channeldb = tw2828devs[id].pChannelDB[bufno];
}

void TWMux_ResetDisplay(u8 id)
{
	if((ReadP(PB_MODE)&0x08) == 0) { 
		//printk("\n cover tearing and stop&go ?????");
     WriteP(0x6FC, 0x20);
     WriteP(0x6E8, 0x00);
     WriteP(0x6E9, 0x00);
     WriteP(0x6EA, 0x00);
     WriteP(0x6EB, 0x00);
     WriteP(0x6EC, 0x00);
     WriteP(0x6ED, 0x00);
     WriteP(0x6EE, 0x00);
     WriteP(0x6EF, 0x00);
     WriteP(0x6F0, 0x00);
     WriteP(0x6F1, 0x00);

     WriteP(0x6FF, 0x00);
     WriteP(0x6F9, 0x00);

     WriteP(0x3DF, 0x60);  // To cover mode change channel missing????
     WriteP(0x3E4, 0x60);
     WriteP(0x3E9, 0x60);
     WriteP(0x3EE, 0x60);
     WriteP(0x3B0, 0x60);

     WriteP(0x3DF, 0x00);
     WriteP(0x3E4, 0x00);
     WriteP(0x3E9, 0x00);
     WriteP(0x3EE, 0x00);
     WriteP(0x3B0, 0x00);

  }

	//Reset_rgb_interface(); //Added for TW2828
	Reset_frsc();
	
}

/** @} */
