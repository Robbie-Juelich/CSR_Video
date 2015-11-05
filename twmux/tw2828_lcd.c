/*! 
 \file  disp.c
 
 \brief
    Main display LCD setup
 
 \date
    02/20/2012
    
 \par Description: 
     
     
 \author
    - Heejeong Ryu  <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 */

#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_reg.h"
#include "tw2828_twmux.h"


#define SetInterlaced(on)  WriteP(LCDMISCCTRL, (ReadP(LCDMISCCTRL) & 0xfe) | on)

extern void MainMemoryStart(u32 x, u32 y);

void MainUpscale(u16 h, u16 v)
{
		WriteP(XUPS_H, (u8)(h>>8));
		WriteP(XUPS_L, (u8)h);

		WriteP(YUPS_H, (u8)(v>>8));
		WriteP(YUPS_L, (u8)v);
}

void MainUpscaleWindow(u16 h, u16 v)
{
		WriteP(XUPS_WIDTH_H, (u8)(h>>8));
		WriteP(XUPS_WIDTH_L, (u8)h);

		WriteP(YUPS_HEIGHT_H, (u8)(v>>8));
		WriteP(YUPS_HEIGHT_L, (u8)v);
}

void MainPosUpWindow(u16 h, u16 v)
{
		WriteP(POS_XUPS_H, (u8)(h>>8));
		WriteP(POS_XUPS_L, (u8)h);

		WriteP(POS_YUPS_H, (u8)(v>>8));
		WriteP(POS_YUPS_L, (u8)v);
}

void TurnOn2DDI(void)
{
		WriteP(DI_ENABLE, 0x01); // Turn On DI

		//WriteP(SEL_2D, 0x11);	//Select 2D
		WriteP(SEL_2D, 0x25);	//Select 2D + BOB
		//WriteP(0x4cf, 0x14);
		WriteP(DCM_FIFO_CNT, 0x1);
	
}
void TurnOnWeave(void)
{
		WriteP(DI_ENABLE, 0x00); // Turn Off DI
		//WriteP(0x4cf, 0x14);
		WriteP(DCM_FIFO_CNT, 0x4);
		WriteP(SEL_2D, ReadP(SEL_2D)&0xf7);
}

void TurnOnWeaveUpScale(void)
{
	WriteP(SEL_2D, ReadP(SEL_2D) | 0x08);	

}

void EnablePosUpScale(void)
{
		WriteP(POS_UPS_CTRL, 0x00);
		delay(1);
		WriteP(POS_UPS_CTRL, 0x01);
}

void DISize(u16 h, u16 v)
{
		WriteP(DI_WIDTH_H, (u8)(h>>8));
		WriteP(DI_WIDTH_L, (u8)h);

		WriteP(DI_HEIGHT_H, (u8)(v>>8));
		WriteP(DI_HEIGHT_L, (u8)v);

		WriteP(WEAVE_WIDTH_H, (u8)(h>>8));
		WriteP(WEAVE_WIDTH_L, (u8)h);

		WriteP(WEAVE_HEIGHT_H, (u8)(v>>8));
		WriteP(WEAVE_HEIGHT_L, (u8)v);
}

// Main Display Single Box
void SingleBox(u8 a, u16 x0, u16 y0, u16 x1, u16 y1)
{
	WriteP((SBOX0_HL_H+a*2), (u8)(x0>>8));
	WriteP((SBOX0_HL_L+a*2), (u8)x0);

	WriteP((SBOX0_HR_H+a*2), (u8)(x1>>8));
	WriteP((SBOX0_HR_L+a*2), (u8)x1);

	WriteP((SBOX0_VT_H+a*2), (u8)(y0>>8));
	WriteP((SBOX0_VT_L+a*2), (u8)y0);

	WriteP((SBOX0_VB_H+a*2), (u8)(y1>>8));
	WriteP((SBOX0_VB_L+a*2), (u8)y1);
}

void SingleBoxOn(u8 a)
{
	//Used boundary
	WriteP((SBOX0_CTRL+a), 0x10);
}

void SingleBoxOff(u8 a)
{
	//Used boundary
	WriteP((SBOX0_CTRL+a), 0x00);
}

void SingBoxBoudaryColor(u8 r, u8 g, u8 b)
{
	WriteP(SBOX_BCOLOR_R, r);
	WriteP(SBOX_BCOLOR_G, g);
	WriteP(SBOX_BCOLOR_B, b);

}

/*!
  \brief
		Set Display upscale output screen  
	\retval  <NONE>
	\par Update history:
		June/29/2012 - Created
	\par Decription
      (1) Set upscale \n
      (2) Set De-Interlace/Weave \n
      (3) Reset (debug) \n
*/
void TWMux_SetSingleBoxWindowOn(u8 id, u8 no, u8 r, u8 g, u8 b, int bufno)
{
	_ChannelInfo *C;
	 C = &tw2828devs[id].pChannelDB[bufno];
	SingleBox(no, ((C->x<<2)+1), C->y+1, (C->x<<2) + (C->w<<2) - 2, (C->y + C->h) - 2);

	SingBoxBoudaryColor(r, g, b);
	SingleBoxOn(no);
	
}

/*!
  \brief
		Set Display upscale output screen  
	\retval  <NONE>
	\par Update history:
		June/29/2012 - Created
	\par Decription
      (1) Set upscale \n
      (2) Set De-Interlace/Weave \n
      (3) Reset (debug) \n
*/
void MakeMainUpScaleScreen(_ups *s, u32 phr, u32 pvr, u32 sw, u32 sh)
{
	u32  xr=4096, yr=1024;
  
	MainMemoryStart(0, DISPLAYBASE);
  
  if(s->ups_on == TRUE) {
	  xr = (sw*4096 + phr/2 )/phr;
	  yr = (sh*1024 + pvr/2 )/pvr;
 // 	#ifdef DEBUG_MDSP
			printk("\n MakeMainUpScaleScreen --- xr,yr(%04x,%04x)", xr, yr);
//		#endif
  }

  if( s->di == DI_WEAVE)
		MainUpscale(xr, yr/2);
	else
			MainUpscale(xr, yr);

	printk("\n --- xr,yr(%04x,%04x) di_on=%d, di=%d ", xr, yr, s->ups_on, s->di );

  if(s->ups_on == TRUE) {
		xr = (phr*4096 + sw-1 )/sw;
		yr = (pvr*4096 + sh/2 )/sh;
		#ifdef DEBUG_MDSP
    	dprintk("\n MakeMainUpScaleScreen --- pos xr,yr(%04x,%04x)", xr, yr);
		#endif
	}
	else
  	yr = 4096;
  	
	MainPosUpWindow(xr, yr);

	MainUpscaleWindow(phr, pvr);

	DISize(sw, sh/2);

  if( s->di == DI_2D)
			TurnOn2DDI();
	else  {
		  TurnOnWeave();
		  if(yr != 4096)
		  	TurnOnWeaveUpScale();
		}

	EnablePosUpScale();

}

/*! 
 * \brief
 *    Set Zoom an area up to screan
 *
 * \param [in] id 
 * \param [in] x ,y ,w, h : position and size (pixel unit)
 * 
 * \return
 *        <NONE> 
 * \note  x,y = display memory start position. not just display position
 */
//Note: 
void TWMux_SetZoomScreen(u8 id, u16 x, u16 y, u16 w, u16 h)
{
	u32  xr, yr;
	int PHR, PVR;
	PHR = tw2828devs[id].PHR;
	PVR = tw2828devs[id].PVR;

	if(tw2828devs[id].ZoonScreenOnFlag == ON) 
		TWMux_SetZoomScreenOff(id);

	#ifdef SUPPORT_MDBOX
	TWMux_AllMotionBoxOff(id);
	#endif

	MainMemoryStart(x>>2,y+DISPLAYBASE);

	xr = (w*4096 + PHR/2 )/PHR;
	yr = (h*1024 + PVR/2 )/PVR;
	if( (mTimingTbl[tw2828devs[id].mdrs].ctrl & INTERLACE) == INTERLACE) 
		MainUpscale(xr, yr/2);
	else
		MainUpscale(xr, yr);

	xr = (PHR*4096 + w-1 )/w;
	//if(xr>=0x71c7) xr=0x71c7;
	yr = (PVR*4096 + h/2 )/h;
	//if(yr>=0x8000) yr=0x7fff;

	MainPosUpWindow(xr, yr);

	MainUpscaleWindow(PHR, PVR);

	DISize(w,h/2);

	if( (mTimingTbl[tw2828devs[id].mdrs].ctrl & INTERLACE) == INTERLACE) 	{
		  TurnOnWeave();
		  if(yr != 4096) 
		  	TurnOnWeaveUpScale();
	}
  else
			TurnOn2DDI();
			
	EnablePosUpScale();

	SingleBoxOff(0);

	tw2828devs[id].ZoonScreenOnFlag = ON;

}

void TWMux_SetZoomScreenOff(u8 id)
{	
	int i, WinCursor;
	_ups  srn;

	if( (mTimingTbl[tw2828devs[id].mdrs].ctrl & INTERLACE) == INTERLACE) 
	  srn.di = DI_WEAVE;
	else
    srn.di = DI_2D;
  srn.ups_on = FALSE;
  srn.sx  = 0;
  srn.sy  = 0;
  MakeMainUpScaleScreen(&srn, tw2828devs[id].PHR, tw2828devs[id].PVR, tw2828devs[id].UHR, tw2828devs[id].UVR);
	#ifdef DEBUG_MDSP
	dprintk("Selected Recover from Zoom");
	#endif
	
	for(i=0; i<36; i++)
	 if((tw2828devs[id].pChannelDB[i].ctrl&MAINON)==MAINON) break;
	WinCursor = i;	

	TWMux_SetSingleBoxWindowOn(id, 0, 0xff,0,0,WinCursor);
	
	tw2828devs[id].ZoonScreenOnFlag = OFF;

}

//------------------------------------------------------
// CLK = (27 / (n+1) ) * (M+1) / q
#define Dram_LCD_0	  0x00
#define Dram_LCD_90	  0x01
#define Dram_LCD_180	0x02
#define Dram_LCD_270	0x03

enum {
	q_4,  q_4_I, 
	q_6,  q_6_I, 
	q_7,  q_7_I, 
	q_8,  q_8_I, 
	q_10, q_10_I, 
	q_12, q_12_I, 
	q_28, q_28_I, 
	q_16, q_16_I, 
};

typedef struct CLKType {
	u32 freq;
	u8 n, m, q;
	u8 phase;
} _ClkType;

const _ClkType ClockTable[] = {
 //freq/1000, n, m-1, q, phase
	{ 13500, 0, 2,  q_6, Dram_LCD_0},
	{ 27000, 0, 5,  q_6, Dram_LCD_0},
	{ 33750, 0, 4,  q_4, Dram_LCD_0},
	{ 37125, 0, 10, q_8, Dram_LCD_0},
	{ 39540, 0, 40, q_28, Dram_LCD_0},
	{ 40500, 0, 5,  q_4, Dram_LCD_0},
	{ 43875, 0, 12, q_8, Dram_LCD_0},
	{ 47250, 0, 6,  q_4, Dram_LCD_0},
	{ 50625, 0, 14, q_8, Dram_LCD_0},
	{ 54000, 0, 7,  q_4, Dram_LCD_0},
	{ 57375, 0, 16, q_8, Dram_LCD_0},
	{ 60750, 0, 8,  q_4, Dram_LCD_0},
	{ 64125, 0, 18, q_8, Dram_LCD_0},
	{ 64800, 0, 23, q_10, Dram_LCD_0},
	{ 67500, 0, 9,  q_4, Dram_LCD_0},
	{ 70875, 0, 20, q_8, Dram_LCD_0},
	{ 74250, 1, 32, q_6_I, Dram_LCD_0},
	{ 77625, 0, 22, q_8, Dram_LCD_0},
	{ 81000, 0, 23, q_8, Dram_LCD_0},
	{ 84375, 0, 24, q_8, Dram_LCD_0},
	{ 87750, 0, 25, q_8, Dram_LCD_0},
	{ 91125, 0, 26, q_8, Dram_LCD_0},
	{ 94500, 0, 27, q_8, Dram_LCD_0},
	{ 97875, 0, 28, q_8, Dram_LCD_0},
	{ 101250, 0, 29, q_8, Dram_LCD_0},
	{ 104625, 0, 30, q_8, Dram_LCD_0},
	{ 106312, 0, 63, q_16, Dram_LCD_0},	
	{ 108000, 0, 23, q_6, Dram_LCD_0},	
	{ 111375, 0, 32, q_8, Dram_LCD_0},
	{ 114750, 0, 33, q_8, Dram_LCD_0},
	{ 118125, 0, 34, q_8, Dram_LCD_0},
	{ 121500, 0, 35, q_8, Dram_LCD_0},
	{ 124875, 0, 36, q_8, Dram_LCD_0},
	{ 128250, 0, 37, q_8, Dram_LCD_0},
	{ 131625, 0, 38, q_8, Dram_LCD_0},
	{ 135000, 0, 39, q_8, Dram_LCD_0},
	{ 138375, 0, 40, q_8, Dram_LCD_0},
	{ 141750, 0, 41, q_8, Dram_LCD_0},
	{ 145125, 0, 42, q_8, Dram_LCD_0},
	{ 146570, 0, 37, q_7, Dram_LCD_0},
	{ 148500, 0, 32, q_6_I, Dram_LCD_0},
	{ 155250, 0, 22, q_4, Dram_LCD_0},
	{ 162000, 0, 35, q_6, Dram_LCD_0},  
	{ 166500, 0, 36, q_6, Dram_LCD_0},  
	{ 168750, 0, 24, q_4, Dram_LCD_0},  
	{ 171000, 0, 37, q_6, Dram_LCD_0},  
	{ 175500, 0, 38, q_6, Dram_LCD_0},  
	{ 177428, 0, 45, q_7, Dram_LCD_0},  
	{ 180000, 0, 39, q_6, Dram_LCD_0},  
	{ 184500, 0, 40, q_6, Dram_LCD_0},  
	{ 189000, 0, 41, q_6, Dram_LCD_0},  
	{ 193500, 0, 42, q_6, Dram_LCD_0},  
	{ 0, 0, 0, 0, 0 },
};

void ChangeVCLK(u32 vclk)
{

	int i;
	u8 n, m, q;
	u32 freq;
   
	i=0;
	while(1) {
		if(ClockTable[i].freq == 0) break;
		if(vclk <= ClockTable[i].freq) break;

		i++;
	}

	if(ClockTable[i].freq == 0 ) {
		freq = 108000;
		n = 0;
		m = 32;
		q = 6;
	}
	else {
		freq = ClockTable[i].freq;
		n = ClockTable[i].n;
		m = ClockTable[i].m;
		q = ClockTable[i].q;
	}

	WriteP(VPLLMR, m);
	WriteP(VPLLNR, n);

	switch (q) {
	case q_4     : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x00); break;
	case q_4_I   : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x01); break;
	case q_6     : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x02); break;
	case q_6_I   : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x03); break;
	case q_7     : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x04); break;
	case q_7_I   : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x05); break;
	case q_8     : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x06); break;
	case q_8_I   : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x07); break;
	case q_10    : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x08); break;
	case q_10_I  : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x09); break;
	case q_12    : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x0a); break;
	case q_12_I  : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x0b); break;
	case q_28    : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x0c); break;
	case q_28_I  : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x0d); break;
	case q_16    : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x0e); break;
	case q_16_I  : WriteP(VPLLCTRL, (ReadP(VPLLCTRL) &0xf0) | 0x0f); break;
	}	

  WriteP(VPLLPHASE, (ReadP(VPLLPHASE) &0xfc) | ClockTable[i].phase);

	printk("\r\nVCLK:%d -- N:%d, M:%d, Q:%d", (u32)freq, n, m, q);

}

/*! 
  \brief
		Set Display output resolution timing 
	\param [in] no   : chip no
	\param [in] mdrs : set main display resolution 	
	\retval  current mdrs value
	\par Update history:
		Mar/08/2012 - Created
	\par Decription
       (1)Set detail display output timing : HTotal, Vtotal, HActive, VActive, HSync, VSync, HPosi, VPosi \n
            Interlace, H/V Sync polarity  \n
       (2)Set VClock  \n
*/
HResult TWMux_SetMainDisplayResolution(int no,_Mdrs i)
{
	_ups  srn;


	if( mTimingTbl[i].vclk == 0) {
		printk("\r\nTWMux_SetMainDisplayResolution - Error! Current Mdrs=%s", mTimingTbl[i].str); 
		return rERR_MDRS;
	}
	
	tw2828devs[no].mdrs = i;
  
	// Set Main Display timing 
	WriteP(HTT_L, (u8)(mTimingTbl[i].htt-1));
	WriteP(HTT_H, (u8)((mTimingTbl[i].htt-1)>>8));

	WriteP(VTT_L, (u8)(mTimingTbl[i].vtt-1));
	WriteP(VTT_H, (u8)((mTimingTbl[i].vtt-1)>>8));

	WriteP(HDE_L, (u8)(mTimingTbl[i].hde-1));
	WriteP(HDE_H, (u8)((mTimingTbl[i].hde-1)>>8));

	WriteP(VDE_L, (u8)(mTimingTbl[i].vde-1));
	WriteP(VDE_H, (u8)((mTimingTbl[i].vde-1)>>8));

	WriteP(HFPORCH, (u8)(mTimingTbl[i].hfp/2));
	WriteP(HDE_H,  ((ReadP(HDE_H)&0x7f) | (((mTimingTbl[i].hfp/2)>>1)&0x80)));
	WriteP(VFPORCH, (u8)mTimingTbl[i].vfp);

	WriteP(HSYNCW, (u8)mTimingTbl[i].hs);
	WriteP(VSYNCW, (u8)mTimingTbl[i].vs);

	WriteP(SYS_CTRL, (ReadP(SYS_CTRL)&0xfc) | (mTimingTbl[i].ctrl&0x03));

	if( (mTimingTbl[i].ctrl & INTERLACE) == INTERLACE) 
		SetInterlaced(ON);
	else
		SetInterlaced(OFF);
	
  printk("\n[%d] mdrs : %s (%x)", no, mTimingTbl[i].str, i ); 

	// Set main display pixel clock
  ChangeVCLK(mTimingTbl[i].vclk);

	tw2828devs[no].PHR=mTimingTbl[i].hde; 
	tw2828devs[no].PVR=mTimingTbl[i].vde;

	if( (mTimingTbl[i].ctrl & INTERLACE) == INTERLACE) {
		tw2828devs[no].PVR *= 2;
		tw2828devs[no].UHR = tw2828devs[no].PHR;
		tw2828devs[no].UVR = tw2828devs[no].PVR;
    tw2828devs[no].INTERLACED = 1;	
	  srn.di = DI_WEAVE;
	}
	else
	{	
    srn.di = DI_2D;
    
		if(tw2828devs[no].PHR > MHR_UPS) 
		{
			tw2828devs[no].UHR = MHR_UPS;
			tw2828devs[no].UVR	= MVR_UPS;
		}
		else if(tw2828devs[no].PVR > MVR_UPS) {  // to use memory effective for 1600*900
			tw2828devs[no].UHR = tw2828devs[no].PHR;
			tw2828devs[no].UVR	= MVR_UPS;
		}
		else {
			tw2828devs[no].UHR = tw2828devs[no].PHR;
			tw2828devs[no].UVR	= tw2828devs[no].PVR;
		}
    tw2828devs[no].INTERLACED = 0;	
  }

  srn.ups_on = FALSE;
  srn.sx  = 0;
  srn.sy  = 0;
  MakeMainUpScaleScreen(&srn, tw2828devs[no].PHR, tw2828devs[no].PVR, tw2828devs[no].UHR, tw2828devs[no].UVR);

	return i;
}

/*! 
  \brief
		Get Display output resolution
	\retval  current mdrs value
	\par Update history:
		JUL/13/2012 - Created
*/
_Mdrs TWMux_GetMainDisplayResolution(int no)
{
	return tw2828devs[no].mdrs;
}

/*! 
  \brief
		Get Display Inofration data
	\param  [in]  no chip id
	\param  [out] UHR
	\param  [out] UVR
	\par Update history:
		JUL/30/2012 - Created
*/
void TWMux_GetMainDisplayInfo(int no, int *phr, int *pvr, int *uhr, int *uvr)
{
	*phr = tw2828devs[no].PHR;
	*pvr = tw2828devs[no].PVR;
	*uhr = tw2828devs[no].UHR;
	*uvr = tw2828devs[no].UVR;
}


