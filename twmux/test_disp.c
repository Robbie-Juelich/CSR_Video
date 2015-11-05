
/*! 
 *************************************************************************************
 * \file  <disp.c>
 *
 * \brief
 *    Main display test and sample application
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    Heejeong Ryu <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
#if 0

#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef __APP__
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif
# endif
#include <linux/kernel.h>
#include <linux/string.h>

#include "tw2828_twmux.h"
#include "tw2828_config.h"

#include "tw2828_test.h"
#include "tw2828_logo.h"

#if 0

#include "rec.h"
//#include "mse.h"
#include "system.h"
//#include "monitor.h"
#include "mdbox.h"
//#include "osd.h"
#include "spot.h"
#include "link.h"
#ifndef __APP__
#include "mem_map.h"
#endif
#endif

#include "test_disp_tbl.h"
#include "tw2828_logo.h"
#include "tw2828_ioctl.h"

extern _Logo_Info *cur_logo;
extern _Logo_Info logo_info[];
extern unsigned char logodata_4ch[];
extern unsigned char logodata_6ch[];
extern unsigned char logodata_9ch[];

u8 HostBusNo = 1;

u32 IHR, IVR, PHR, PVR, UHR, UVR, INTERLACED;	

u8 CurDisplayMode=0xFF;
u8 PBAutoMode = OFF;
_ChannelInfo ChannelDB[36];
_ChannelInfo CW;
//_LogoInfo logo;
//_LogoInfo logo2;

u8  WinCursor=0;

//struct windows_cam_mode win_mode_cache;

/** @defgroup Display_Test /tw2828Test/Display_Test 
*   @ingroup tw2828Test
*   @addtogroup Display_Test
*   @{
*/

const _ChannelInfo  Default_ChWin = 
{
	chno : 0,
	port : 0,
	ix:0,iy:0,iw:0,ih:0,
	x:0,y:0,w:0,h:0,
	cx:0, cy:0, cx_unit:0, cy_unit:0,
	ctrl:0,
	//level:	MD_LEVEL_VAL,
};

inline void CleanCWDataBaseAll(void)
{
	int i;

	for(i=0; i<36; i++) {
		memset(&ChannelDB[i], 0, sizeof(_ChannelInfo));
		ChannelDB[i].chno = tb_36CH[i].chno;
	}
   
	memcpy( &CW, &Default_ChWin, sizeof(_ChannelInfo) );

}


void LoadMdspDb_20CH(void)
{
	// display 4x4live + 4xpb  window
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/5/4;
	CW.ih = UVR/4;
	CW.w = PHR/5/4;
	CW.h = PVR/4;

	while(i<20)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		if(i<16) {
			ChannelDB[i].chno = CH1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON |MDBOXON;
		}
		else{
			ChannelDB[i].chno = PB1+(i-16);
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].ctrl = MAINON |MDBOXON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.ix = CW.iw*(i%4);
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%4);
			if((i%4)==0) CW.y += CW.h;

		}
		else if(i<20) {
			CW.ix = (UHR/4-CW.iw);
			CW.iy = CW.ih*(i-16);

			CW.x = (PHR/4-CW.w);
			CW.y = CW.h*(i-16);

		}
	}

}

void LoadMdspDb_6CH(void)
{
	// display 3x2 live window
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll();

	CW.iw = UHR/3/4;
	if((UVR/2) <= 480) CW.ih = UVR/2;
	else 	  CW.ih = 480;

	CW.w = PHR/3/4;
	if(CW.ih == (UVR/2)) CW.h = PVR/2;
	else CW.h = 480;

//	CW.iy=(UVR-CW.ih-CW.ih)/2;
//	CW.y=(PVR-CW.h-CW.h)/2;
	CW.iy = 0x0;
	CW.y = 0x0;
	
	i=0;
	//while(i<18)
	while(i<6)
	{

		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		if(i<6) ChannelDB[i].ctrl = MAINON|MDBOXON;
		//else if( (i==16) || (i==17)) ChannelDB[i].ctrl = MAINON |MDBOXON ;
		else 	ChannelDB[i].ctrl = OFF; 
		
	//	ChannelDB[i].chno = CH1+i;
		ChannelDB[i].chno = CH1 +  (i/2) * 4  + i % 2;
		//int j = win_mode_cache.cam_id[i];
		//ChannelDB[i].chno = CH1 +  (j/2) * 4  + j%2;
		if(i<16)
			ChannelDB[i].port = PORT_LIVE_0+i;
		else {
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

	    if(i<6) {
			CW.ix = CW.iw*(i%3);
			if((i%3)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%3);
			if((i%3)==0) CW.y += CW.h;
		}
	    else if((i==16)||(i==17) ) {
			CW.ix = CW.iw*2;
			if(i==16) CW.iy=(UVR-CW.ih-CW.ih)/2;
			else CW.iy= (UVR-CW.ih-CW.ih)/2+CW.ih;

			CW.x = CW.w*2;
			if(i==16) CW.y=(PVR-CW.h-CW.h)/2;
			else CW.y= (PVR-CW.h-CW.h)/2+CW.h;
		}

	}

	cur_logo = &logo_info[LOGO_6ch];
	cur_logo->x0 = 0x0;
	cur_logo->y0 = ChannelDB[0].h * 2;
	cur_logo->width = UHR;
	cur_logo->height = UVR - cur_logo->y0;
	cur_logo->logo_data = logodata_6ch;

}

void LoadMdspDb_12CH(void)
{
	// display 2*4 live + 4D1 pb window
	// 4D1 (1440x960) : 16bit/54Mhz/PB1-2/Rec1-2/Normal Mode

	u8 i=0;
	u16 k = (UHR+1)/3;

	PBAutoMode = 0; // PB Auto Mode

	CleanCWDataBaseAll();

	CW.iw = (UHR-(k*2))/2/4;
	CW.ih = UVR/4;
	CW.w = (PHR-(PHR/3*2))/2/4;
	CW.h = PVR/4;

	while(i<=18)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].port = PORT_LIVE_0+i;
		ChannelDB[i].chno = CH1+i;
		
		if(i==18){
			ChannelDB[i].port = PORT_PB_3;
			ChannelDB[i].chno = PB3;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		if( (i>=8) && (i<=17)) ChannelDB[i].ctrl = OFF;
		else 		ChannelDB[i].ctrl = MAINON;

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.ix = CW.iw*(i%2);
			if((i%2)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%2);
			if((i%2)==0) CW.y += CW.h;
		}
		else {
			// PB
		 if(UVR>=960) {
			CW.ix = k/4;
			CW.iy = (UVR-960)/2;
			CW.iw = (UHR-k)/4;
			CW.ih = 960;

			CW.x = PHR/3/4;
			if(CW.iy==0) CW.y=0;
			else CW.y = (PVR-960)/2;
			CW.w = (PHR-(PHR/3))/4;
			if(CW.y==0) CW.h = PVR;
			else CW.h = 960;
		 }
		 else {
			CW.ix = k/4;
			CW.iy = 0;
			CW.iw = (UHR-k)/4;
			CW.ih = UVR;

			CW.x = PHR/3/4;
			CW.y=0;
			CW.w = (PHR-(PHR/3))/4;
			CW.h = PVR;
	
		 }
		 	
		}
	}
	
	
	printk("\r\n2 LoadMdspDb_12CH- ");

}

void LoadMdspDb_HD_Full(void)
{
	// display 2 live + 1*6VGA pb

	u8 i=0;
	u16 k = UHR/3;
	u16 k1 = PHR/3;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll();

	CW.ix = k*2/4; 
	CW.iw = k/4;
	if((UVR/2) <= 480)  CW.ih = UVR/2;
	else CW.ih = 480;
	CW.x = k1*2/4; 
	CW.w = k1/4;
	if((UVR/2) <= 480)  CW.h = PVR/2;
	else CW.h = 480;

	while(i<17)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		if(i<16) {
			ChannelDB[i].port = PORT_LIVE_0+i;
		}
		else {
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}

		if( (i>=2) && (i<=15)) ChannelDB[i].ctrl = OFF;
		else 		ChannelDB[i].ctrl = MAINON;

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.iy = UVR/2; //540
			CW.y = PVR/2;
		}
		else {
			// PB
			CW.ix = 0;
			CW.iw = (UHR-k)/4;
			CW.iy = (UVR-720)/2;//			iy = 180;
			CW.ih = 720;

			CW.w = (PHR-k1)/4;
			CW.h = 720*PVR/UVR;
			CW.x = 0;
			CW.y = (PVR-CW.h)/2; 

		}
	}
	

}

void LoadMdspDb_1HDn8D1PBn4Live(void)
{
	u8 i=0;
	u16 k, k1;

	PBAutoMode = 1; // PB Auto Mode
	CleanCWDataBaseAll();

	k = UHR/5/4;
	CW.ih = UVR/4;
	k1= PHR/5/4;
	CW.h = PVR/4;
	CW.iw =k;
	CW.w = k1;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		if(i<4){
			ChannelDB[i].chno = CHx1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON;
		}
		else if(i==16)	{
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
			ChannelDB[i].port = PORT_PB_1;
			ChannelDB[i].iw = (UHR/4)-k;
			ChannelDB[i].ih = 540;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		else if(i>=24) {
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			if( ChannelDB[i].chno <= PBx12) ChannelDB[i].port = PORT_PB_3;
			else if( ChannelDB[i].chno <= PBx16) ChannelDB[i].port = PORT_PB_4;
		}
		else {
				ChannelDB[i].chno = CHx1+i;
			  ChannelDB[i].ctrl = OFF;	
		}
	
		i++;

		if(i<4){
			CW.ix = 0;
			CW.iy = CW.ih*i;

			CW.x = 0;
			CW.y = CW.h*i;
		}
		else if(i==16) {
			CW.ix = k;
			CW.iy = 0;

			CW.x = k1;
			CW.y = 0;
			CW.w = k1*4;
			CW.h = 540;
			
		}
		else {
			CW.ix = k*(i%4) + k;
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = k1*(i%4) + k1;
			if((i%4)==0) CW.y += CW.h;
			CW.w = k1;
			CW.h = PVR/4;;	
		}
	
		if(i==4) { 
			CW.y=0; CW.iy=0; 
		}
	}


}

void LoadMdspDb_HD_Half(void)
{
	// display 2 live + 2*6VGA pb
	u8 i=0;
	u16 k = UHR/3;
	u16 k1 = PHR/3;

	PBAutoMode = 0; // PB Normal Mode
	CleanCWDataBaseAll();

	CW.ix = k*2/4;
	CW.iw = k/4;
	if((UVR/2) <= 480)  CW.ih = UVR/2;
	else CW.ih = 480;
	CW.x = k1*2/4;
	CW.w = k1/4;
	if(CW.ih == (UVR/2)) CW.h = PVR/2;
	else CW.h = 480;

	while(i<19)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		if(i<16) {
			ChannelDB[i].port = PORT_LIVE_0+i;
		}
		else {
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		if( (i==0) || (i==1) || (i==16) || (i==18)) ChannelDB[i].ctrl = MAINON;
		else ChannelDB[i].ctrl = OFF;
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.iy = UVR/2;
			CW.y = PVR/2;
		}
		else {

			// 
			CW.ix = 0;
			CW.iw = (UHR-k)/4;
			if((UVR/2)<540) CW.ih = UVR/2;
			else 	CW.ih = 540;
			if(i==16)  CW.iy=0;
			else  CW.iy = CW.ih;

			CW.x = 0;
			if(i==16)  CW.y=0;
			else  CW.y = CW.h;
			CW.w = (PHR-k1)/4;
			if((UVR/2)<540) CW.h = PVR/2;
			else 	CW.h = 540;
			// PB
			
		}
	}

}

void LoadMdspDb_16PB_16Live(void)
{
	//used 16D1_108MHz_PB, 16Live
	u8 i=0, j=0;
	
	PBAutoMode = 1; // PB Auto Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/6/4;
	CW.ih = UVR/6;
	CW.w = PHR/6/4;
	CW.h = PVR/6;

	i=16;
	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		if(i<16){
			ChannelDB[i].chno = CHx1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON;
		}
		else {
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			if( ChannelDB[i].chno <= PBx4) ChannelDB[i].port = PORT_PB_1;
			else if( ChannelDB[i].chno <= PBx8) ChannelDB[i].port = PORT_PB_2;
			else if( ChannelDB[i].chno <= PBx12) ChannelDB[i].port = PORT_PB_3;
			else if( ChannelDB[i].chno <= PBx16) ChannelDB[i].port = PORT_PB_4;
		}
	
		i++;
		j++;
		if(i==32) i=0;
		if(i==16) break;
			
		CW.ix = CW.iw*(j%6);
		if((j%6)==0) CW.iy += CW.ih;

		CW.x = CW.w*(j%6);
		if((j%6)==0) CW.y += CW.h;
		
	}

}


void LoadMdspDb_16LiveN16PB(void)
{
	//used 16D1_108MHz_PB, 16Live
	u8 i=0;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll();

	CW.iw = UHR/8/4;
	CW.ih = UVR/4;
	CW.w = PHR/8/4;
	CW.h = PVR/4;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		if(i<16){
			ChannelDB[i].chno = CHx1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON;
		}
		else {
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			if( ChannelDB[i].chno <= PBx4) ChannelDB[i].port = PORT_PB_1;
			else if( ChannelDB[i].chno <= PBx8) ChannelDB[i].port = PORT_PB_2;
			else if( ChannelDB[i].chno <= PBx12) ChannelDB[i].port = PORT_PB_3;
			else if( ChannelDB[i].chno <= PBx16) ChannelDB[i].port = PORT_PB_4;
		}
	
		i++;
			
		if(i<16) CW.ix = CW.iw*(i%4);
		else 		 CW.ix = CW.iw*(i%4) + (CW.iw*4);
		if((i%4)==0) CW.iy += CW.ih;
		if(i==16) CW.iy=0;
			
		if(i<16) CW.x = CW.w*(i%4);
		else 		 CW.x = CW.w*(i%4) + (CW.w*4);
		if((i%4)==0) CW.y += CW.h;
		if(i==16) CW.y=0;
		
		
	}

}

void LoadMdspDb_16LiveN16PB_FieldSwitch(void)
{
	//used 16D1_108MHz_PB 2port FW, 16Live : 7x5
	u8 i=0;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll();

	CW.iw = UHR/6/4;
	CW.ih = UVR/6;
	CW.w = PHR/6/4;
	CW.h = PVR/6;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		if(i<16){
			ChannelDB[i].chno = CHx1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON;
		}
		else {
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih*2;

			if( ChannelDB[i].chno <= PBx4) ChannelDB[i].port = PORT_PB_3;//PORT_PB_1;
			else if( ChannelDB[i].chno <= PBx8) ChannelDB[i].port = PORT_PB_3;//PORT_PB_1;
			else if( ChannelDB[i].chno <= PBx12) ChannelDB[i].port = PORT_PB_4;//PORT_PB_2;
			else if( ChannelDB[i].chno <= PBx16) ChannelDB[i].port = PORT_PB_4;//PORT_PB_2;
		}
	
		i++;
					
		CW.ix = CW.iw*(i%6);
		if((i%6)==0) CW.iy += CW.ih;

		CW.x = CW.w*(i%6);
		if((i%6)==0) CW.y += CW.h;
		
	}

}


void LoadMdspDb_16D1PB4Live(void)
{
	u8 i=0;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll();

	CW.iw = UHR/5/4;
	CW.ih = UVR/4;
	CW.w = PHR/5/4;
	CW.h = PVR/4;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		if(i<4){
			ChannelDB[i].chno = CHx1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON;
		}
		else {
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			if( ChannelDB[i].chno <= PBx4) ChannelDB[i].port = PORT_PB_1;
			else if( ChannelDB[i].chno <= PBx8) ChannelDB[i].port = PORT_PB_2;
			else if( ChannelDB[i].chno <= PBx12) ChannelDB[i].port = PORT_PB_3;
			else if( ChannelDB[i].chno <= PBx16) ChannelDB[i].port = PORT_PB_4;
		}
	
		i++;

		if(i<4){
			CW.ix = 0;
			CW.iy = CW.ih*i;

			CW.x = 0;
			CW.y = CW.h*i;
		}
		else {
			CW.ix = CW.iw*(i%4) + CW.iw;
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%4) + CW.w;
			if((i%4)==0) CW.y += CW.h;
		}
	
		if(i==4) { 
			CW.y=0; CW.iy=0; 

			while(i<16) 
			  ChannelDB[i++].ctrl = OFF;	
		}
	}

}

void LoadMdspDb_16QuadPB(void)
{	// 6x5: 2x4+6:Live 4x4:PB_QUAD Auto
	u8 i=0;
	int k, k1;
	
	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll();

  CW.iw = UHR/6/4;
	CW.ih = UVR/5;
	CW.w = PHR/6/4;
	CW.h = PVR/5;
	k=CW.iw;
	k1=CW.w;
	CW.cx_unit = 1; //1:4pixel, 2:8pixel
	CW.cy_unit = 2; //2:2line,  4:4line 

	while(i<32) 
	{

		switch(i) {
		case 16: case 18: case 28: case 30:	case 8:	  CW.ix=0;	  CW.x=0;    break;
		case 17: case 19: case 29: case 31:	case 9:	  CW.ix=k;	  CW.x=k1;   break;
		case 20: case 22: case 0:  case 2:	case 10:  CW.ix=k*2;	CW.x=k1*2; break;
		case 21: case 23: case 1:  case 3:	case 11:	CW.ix=k*3;	CW.x=k1*3; break;
		case 24: case 26: case 4:  case 6:	case 12:	CW.ix=k*4;	CW.x=k1*4; break;
		case 25: case 27: case 5:  case 7:	case 13:	CW.ix=k*5;	CW.x=k1*5; break;
		}
		
		switch(i) {
		case 16: case 17: case 20: case 21: case 24:	case 25: CW.iy=0;	     CW.y=0;      break;
		case 18: case 19: case 22: case 23: case 26:	case 27: CW.iy=CW.ih;	     CW.y=CW.h;   break;
		case 28: case 29: case 0:  case 1:  case 4:	  case 5:  CW.iy=CW.ih*2;	   CW.y=CW.h*2; break;
		case 30: case 31: case 2:  case 3:  case 6:	  case 7:  CW.iy=CW.ih*3;	   CW.y=CW.h*3; break;
		case 8: case 9: case 10:  case 11:  case 12:	case 13: CW.iy=CW.ih*4;	   CW.y=CW.h*4; break;
	  }

		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		if(i>=16) {
			ChannelDB[i].tw = CW.iw*2;
			ChannelDB[i].th = CW.ih*2;
			ChannelDB[i].cx = (i%2)*CW.iw/ChannelDB[i].cx_unit;
			ChannelDB[i].cy = ((i%4)>>1)*CW.ih/ChannelDB[i].cy_unit;

  		ChannelDB[i].chno = tb_36CH[i].chno;
	  	ChannelDB[i].port = tb_36CH[i].port;
		  ChannelDB[i].ctrl = MAINON  | AUTOMODEON;	
		}
		
		if(i<14) {
		  ChannelDB[i].chno = tb_36CH[i].chno;
		  ChannelDB[i].port = tb_36CH[i].port;
		  ChannelDB[i].ctrl = MAINON;	
	  }
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

	}

}

void LoadMdspDb_32CH(void)
{
	u8 i=0;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/7/4;
	//if(UVR>240)	ih = 240; 
	//else 
	CW.ih = UVR/5;
	CW.w = PHR/7/4;
	CW.h = PVR/5;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].ctrl = MAINON;
		ChannelDB[i].chno = tb_36CH[i].chno;
		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
		
		if(i>=16) {
			ChannelDB[i].tw = CW.iw*2;
			ChannelDB[i].th = CW.ih*2;
			ChannelDB[i].cx_unit = 1; //1:4pixel, 2:8pixel
			ChannelDB[i].cy_unit = 2; //2:2line,  4:4line 
			ChannelDB[i].cx = (i%2)*CW.iw/ChannelDB[i].cx_unit;
			ChannelDB[i].cy = ((i%4)>>1)*CW.ih/ChannelDB[i].cy_unit;
    }
    		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.ix = CW.iw*(i%7);
			if((i%7)==0) CW.iy += CW.ih;
			CW.x = CW.w*(i%7);
			if((i%7)==0) CW.y += CW.h;
		}
		else {
			CW.ix = CW.iw*((i+3)%7);
			if(((i+3)%7)==0) CW.iy += CW.ih;
			CW.x = CW.w*((i+3)%7);
			if(((i+3)%7)==0) CW.y += CW.h;
		}
	
	}

}

void LoadMdspDb_2_8D1PB4Live(void)
{
	u8 i=0;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/5/4;
	CW.ih = UVR/4;
	CW.w = PHR/5/4;
	CW.h = PVR/4;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		if(i<4){
			ChannelDB[i].chno = CHx1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
		}
		else {
			ChannelDB[i].chno = PBx1+(i-16);
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			if( ChannelDB[i].chno <= PBx8) ChannelDB[i].port = PORT_PB_1;
			else if( ChannelDB[i].chno <= PBx16) ChannelDB[i].port = PORT_PB_3;
		}
	
		i++;

		if(i<4){
			CW.ix = 0;
			CW.iy = CW.ih*i;

			CW.x = 0;
			CW.y = CW.h*i;
		}
		else {
			CW.ix = CW.iw*(i%4) + CW.iw;
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%4) + CW.w;
			if((i%4)==0) CW.y += CW.h;
		}
	
		if(i==4) { 
			CW.y=0; CW.iy=0; i=16;
		}
	}

}


void LoadMdspDb_2_4D1_4Cut(void)
{
	// display 2 live + 2*4D1 pb  to 12ch window demo.
	// Autochannel id insertion demo by cropping 2*(2*4D1 to 4 cut PB)

	u8 i=0;
	u16 k, k1;
	
	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/3/4;
	CW.ih = 480;
	CW.w = PHR/3/4;
	if(CW.ih == (UVR/2)) CW.h = PVR/2;
	else CW.h = 480;
	CW.iy=(UVR-CW.ih-CW.ih)/2;
	CW.y=(PVR-CW.h-CW.h)/2;
	k1=UHR/3/4;
	k=PHR/3/4;

	CleanCWDataBaseAll(); 

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;

		if(i<2) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
		}
		else if( (i>=16) && (i<=19)) { 
			ChannelDB[i].ctrl = MAINON   | AUTOMODEON | INSERTCHID;
			ChannelDB[i].tw = CW.iw*2;
			ChannelDB[i].th = CW.ih*2;
		}
		else if( (i>=24) && (i<=27)) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON | INSERTCHID;
			ChannelDB[i].tw = CW.iw*2;
			ChannelDB[i].th = CW.ih*2;
		}
		else  ChannelDB[i].ctrl = OFF;

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<2) {
			CW.iy += CW.ih;
			CW.y += CW.h;
		}
		else {
			if(i==16) {
				CW.iw = UHR/3/4;
				CW.ih = UVR/4;
				CW.iy=0; CW.y=0;
				CW.ix=k1; CW.x=k;

				CW.w = PHR/3/4;
				CW.h = PVR/4;
			}
			else if( (i>=16) && (i<=19)) {
				CW.ix = k1+CW.iw*(i%2);
				if((i%2)==0) CW.iy += CW.ih;
			
				CW.x = k+CW.w*(i%2);
				if((i%2)==0) CW.y += CW.h;
			}
			else if( (i>=24) && (i<=27)) {
				CW.ix = k1+CW.iw*(i%2);
				if((i%2)==0) CW.iy += CW.ih;
			
				CW.x = k+CW.w*(i%2);
				if((i%2)==0) CW.y += CW.h;
			}
		}
	}

}

void LoadMdspDb_16D1_NewFieldSwitching(void)
{
	u8 i=0;
	u16 ix=0,iy=0,iw,ih;
	u16 x=0,y=0,w,h;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	iw = UHR/4/4;
	if(UVR>240)	ih = 240; 
	else ih = UVR/4;
	w = PHR/4/4;
	if(PVR>240)	h = 240; 
	else h = PVR/4;

	while(i<32)
	{
		ChannelDB[i].chno = tb_32CH_FS[i].chno;
		ChannelDB[i].port = tb_32CH_FS[i].port;
		ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
		ChannelDB[i].ix = ix;
		ChannelDB[i].iy = iy;
		ChannelDB[i].iw = iw;
		ChannelDB[i].ih = ih;
		ChannelDB[i].x = x;
		ChannelDB[i].y = y;
		ChannelDB[i].w = w;
		ChannelDB[i].h = h;
		ChannelDB[i].tw = iw;
		ChannelDB[i].th = ih*2;
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		ix = iw*(i%4);
		if((i%4)==0) iy += ih;

		x = w*(i%4);
		if((i%4)==0) y += h;
	
	}

}

//------------------------------------------------------

void LoadMdspDb_4HD_PB(void)
{
	// display 4*HD pb

	u8 i=0;
	u16 k = UHR/2;
	u16 k1 = PHR/2;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll(); 

	CW.iw = k/4;
	if((UVR/2)<540) CW.ih = UVR/2;
	else 	CW.ih = 540;
	CW.w = k1/4;
	if(CW.ih == (UVR/2)) CW.h = PVR/2;
	else CW.h = 540; //480;

	i=16;
	while(i<=19)
	{
		if( i==16 || i==18 ) CW.ix = 0;
		else CW.ix=k/4;	
		if(i<18)  CW.iy=0;
		else  CW.iy = CW.ih;

		if( i==16 || i==18 ) CW.x = 0;
		else CW.x=k1/4;	
		if(i<18)  CW.y=0;
		else  CW.y = CW.h;

		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		ChannelDB[i].port = PORT_PB_1+(i-16);
		ChannelDB[i].ctrl = MAINON |MDBOXON;
		ChannelDB[i].tw = CW.iw;
		ChannelDB[i].th = CW.ih;
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;
	
	}

}

void LoadMdspDb_6VGA_16Cut(void)
{
	// display 6VGA pb  cutting to 16ch window demo.
	// Autochannel id insertion demo by cropping (1*6VGA to 16 cut PB)

	u8 i=0;
	u16 ix=0,iy=0,iw,ih; //window position in memory before UPS
	u16 x=0,y=0,w,h;
	u16 k;
	u16 k1;

	PBAutoMode = 1; // PB Auto Mode

	iw = UHR/4/4;
	ih = UVR/4;
	w = PHR/4/4;
	if(ih == (UVR/2)) h = PVR/2;
	else h = PVR/4;
/*	
	iy=(UVR-ih-ih)/2;
	y=(PVR-h-h)/2;
*/
	k=UHR/4/4;
	k1=PHR/4/4;

	CleanCWDataBaseAll(); 

	while(i<32)
	{
		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;
		ChannelDB[i].cx_unit = 1; //1:4pixel, 2:8pixel
		ChannelDB[i].cy_unit = 4; //2:2line,  4:4line 

		if(i>=16) {
			ChannelDB[i].tw = iw*4;
			ChannelDB[i].th = ih*4;
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON | INSERTCHID;
		}
		else  ChannelDB[i].ctrl = OFF;

		ChannelDB[i].ix = ix;
		ChannelDB[i].iy = iy;
		ChannelDB[i].iw = iw;
		ChannelDB[i].ih = ih;
		ChannelDB[i].cx = ix/ChannelDB[i].cx_unit;
		ChannelDB[i].cy = iy/ChannelDB[i].cy_unit;
		ChannelDB[i].x = x;
		ChannelDB[i].y = y;
		ChannelDB[i].w = w;
		ChannelDB[i].h = h;
		
		#ifdef DEBUG_MDSP
		printk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		printk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		switch(i-16) {
		case 0: case 2: case 8: case 10:	ix=0;	x=0; break;
		case 1: case 3: case 9: case 11:	ix=k;	x=k1; break;
		case 4: case 6: case 12: case 14:	ix=k*2;	x=k1*2; break;
		case 5: case 7: case 13: case 15:	ix=k*3;	x=k1*3; break;
		}
		
		switch(i-16) {
		case 0: case 1: case 4: case 5:	iy=0;	y=0; break;
		case 2: case 3: case 6: case 7:	iy=ih;	y=h; break;
		case 8: case 9: case 12: case 13:	iy=ih*2;	y=h*2; break;
		case 10: case 11: case 14: case 15:	iy=ih*3;	y=h*3; break;
		}
	}

}
//---------------------------------------------
void LoadMdspDb_1HD_16Cut(void)
{
	// display 1HD pb to 16ch window demo.
	// Autochannel id insertion demo by cropping (1*HD to 16 cut PB)
  
	u8 i=0;
	u16 k;
	u16 k1;

	PBAutoMode = 1; // PB Auto Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	CW.ih = UVR/4;
	CW.w = PHR/4/4;
	CW.h = PVR/4;

	k=UHR/4/4;
	k1=PHR/4/4;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;
		ChannelDB[i].cx_unit = 2; //1:4pixel, 2:8pixel
		ChannelDB[i].cy_unit = 4; //2:2line,  4:4line 

		if(i>=16) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON | INSERTCHID;
			ChannelDB[i].tw = CW.iw*4;
			ChannelDB[i].th = CW.ih*4;
		}
		else  ChannelDB[i].ctrl = OFF;

		ChannelDB[i].cx = (CW.ix+ChannelDB[i].cx_unit-1)/ChannelDB[i].cx_unit;
		ChannelDB[i].cy = (CW.iy+ChannelDB[i].cy_unit-1)/ChannelDB[i].cy_unit;
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d : %04d, %04d ",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h, \
		     ChannelDB[i].cx, ChannelDB[i].cy);
		#endif

		i++;
		
		switch(i-16) {
		case 0: case 2: case 8: case 10:	CW.ix=0;	CW.x=0;  break;
		case 1: case 3: case 9: case 11:	CW.ix=k;	CW.x=k1; break;
		case 4: case 6: case 12: case 14:	CW.ix=k*2;	CW.x=k1*2; break;
		case 5: case 7: case 13: case 15:	CW.ix=k*3;	CW.x=k1*3; break;
		}
		
		switch(i-16) {
		case 0: case 1: case 4: case 5:	    CW.iy=0;	      CW.y=0; break;
		case 2: case 3: case 6: case 7:	    CW.iy=CW.ih;	  CW.y=CW.h; break;
		case 8: case 9: case 12: case 13:	  CW.iy=CW.ih*2;	CW.y=CW.h*2; break;
		case 10: case 11: case 14: case 15:	CW.iy=CW.ih*3;	CW.y=CW.h*3; break;
		}
		

	}

}


//---------------------------------------------
void LoadMdspDb_1HD_8Cut_8Live(void)
{
	// Display 1HD pb 1080i by PB12_pin cutting to 16ch window, and display 8ch(PB5,6,7,8,13,14,15,16) with 8 Live demo. 
	// HD input is from 4HD daughter board.
	// Autochannel id insertion demo by cropping (1*HD to 16 cut PB)
  
	u8 i=0;
	u16 k, k1;

	PBAutoMode = 1; // PB Auto Mode
	
	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	CW.ih = UVR/4;
	CW.w = PHR/4/4;
	CW.h = PVR/4;

	k=UHR/4/4;
	k1=PHR/4/4;

	while(i<32)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;
		ChannelDB[i].cx_unit = 2; //1:4pixel, 2:8pixel
		ChannelDB[i].cy_unit = 4; //2:2line,  4:4line 

		if(i<8) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
		}
		else if((i>=20 && i<=23)||((i>=28 && i<=31))) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON | INSERTCHID;
			ChannelDB[i].tw = CW.iw*4;
			ChannelDB[i].th = CW.ih*4;
		}
		else  { 
			 ChannelDB[i].ctrl = OFF; 
		}

		if(i>=16) {
		ChannelDB[i].cx = (CW.ix+ChannelDB[i].cx_unit-1)/ChannelDB[i].cx_unit;
		ChannelDB[i].cy = (CW.iy+ChannelDB[i].cy_unit-1)/ChannelDB[i].cy_unit;
	  }
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d : %04d, %04d ",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h, \
		     ChannelDB[i].cx, ChannelDB[i].cy);
		#endif
		
		i++;

		switch(i) {
		case 0: case 2: case 4: case 6:	    CW.ix=0;	CW.x=0;  break;
		case 1: case 3: case 5: case 7:	    CW.ix=k;	CW.x=k1; break;
		case 20: case 22: case 28: case 30:	CW.ix=k*2;	CW.x=k1*2; break;
		case 21: case 23: case 29: case 31:	CW.ix=k*3;	CW.x=k1*3; break;
		}
		
		switch(i) {
		case 0: case 1: case 20: case 21:	CW.iy=0;	      CW.y=0; break;
		case 2: case 3: case 22: case 23:	CW.iy=CW.ih;	  CW.y=CW.h; break;
		case 4: case 5: case 28: case 29:	CW.iy=CW.ih*2;	CW.y=CW.h*2; break;
		case 6: case 7: case 30: case 31:	CW.iy=CW.ih*3;	CW.y=CW.h*3; break;
		}
	}

}
//---------------------------------------------
void LoadMdspDb_16Live(void)
{
	// display 4x4live window
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	CW.ih = UVR/4;
	CW.w = PHR/4/4;
	CW.h = PVR/4;

	while(i<16)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		ChannelDB[i].port = PORT_LIVE_0+i;
		ChannelDB[i].ctrl = MAINON | MDBOXON;
	
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		CW.ix = CW.iw*(i%4);
		if((i%4)==0) CW.iy += CW.ih;

		CW.x = CW.w*(i%4);
		if((i%4)==0) CW.y += CW.h;

	}

}
//---------------------------------------------
void LoadMdspDb_16Live_HDSpot(void)
{
	// display 4x4live window
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	CW.ih = UVR/4;
	CW.w = PHR/4/4;
	CW.h = PVR/4;

	while(i<16)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		ChannelDB[i].port = PORT_LIVE_0+i;
		ChannelDB[i].ctrl = MAINON | MDBOXON;
	
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		CW.ix = CW.iw*(i%4);
		if((i%4)==0) CW.iy += CW.ih;
		CW.x = CW.w*(i%4);
		if((i%4)==0) CW.y += CW.h;

	}

	
	while(i<20)
	{
			CW.iw = IHR_720/4;
			CW.ih = IVR;
			CW.w = IHR_720/4;
			CW.h = IVR;
			CW.ix = CW.iw*(i%4);
			CW.iy = UVR;
			CW.x  = CW.w*(i%4);
			CW.y  = PVR;

			memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

			ChannelDB[i].chno = PB1+(i-16);
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].ctrl = MAINON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			i++;

		
	}

}

//---------------------------------------------
void LoadMdspDb_16Live_HDSpot1(void)
{
	// display 4x4live window + 4hd + 1hd_spot
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/5/4;
	CW.ih = UVR/4;
	CW.w = PHR/5/4;
	CW.h = PVR/4;

	while(i<20)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		if(i<16) {
			ChannelDB[i].chno = CH1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON |MDBOXON;
		}
		else{
			ChannelDB[i].chno = PB1+(i-16);
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].ctrl = MAINON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.ix = CW.iw*(i%4);
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%4);
			if((i%4)==0) CW.y += CW.h;

		}
		else if(i<20) {
			CW.ix = (UHR/4-CW.iw);
			CW.iy = CW.ih*(i-16);

			CW.x = (PHR/4-CW.w);
			CW.y = CW.h*(i-16);

		}
	}

	
	i=20;
	{
			CW.iw = IHR_720/4;
			CW.ih = IVR;
			CW.w = IHR_720/4;
			CW.h = IVR;
			CW.ix = 0;
			CW.iy = UVR;
			CW.x  = 0;
			CW.y  = PVR;

			memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

			ChannelDB[i].chno = PB5;
			ChannelDB[i].port = PORT_PB_5;
			ChannelDB[i].ctrl = MAINON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
	
	}

}
//-------------------------------------------
void LoadMdspDb_16Live_HDSpot2(void)
{
	// display 4x4live window + hd_quad_spot(pb1-4)
	int i=0;
	PBAutoMode = 0; // PB Normal Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	CW.ih = UVR/4;
	CW.w = PHR/4/4;
	CW.h = PVR/4;

	while(i<16)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		ChannelDB[i].port = PORT_LIVE_0+i;
		ChannelDB[i].ctrl = MAINON | MDBOXON;
	
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		CW.ix = CW.iw*(i%4);
		if((i%4)==0) CW.iy += CW.ih;
		CW.x = CW.w*(i%4);
		if((i%4)==0) CW.y += CW.h;

	}

	
	while(i<=20)
	{
			CW.iw = IHR_720/2/4;
			CW.ih = IVR/2;
			CW.w = IHR_720/2/4;
			CW.h = IVR/2;
			CW.ix = CW.iw*((i-16)%5);
			CW.iy = UVR;
			CW.x  = CW.w*((i-16)%5);
			CW.y  = PVR;

			memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

			ChannelDB[i].chno = PB1+(i-16);
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].ctrl = MAINON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;

			i++;

		
	}
}
//-------------------------------------------
void LoadMdspDb_12Live_Master(void)
{
	// master 4x4 live : win 12 ~ 16 will be empty. Expected fill with Slave 4 HD. 
	
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	CW.ih = UVR/8;
	CW.w = PHR/4/4;
	CW.h = PVR/8;

	while(i<17)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		
		if(i<16) {
			ChannelDB[i].chno = CH1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON;
		}
		else{
			ChannelDB[i].chno = PB1+(i-16);
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].ctrl = 0;//MAINON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.ix = CW.iw*(i%4);
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%4);
			if((i%4)==0) CW.y += CW.h;

		}
		else if(i==16) {
			CW.ix = 0;
			CW.iy = CW.ih*4;

			CW.x = 0;
			CW.y = CW.h*4;

		}
	}

}

void LoadMdspDb_16LIVE_4HD(void)
{
	// display 4x4live + 4xhdpb  window
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll();
	
	CW.iw = UHR/5/4;
	CW.ih = UVR/4;
	CW.w  = PHR/5/4;
	CW.h  = PVR/4;

	while(i<20)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));
		 
		if(i<16) {
			ChannelDB[i].chno = CH1+i;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].ctrl = MAINON | MDBOXON;
		}
		else{
			ChannelDB[i].chno = PB1+(i-16);
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].ctrl = MAINON | MDBOXON;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		
		#ifdef DEBUG_MDSP
		printk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		printk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		if(i<16) {
			CW.ix = CW.iw*(i%4);
			if((i%4)==0) CW.iy += CW.ih;

			CW.x = CW.w*(i%4);
			if((i%4)==0) CW.y += CW.h;

		}
		else if(i<20) {
			CW.ix = (UHR/4-CW.iw);
			CW.iy = CW.ih*(i-16);

			CW.x = (PHR/4-CW.w);
			CW.y = CW.h*(i-16);

		}
	}

	
}

//------------------------------------------------------

void LoadMdspDb_5PB_5LIVE(void)
{
	// Autochannel id insertion demo by cropping (4*HD to 16 cut PB)
  
	u8 i=0;
	u16 k, k1;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/5/4;
	CW.ih = UVR/5;
	CW.w = PHR/5/4;
	CW.h = PVR/5;

	k=UHR/5/4;
	k1=PHR/5/4;

	while(i<36)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;
		ChannelDB[i].cx_unit = 1; //1:4pixel, 2:8pixel
		ChannelDB[i].cy_unit = 2; //2:2line,  4:4line 

		if(i<5) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON;
		}
		else if(i>=16) {
			ChannelDB[i].ctrl = MAINON  | AUTOMODEON | INSERTCHID;

			ChannelDB[i].tw = CW.iw*2;
			ChannelDB[i].th = CW.ih*2;
			ChannelDB[i].cx = (i%2)*CW.iw/ChannelDB[i].cx_unit;
			ChannelDB[i].cy = ((i%4)>>1)*CW.ih/ChannelDB[i].cy_unit;
		}
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d : %04d, %04d ",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h, \
		     ChannelDB[i].cx, ChannelDB[i].cy);
		#endif

		i++;
		
		switch(i) {
		case 0: case 1: case 2: case 3:	case 4: CW.ix=0;	CW.x=0;  break;
		case 16: case 18: case 24: case 26:	case 32: CW.ix=k;	  CW.x=k1;   break;
		case 17: case 19: case 25: case 27:	case 33: CW.ix=k*2;	CW.x=k1*2; break;
		case 20: case 22: case 28: case 30:	case 34: CW.ix=k*3;	CW.x=k1*3; break;
		case 21: case 23: case 29: case 31:	case 35: CW.ix=k*4;	CW.x=k1*4; break;
		}
		
		switch(i) {
		case 0: case 16: case 17: case 20: case 21:	CW.iy=0;	      CW.y=0; break;
		case 1: case 18: case 19: case 22: case 23:	CW.iy=CW.ih;	  CW.y=CW.h; break;
		case 2: case 24: case 25: case 28: case 29:	CW.iy=CW.ih*2;	CW.y=CW.h*2; break;
		case 3: case 26: case 27: case 30: case 31:	CW.iy=CW.ih*3;	CW.y=CW.h*3; break;
		case 4: case 32: case 33: case 34: case 35:	CW.iy=CW.ih*4;	CW.y=CW.h*4; break;
		}
		

	}

}

//------------------------------------------------------

void LoadMdspDb_5PB_1LIVE(void)
{
	// Normal mode to test PB5

	u8 i=0;
	u16 k, k1;

	PBAutoMode = 0; 

	CleanCWDataBaseAll(); 

	CW.iw = UHR/3/4;
	CW.ih = UVR/3;
	CW.w  = PHR/3/4;
	CW.h  = PVR/3;

	k=UHR/3/4;
	k1=PHR/3/4;


	while(i<21)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		if((i>=16 && i<=19) || (i==1) ){
			ChannelDB[i].ctrl = MAINON | MDBOXON;
			ChannelDB[i].port = PORT_LIVE_0+i;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		if(i==20) {
			ChannelDB[i].ctrl = MAINON | MDBOXON;
			ChannelDB[i].port = PORT_PB_5;
			ChannelDB[i].chno = PB5;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}
		
		
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d : %04d, %04d ",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h, \
		     ChannelDB[i].cx, ChannelDB[i].cy);
		#endif

		i++;
		
		switch(i) {
		case 18:          CW.ix=0;	    CW.x=0;   break;
		case 19:          CW.ix=k*1;	CW.x=k1*1; break;
		case 1: case 16: case 17: CW.ix=k*2;	CW.x=k1*2; break;
		case 20:		  CW.ix=0;	    CW.x=0;   CW.iw=k*2; CW.w=k1*2; break;
		}
		
		switch(i) {
		case 16:			CW.iy=0;	      CW.y=0; break;
		case 17: 	        CW.iy=CW.ih;	  CW.y=CW.h; break;
		case 1: case 18: case 19:   CW.iy=CW.ih*2;	  CW.y=CW.h*2; break;
		case 20:   CW.iy=0;	CW.y=0; CW.ih=UVR*2/3; CW.h=PVR*2/3; break;  
		}
	
	}

}

//------------------------------------------------------

void LoadMdspDb_16Live_20PB(void)
{
	// Normal mode to test PB5

	u8 i=0;
	u16 k, k1;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/6/4;
	CW.ih = UVR/6;
	CW.w = PHR/6/4;
	CW.h = PVR/6;

	k=UHR/6/4;
	k1=PHR/6/4;

	while(i<36)
	{

		switch(i) {
		case 16: case 18: case 28: case 30:	case 4: case 10: CW.ix=0;	CW.x=0;      break;
		case 17: case 19: case 29: case 31:	case 5: case 11: CW.ix=k;	CW.x=k1;     break;
		case 20: case 22: case 32: case 34:	case 6: case 12: CW.ix=k*2;	CW.x=k1*2; break;
		case 21: case 23: case 33: case 35:	case 7: case 13: CW.ix=k*3;	CW.x=k1*3; break;
		case 24: case 26: case 0: case 2:	case 8: case 14:   CW.ix=k*4;	CW.x=k1*4; break;
		case 25: case 27: case 1: case 3:	case 9: case 15:   CW.ix=k*5;	CW.x=k1*5; break;
		}
		
		switch(i) {
		case 16: case 17: case 20: case 21: case 24:	case 25: CW.iy=0;	      CW.y=0;      break;
		case 18: case 19: case 22: case 23: case 26:	case 27: CW.iy=CW.ih;	  CW.y=CW.h;   break;
		case 28: case 29: case 32: case 33: case 0:	case 1:    CW.iy=CW.ih*2;	CW.y=CW.h*2; break;
		case 30: case 31: case 34: case 35: case 2:	case 3:    CW.iy=CW.ih*3;	CW.y=CW.h*3; break;
		case 4: case 5: case 6: case 7: case 8:	case 9:        CW.iy=CW.ih*4;	CW.y=CW.h*4; break;
		case 10: case 11: case 12: case 13: case 14:	case 15: CW.iy=CW.ih*5;	CW.y=CW.h*5; break;
		}

		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;
	  ChannelDB[i].cx_unit = 2; //1:4pixel, 2:8pixel
	  ChannelDB[i].cy_unit = 4; //2:2line,  4:4line 

		ChannelDB[i].ctrl = MAINON  | AUTOMODEON | INSERTCHID;
		
		if(i>=16) {
			ChannelDB[i].tw = CW.iw*2;
			ChannelDB[i].th = CW.ih*2;
			ChannelDB[i].cx = (i%2)*CW.iw/ChannelDB[i].cx_unit;
			ChannelDB[i].cy = ((i%4)>>1)*CW.ih/ChannelDB[i].cy_unit;
		}

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d : %04d, %04d ",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h, \
		     ChannelDB[i].cx, ChannelDB[i].cy);
		#endif

		i++;
		
	}
	
}

//------------------------------------------------------

void LoadMdspDb_PB5toLive_4HD(void)
{
	// Normal mode to test PB5

	u8 i=0;
	u16 k, k1;

	PBAutoMode = 1; // PB Auto Mode

	CleanCWDataBaseAll(); 

	CW.iw = UHR/6/4;
	CW.ih = UVR/6;
	CW.w = PHR/6/4;
	CW.h = PVR/6;

	k=UHR/6/4;
	k1=PHR/6/4;

	while(i<36)
	{

		switch(i) {
		case 16: case 18: case 28: case 30:	case 4: case 10: CW.ix=0;	CW.x=0;      break;
		case 17: case 19: case 29: case 31:	case 5: case 11: CW.ix=k;	CW.x=k1;     break;
		case 20: case 22: case 32: case 34:	case 6: case 12: CW.ix=k*2;	CW.x=k1*2; break;
		case 21: case 23: case 33: case 35:	case 7: case 13: CW.ix=k*3;	CW.x=k1*3; break;
		case 24: case 26: case 0: case 2:	case 8: case 14:   CW.ix=k*4;	CW.x=k1*4; break;
		case 25: case 27: case 1: case 3:	case 9: case 15:   CW.ix=k*5;	CW.x=k1*5; break;
		}
		
		switch(i) {
		case 16: case 17: case 20: case 21: case 24:	case 25: CW.iy=0;	      CW.y=0;      break;
		case 18: case 19: case 22: case 23: case 26:	case 27: CW.iy=CW.ih;	  CW.y=CW.h;   break;
		case 28: case 29: case 32: case 33: case 0:	case 1:    CW.iy=CW.ih*2;	CW.y=CW.h*2; break;
		case 30: case 31: case 34: case 35: case 2:	case 3:    CW.iy=CW.ih*3;	CW.y=CW.h*3; break;
		case 4: case 5: case 6: case 7: case 8:	case 9:        CW.iy=CW.ih*4;	CW.y=CW.h*4; break;
		case 10: case 11: case 12: case 13: case 14:	case 15: CW.iy=CW.ih*5;	CW.y=CW.h*5; break;
		}

		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].port = tb_36CH[i].port;
		ChannelDB[i].chno = tb_36CH[i].chno;
		ChannelDB[i].tw = CW.iw*4;
		ChannelDB[i].th = CW.ih*4;
		ChannelDB[i].cx_unit = 2; //1:4pixel, 2:8pixel
		ChannelDB[i].cy_unit = 4; //2:2line,  4:4line 

		ChannelDB[i].ctrl = MAINON | AUTOMODEON;
		
		if(i>=16) {
			ChannelDB[i].cx = (i%2)*CW.iw/ChannelDB[i].cx_unit;
			ChannelDB[i].cy = ((i%4)>>1)*CW.ih/ChannelDB[i].cy_unit;
		}

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d : %04d, %04d ",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h, \
		     ChannelDB[i].cx, ChannelDB[i].cy);
		#endif

		i++;
		
	}
	
}

//------------------------------------------------------
void LoadMdspDb_4CH_HDSpot_Demo(void)
{
	// display 4*HD pb

	u8 i=0;
	u16 k = UHR/2;
	u16 k1 = PHR/2;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll(); 

	CW.iw = k/4;
	if((UVR/2)<540) CW.ih = UVR/2;
	else 	CW.ih = 540;
	CW.w = k1/4;
	if(CW.ih == (UVR/2)) CW.h = PVR/2;
	else CW.h = 540; //480;

	i=16;
	while(i<=19)
	{
		if( i==16 || i==18 ) CW.ix = 0;
		else CW.ix=k/4;	
		if(i<18)  CW.iy=0;
		else  CW.iy = CW.ih;

		if( i==16 || i==18 ) CW.x = 0;
		else CW.x=k1/4;	
		if(i<18)  CW.y=0;
		else  CW.y = CW.h;

		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].chno = CH1+i;
		ChannelDB[i].port = PORT_PB_1+(i-16);
		ChannelDB[i].ctrl = MAINON |MDBOXON;
		ChannelDB[i].tw = CW.iw;
		ChannelDB[i].th = CW.ih;
		
		//printk("\n - %d %d", ChannelDB[i].pw, ChannelDB[i].ph);
		
		#ifdef DEBUG_MDSP
		printk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		printk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;
	
	}

	{ //pb5
			CW.ix = 0;
			CW.iy = UVR;
			CW.x = 0;
			CW.y = PHR;
			CW.iw = IHR_720/4;
		  CW.ih = 480;
	    CW.w = IHR_720/4;
			CW.h =  480;
			
			memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

			ChannelDB[i].ctrl = MAINON;
			ChannelDB[i].port = PORT_PB_5;
			ChannelDB[i].chno = PB5;
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
	}
	
	//---

	

}

void LoadMdspDb_4CH(void)
{
	// display 2x2 live window: ((0,0),(0,1) --> no v.dns; (1,0),(1,0) --> v.dns 
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode

	CleanCWDataBaseAll();

	CW.iw = IHR/4;
	CW.w = IHR/4;
	CW.h= CW.ih = IVR;

//	CW.ih = CW.iw * 3 ;
//	CW.h = CW.w * 3;

	
#if 0
	CW.iw = UHR/2/4;
	CW.ih = UVR/2;
	CW.w = UHR/2/4;
	CW.h = UVR/2;
#endif

//	CW.iw = CW.ih / 3;
//	CW.w = CW.h / 3;

	while(i< 19)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		ChannelDB[i].ctrl = MAINON|MDBOXON;
//		ChannelDB[i].chno = CH1+i;
		ChannelDB[i].chno = CH1 + (i/2) * 4  + i%2;;
		//ChannelDB[i].chno = CH1 +  (windows_cam_id[i]/2) * 4  + windows_cam_id[i]%2;
		//int j = win_mode_cache.cam_id[i];
		//ChannelDB[i].chno = CH1 +  (j/2) * 4  + j%2;
		ChannelDB[i].port = PORT_LIVE_0+i;

		if (i >=4)  ChannelDB[i].ctrl = OFF;

		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

	    CW.ix = CW.iw*(i%2);
        CW.iy = CW.ih*(i/2);

        CW.x = CW.w*(i%2);
        CW.y = CW.h*(i/2);

	}

	cur_logo = &logo_info[LOGO_4ch];
	cur_logo->x0 = CW.w * 2 * 4;
	cur_logo->y0 = 0x0;
	cur_logo->width = UHR - cur_logo->x0;
	cur_logo->height = UVR;
	cur_logo->logo_data = logodata_4ch;
}


void LoadMdspDb_8CH(void)
{

	// display 4x4live window
	u8 i=0;

	PBAutoMode = 0; // PB Normal Mode
	CleanCWDataBaseAll();

	CW.iw = UHR/4/4;
	//CW.ih = UVR/4;
	CW.ih = (CW.iw * 3 );  // the window is 4:3
	CW.w = PHR/4/4;
	//CW.h = PVR/4;
	CW.h = (CW.w * 3 );

	CW.iy += (UVR - (CW.ih << 1)) >> 1;  // (UVR - CW.ih * 2) / 2;
	CW.y  += (PVR - (CW.h  << 1)) >> 1;  // (PVR - CW.h * 2) / 2;

	while(i<8)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));


		//ChannelDB[i].chno = CH1+i;
		//ChannelDB[i].port = PORT_LIVE_1+i;
		ChannelDB[i].chno = CH1+ (i/2) * 4  + i%2 ;
		ChannelDB[i].port = PORT_LIVE_1+ (i/2) * 4 + i % 2;
		ChannelDB[i].ctrl = MAINON | MDBOXON;
	
		#ifdef DEBUG_MDSP
		dprintk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		dprintk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
		#endif
		
		i++;

		CW.ix = CW.iw*(i%4);
		if((i%4)==0) CW.iy += CW.ih;
		
		CW.x = CW.w*(i%4);
		if((i%4)==0) CW.y += CW.h;

	}

	ChannelDB[8].ctrl = 0x0; //MAINON | MDBOXON;
	ChannelDB[8].chno = CH3;
 	ChannelDB[8].port = PORT_LIVE_3;
	ChannelDB[8].ix = 0x0;
	ChannelDB[8].iy = 0x0;
	ChannelDB[8].x = 0x0;
	ChannelDB[8].y = 0x0;
	ChannelDB[8].iw = UHR >> 2;
	ChannelDB[8].ih = (UVR - (CW.ih << 1)) >> 1;;
	ChannelDB[8].w = PHR >> 2;
	ChannelDB[8].h = (PVR - (CW.h  << 1)) >> 1;;
		
	ChannelDB[9].ctrl = 0x0; //MAINON | MDBOXON;
	ChannelDB[9].chno = CH4;
 	ChannelDB[9].port = PORT_LIVE_4;
	ChannelDB[9].ix = 0x0;
	ChannelDB[9].iy = ChannelDB[8].ih + (CW.ih << 1);
	ChannelDB[9].x = 0x0;
	ChannelDB[9].y = ChannelDB[8].h + (CW.h << 1);
	ChannelDB[9].iw = UHR >> 2;
	ChannelDB[9].ih = (UVR - (CW.ih << 1)) >> 1;;
	ChannelDB[9].w = PHR >> 2;
	ChannelDB[9].h = (PVR - (CW.h  << 1)) >> 1;;
}



void LoadMdspDb_9CH(void)
{
	// display 3x3 live window 
	int i;
	PBAutoMode = 0; // PB Normal Mode
//	printk("[%s]zzc \n", __func__);

	CleanCWDataBaseAll();
	
	CW.iw = UHR/3/4;
	CW.ih = UVR/3;
	CW.w = PHR/3/4;
	CW.h = PVR/3;
	
	i=0;
	while(i<19)
	{
		memcpy(&ChannelDB[i], &CW, sizeof(_ChannelInfo));

		//if(i<6) ChannelDB[i].ctrl = MAINON|MDBOXON;
		//if( i<9 && i > 0) ChannelDB[i].ctrl = MAINON |MDBOXON;
		if( i < 8 ) ChannelDB[i].ctrl = MAINON |MDBOXON;
//		else if( (i>=16) && (i<=18)) ChannelDB[i].ctrl = MAINON |MDBOXON ;
		else 	ChannelDB[i].ctrl = OFF; 
		
	//	ChannelDB[i].chno = CH1+i;
	//	if(i > CH4) ChannelDB[i].chno  = CH1+i + 8;
#if defined VIDEO_2D1
		ChannelDB[i].chno = CH1 +  (i/2) * 4  + i%2;
#else
		ChannelDB[i].chno = CH1 + i;
	#error "Notice, we use 2d1 by default"
#endif
		//ChannelDB[i].chno = CH1 +  (windows_cam_id[i]/2) * 4  + windows_cam_id[i]%2;
		//int j = win_mode_cache.cam_id[i];
		//ChannelDB[i].chno = CH1 +  (j/2) * 4  + j%2;
		if(i<16)
			ChannelDB[i].port = PORT_LIVE_0 + i;//(i/2) * 4  + i%2;
		else {
			ChannelDB[i].port = PORT_PB_1+(i-16);
			ChannelDB[i].tw = CW.iw;
			ChannelDB[i].th = CW.ih;
		}

	//	#ifdef DEBUG_MDSP
		printk("\r\n - %02x - %02x - %02x - %02x : ",i,ChannelDB[i].chno,ChannelDB[i].port,(BYTE)ChannelDB[i].ctrl);
		printk("- %04d - %04d - %04d - %04d -",ChannelDB[i].x,ChannelDB[i].y,ChannelDB[i].w,ChannelDB[i].h);
	//	#endif
		
		i++;

#if 1
	    if(i<16) {
			CW.ix = CW.iw*(i%3);
			CW.iy = CW.ih*(i/3);

			CW.x = CW.w*(i%3);
			CW.y = CW.h*(i/3);
		  }
	    else if((i>=16)&&(i<=18) ) {
			CW.ix = CW.iw*2;
			CW.iy = CW.ih*(i-16);

			CW.x = CW.w*2;
			CW.y = CW.h*(i-16);
		}
#else
	    if(i<16) {
			CW.ix = CW.iw*(i%4);
			CW.iy = CW.ih*(i/2);

			CW.x = CW.w*(i%4);
			CW.y = CW.h*(i/2);
		  }
	    else if((i>=16)&&(i<=18) ) {
			CW.ix = CW.iw * (i - 16);
			CW.iy = CW.ih * 3;

			CW.x = CW.w * (i - 16);
			CW.y = CW.h * 3;
		}

#endif
	}

	cur_logo = &logo_info[LOGO_9ch];
	cur_logo->x0 = CW.w * 2 * 4;
	cur_logo->y0 = CW.h * 2;
	cur_logo->width = CW.w * 4;
	cur_logo->height = CW.h;
	cur_logo->logo_data = logodata_9ch;  
	
#if 0
	ChannelDB[8].ctrl = MAINON;// | MDBOXON;
	ChannelDB[8].chno = CH15;
 	ChannelDB[8].port = CH15;
	ChannelDB[8].ix = CW.iw * 2;
	//ChannelDB[8].iy = CW.ih * 2 + osd_height;
	ChannelDB[8].iy = UVR - osd_height - 15;
	ChannelDB[8].x =  CW.w * 2;
	//ChannelDB[8].y =  CW.h * 2  + osd_height;
	ChannelDB[8].y =  UVR  - osd_height - 15;
	ChannelDB[8].iw = CW.iw;
	ChannelDB[8].ih = osd_height;
	ChannelDB[8].w = CW.w;
	ChannelDB[8].h = osd_height;
		
	ChannelDB[9].ctrl = MAINON;// | MDBOXON;
	ChannelDB[9].chno = CH16;
 	ChannelDB[9].port = CH16;
	ChannelDB[9].ix = CW.iw * 2;
	//ChannelDB[9].iy = CW.ih * 2 + osd_height * 2;
	ChannelDB[9].iy = UVR - osd_height * 2 - 15;
	ChannelDB[9].x =  CW.w * 2;
	//ChannelDB[9].y =  CW.h * 2 + osd_height * 2;
	ChannelDB[9].y =  UVR - osd_height * 2 -15;
	ChannelDB[9].iw = CW.iw;
	ChannelDB[9].ih = osd_height;
	ChannelDB[9].w = CW.w;
	ChannelDB[9].h = osd_height;
#endif
}

//# if 0
// Display logo graphic image in the middle of 32 channel window.

void DrawLogo(u8 id)
{
	if (cur_logo->logo_data == NULL) return;
	printk("\nx: %d,  y: %d,  w: %d,  h: %d\n", 
		cur_logo->x0, cur_logo->y0, cur_logo->width, cur_logo->height);
	TWMux_SetYUVImageToLogoWindow(id, cur_logo->x0, cur_logo->y0, 
		cur_logo->width, cur_logo->height, 
		IMAGE_RAM_START + (uint32_t)cur_logo->logo_data);
}


void DrawGraphicImage(u8 id)
{
	u32 addr;
	u32 x, y, w, h;
  _Mdrs mdrs;
  
	w = ChannelDB[0].iw*4;
	h = ChannelDB[0].ih;
	x = w*2;
	w*=3;
	y = ChannelDB[0].iy + (h*2);
  mdrs = TWMux_GetMainDisplayResolution(id);
	
#if 0 
	switch(mdrs)
	{

#ifdef SUPPORT_SXGA60
	case rSXGA60:	addr = bmptechwell_sxga_540x204Addr;	break;
#endif
#ifdef SUPPORT_HD1080P
	case rHD1080P:	addr = bmppicture_1080p_816x216Addr;	break;
#endif
	default:				addr = bmppicture_1080p_816x216Addr;	break;
	}
#endif
	addr = 0x0;
	//addr = (u32)logoData;  // do not use it now
	TWMux_SetYUVImageToLogoWindow(id, x,y,w,h,IMAGE_RAM_START+addr);

}
//#endif

u8 GetWinCursor(void)
{
	int i;
	
	for(i=0; i<36; i++)
	 if((ChannelDB[i].ctrl&MAINON)==MAINON) break;
	
	return (u8)i;
}	
	
void DisplayMainWindows(u8 id, _ChannelInfo *pC)
{
	_IN_Type pbt;

extern int LOOPBACK[];
	// Recoder
//	if(LOOPBACK[id]==1) {
#if 0
	if (0) {
		InitRecforMdsp(id, CurDisplayMode);
	}
	else {

		pbt = TWMux_GetPBInType(id, 0);
		if(pbt < HD_1080I) {
			InitRecforMdsp(id, CurDisplayMode);
		}
		else {
			if( CurDisplayMode == MODE_16LIVE_HDSPOT) 
				InitSpot_4_HD_Rotate(HostBusNo, SPOT1, PORT5, SEL_PBBUF);
			else if( CurDisplayMode == MODE_16LIVE_HDSPOT1) 
				InitSpot_4_HD_Rotate_by_PB5(HostBusNo, SPOT1, PORT5, SEL_PBBUF);
			else if( CurDisplayMode == MODE_16LIVE_HDSPOT2) 
				InitSpot_HDMix_Quad_Rotate(HostBusNo, SPOT1, PORT5, SEL_PBBUF);
			else if( CurDisplayMode == MODE_16LIVE_HDSPOT3) 
				InitSpot_HD_Quad(HostBusNo, SPOT1, PORT5, SEL_PBBUF);
			else if( CurDisplayMode == MODE_4CH_HDSPOT_DEMO) 
				InitSpot_4_HD_Rotate_by_PB5(HostBusNo, SPOT1, PORT5, SEL_PBBUF);
			else
				InitSpot_16_D1_Rotate(HostBusNo, SPOT1, PORT5, SEL_RBUF);	
				
			InitSpotAll_16CIF_OneStop(HostBusNo, SPOT2, PORT6, SEL_SBUF);
		}
	}
#endif
	{ 
		int x = pC->x * 4 , y = pC->y, w = pC->w * 4, h = pC->h;
		x = 0; y = 0;
		//TWMux_WriteDisplayToMemory(logoData,  x ,y+DISPLAYBASE);
	}

	TWMux_MakeMainChannelWindows(id, pC);
	
	WinCursor = GetWinCursor();
	//TWMux_SetSingleBoxWindowOn(id, 0, 0xff, 0, 0, WinCursor);
	//TWMux_SetSingleBoxWindowOn(id, 0, 0xff, 0, 0, 0x1);

	if( CurDisplayMode == MODE_32CH) 
		DrawGraphicImage(id);

	extern void change_logo_info(MDPS_t mode);

	if (CurDisplayMode == MODE_9CH || CurDisplayMode == MODE_6CH ||
		CurDisplayMode == MODE_4CH) {
#if defined (LOGO_LATE)
#else
		DrawLogo(id);
#endif
		// if do not support multi mode change, 
		// do not need change_logo_info here
		//change_logo_info(CurDisplayMode);
	}
	#ifdef SUPPORT_MDBOX
	TWMux_InitMotionBox(id);
	#endif

	printk("\n[%d] DisplayMainWindows --- ", id);
}

void ChangeChannel(u8 id, u8 srcno, u8 desno)
{
#ifndef __APP__ 
	//_ChannelInfo tmp;
   u16 ix, iy, iw, ih, x, y, w, h;
  
	//if(CurDisplayMode == MODE_6VGA_16CUT) return; 
	//if(CurDisplayMode == MODE_12CH_CASCADE_Master) return;
	if( CurDisplayMode == MODE_6CH ) return;  
	//if( CurDisplayMode == MODE_4HD_PB ) return;  

		ix = ChannelDB[desno].ix; 
		iy = ChannelDB[desno].iy; 
		iw = ChannelDB[desno].iw; 
		ih = ChannelDB[desno].ih; 
		x  = ChannelDB[desno].x; 
		y  = ChannelDB[desno].y; 
		w  = ChannelDB[desno].w; 
		h  = ChannelDB[desno].h; 

		ChannelDB[desno].ix = ChannelDB[srcno].ix;
		ChannelDB[desno].iy = ChannelDB[srcno].iy;
		ChannelDB[desno].iw = ChannelDB[srcno].iw;
		ChannelDB[desno].ih = ChannelDB[srcno].ih;
		ChannelDB[desno].x  = ChannelDB[srcno].x; 
		ChannelDB[desno].y  = ChannelDB[srcno].y; 
		ChannelDB[desno].w  = ChannelDB[srcno].w; 
		ChannelDB[desno].h  = ChannelDB[srcno].h; 

		ChannelDB[srcno].ix = ix; 
		ChannelDB[srcno].iy = iy; 
		ChannelDB[srcno].iw = iw; 
		ChannelDB[srcno].ih = ih; 
		ChannelDB[srcno].x  = x; 
		ChannelDB[srcno].y  = y; 
		ChannelDB[srcno].w  = w; 
		ChannelDB[srcno].h  = h; 

		//Select_LIVE_OR_PB(ChannelDB[desno].chno);
		TWMux_SetMainChannel(HostBusNo, &ChannelDB[desno] );
		WinCursor = desno;

		//Select_LIVE_OR_PB(ChannelDB[srcno].chno);
		TWMux_SetMainChannel(HostBusNo, &ChannelDB[srcno] );
		
		#ifdef SUPPORT_MDBOX
		TWMux_InitMotionBox(id);
		#endif

		//???EnablePosUpScale();
#endif		

}

void MdspAction(u8 id, MDPS_t m)
{
	int i;
	extern void EnablePosUpScale(void);
	for(i=0; msdpTbl[i].mdsp!=MAXMODE; i++)
	{
		if(msdpTbl[i].mdsp == m) break;
	}
	
	if(msdpTbl[i].getChannelDB) {
		int extra_osd_offset = 0x0;
		msdpTbl[i].getChannelDB();
		CurDisplayMode = m;

	  	DisplayMainWindows(id, (_ChannelInfo *)&ChannelDB);

#if 0
		extra_osd_offset = (ChannelDB[0].w << 2) - 10;
		WriteP(0x83, extra_osd_offset & 0xff);
		WriteP(0x84, (extra_osd_offset >> 8) & 0xff);
#endif

#if 0	
extern void MainUpscale(u16 h, u16 v);
extern void MainUpscaleWindow(u16 h, u16 v);
extern void MainPosUpWindow(u16 h, u16 v);
extern void DISize(u16 h, u16 v);

		if(m == MODE_4CH) {
			u32  xr, yr;
			u32 pw, ph, phr, pvr;
			
			phr = ChannelDB[0].w << 1;
			pvr = ChannelDB[0].h;
		
			pw = tw2828devs[id].ilvp.IHR;
 	        ph = tw2828devs[id].ilvp.IVR;
	
			
			xr = (pw*4096 + phr/2 )/phr;
			yr = (ph*1024 + pvr/2 )/pvr;
			
			if(tw2828devs[id].INTERLACED == 1)
			    yr = yr/2;

			MainUpscale(xr, yr);
			
			xr = (phr*4096 + pw-1 )/pw;
		    yr = (pvr*4096 + ph/2 )/ph;
			MainPosUpWindow(xr, yr);
			MainUpscaleWindow(phr, pvr);
			DISize(pw,ph/2);

		}
#endif
		EnablePosUpScale();

	}

}

//-------------------------------------------
/*! 
 * \brief
 *    Change Display Division mode
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      (1) Set RGB winodw size, position \n
 *      (2) Set Live/PB downscaler\n
 * \par Usage: \n
 *      mdsp [mode <n>] : \n
 *     	n- 0:20ch 1:6ch 2:12ch 3:4HD 4:1HD 5:2HD 6:16PB_4LV 7:16SD+4HD \n
 *     	8:16QuadPB a:32ch b:2x8D1 c:4cut4d1 d:4cut6VGA \n
 *     	11:32CH 13:32CH 15:16LV+1HD 16:1HD+8D1PB 19:1HD_16CUT \n
 *     	1a:1_HD16CUT+8LV 1b:5PB_5LIVE_automode 1c:5PB_1LV 1d:36CH \n
 *      mdsp win <win1> <win2>  - swap win1 to win2 \n
 *      mdsp ch <winno> <ch>  - Change winno'channel to ch\n
 *      mdsp full <n>  - full n ch \n
 *      mdsp zoom on <x> <y> <w> <h>  - zoom on \n
 *      mdsp zoom off  - recover from zoom
 **/

int MainDisplayTest(int argc, char **argv)
{
	GetIOInfoData(HostBusNo);
	
	if ( !strcmp( (char *)argv[1], "mode" ) ) {
		if(argc == 3) {
			printk("[%s] zzc call MdspAction\n", __func__);
			MdspAction(HostBusNo, Asc2Bin( argv[2]));
			return 1;
		}
	}

	else if ( !strcmp( (char *)argv[1], "win" ) ) {
		ChangeChannel( HostBusNo, Asc2Bin( argv[2]), Asc2Bin( argv[3] ));	
		return 1;

	}
	else if ( !strcmp( (char *)argv[1], "ch" ) ) {
		TWMux_SetVIChno( HostBusNo, Asc2Bin( argv[2]), Asc2Bin( argv[3] ));	
		return 1;

	}

	else if ( !strcmp( (char *)argv[1], "full" ) ) {
		TWMux_MakeFullScreen(HostBusNo, Asc2Bin( argv[2]));	
		return 1;
	}
	else if ( !strcmp( (char *)argv[1], "zoom" ) ) {


		if( !strcmp( (char *)argv[2], "off" ) ) {
			TWMux_SetZoomScreenOff(HostBusNo);
			return 1;
		}

		else if( !strcmp( (char *)argv[2], "on" ) ) {

			if(argc==7) {

				u32 x, y, w, h;

				x=(unsigned int) Asc2Word( argv[3] );
				y=(unsigned int) Asc2Word( argv[4] ); 
				w=(unsigned int) Asc2Word( argv[5] ); 
				h=(unsigned int) Asc2Word( argv[6] ); 
		
				TWMux_SetZoomScreen(HostBusNo,x,y,w,h);
				return 1;

			}
		}

	}

	if( argc<3){
	
			printk("\nUsage: mdsp mode [<n>] ");
			printk("\n\tn- 0:20ch 1:6ch 2:12ch 3:4HD 4:1HD 5:2HD 6:16PB_4LV 7:16SD+4HD\r\n\t8:16QuadPB a:32ch b:2x8D1 c:4cut4d1 d:4cut6VGA");
			printk("\n\t11:32CH 13:32CH 15:16LV+1HD 16:1HD+8D1PB 19:1HD_16CUT");
			printk("\n\t1a:1_HD16CUT+8LV 1b:5PB_5LIVE_automode 1c:5PB_1LV 1d:36CH");
			printk("\n       mdsp ch <n>  - Change ch to n");
			printk("\n       mdsp full <n>  - full n ch");
			printk("\n       mdsp zoom on <x> <y> <w> <h>  - zoom on");
			printk("\n       mdsp zoom off  - recover from zoom");

	}

	return 1;

}


/*! 
 * \brief
 *    Change Display Resolution
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      (1) Set VCLK \n
 *      (2) Set Display Output Timing.\n
 *      (3) Set HDMI Tx according to mdrs mode.
 * \par Usage:
 *      mdrs [mode <n>] :\n  
 *      0 - 1280*1024/60  \n
 *      1 - HD1080p       \n
 *      2 - 1920*1200     \n
 *      3 - 1680*1050     \n
 *      4 - 720p          \n
 *      5 - 1080i         \n
 *      6 - 1600*900      \n
 *      7 - HD1080p50     \n
 *      8 - 1080i50       \n
 *      9 - 720p50       
 **/
int ChangeMdRsTest(int argc, char **argv)
{
	u8 i;
  _Mdrs mdrs;
	if ( !strcmp( (char *)argv[1], "mode" ) ) {

		if(argc==3) {
      i = (int)Asc2Bin(argv[2]);
			if( i >= rMAX_MDRS ) i=rSXGA60;
		  if(TWMux_SetMainDisplayResolution(HostBusNo, i)) {
		  delay(100);	
			//HDMI_tx_Init(HostBusNo, i);

			//GetIOInfoData(HostBusNo);
			MdspAction(HostBusNo, MODE_16LIVE_4HD);

			return 1;
		 }
		}
	}
	
	{
		  mdrs = TWMux_GetMainDisplayResolution(HostBusNo);
			printk("\r\nCurrent: mdrs[%d] mode=%0x :",HostBusNo, mdrs);
			printk("\nUsage: mdrs mode <n> :");
			i=0;
			while(mTimingTbl[i].str) {
				printk("\n\t%x - %s",i,mTimingTbl[i].str);
				i++;
			}
	}
	return 1;
}


int PrintChannelDataBase(int argc, char **argv)
{
	int i;
	for(i=0; i<TW2828_MAX; i++) {
		TWMux_printChannelDB(i);
	}
	return 0;
}
/** @} */ 
