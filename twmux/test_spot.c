/*! 
 *************************************************************************************
 * \file  spot.c
 *
 * \brief
 *    spot application and test
 *
 * \date
 *    02/20/2012
 *
 * \note 
 *    Spot has only sbuf and encoder output for TW2828.
 *
 * \author
 *    Heejeong Ryu <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *
 */
# if 0
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "twmux.h"
#include "tw2828_hostif.h"
#include "disp.h"
#include "rec.h"
#include "system.h"
#include "spot.h"
#include "spot_tbl.h"
#endif

#include <linux/string.h>
#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_test.h"
//#include "test_rec_tbl.h"
#include "test_spot_tbl.h"
#include "test_spot.h"
_SportInfo	sPort[2];
u32 sRotateTime = 1000;
u8 SpotMode[2];
u32 fgActiveSpotRotate_0 = 1;  //default =1 (Rotate On)
u32 fgActiveSpotRotate_1 = 1;

/** @defgroup SPOT_Test /tw2828Test/SPOT_Test
*   @ingroup tw2828Test
*   @addtogroup SPOT_Test
*   @{
*/


void SetSpotRotateEnable(u8 p)
{
	sPort[p].rotate_en = 1;
}

void SetSpotRotateDisable(u8 p)
{
	sPort[p].rotate_en = 0;
}

void LoadBuffersForRecSpot(u8 no, _RecSpotBuf *buf)
{
	u8 i;
	_RecSpotBuf *Buf;
	
		Buf = (_RecSpotBuf *)buf;
		for(i=0; Buf[i].bf!=NOBUF; i++) {

	//printk("\n- [%d]: rBuf#%d, (%d,%d,%02x,%02x,", no, Buf[i].bf, Buf[i].chno,Buf[i].en,Buf[i].format,Buf[i].res);
	//printk("%02x,%02x)", Buf[i].hposi, Buf[i].vposi );
			if(Buf[i].bf<S_1)
		  	TWMux_LoadRecBuf(no, Buf[i].bf, Buf[i]);
		  else
		  	TWMux_LoadSpotBuf(no, Buf[i].bf-S_1, Buf[i]);

		}
	
}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT 4 HD Rotate by PB5
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      4 HD, Rotate On : (1)->(2)->(3)->(4)-> ..
 * \note PB5 should dedicate to use SPOT. \n
 *       Rotate by changing loop_cnt
 */
void InitSpot_4_HD_Rotate_by_PB5(u8 no, u8 p, u8 rp, u8 buf_sel)
{
  _twSpot_port rTbPort_4HD_PB5 = { rp, ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , sPort[p].buf, p };

	printk("\n[%d]++(InitSpot_4_HD_Rotate_by_PB5)", no);

  if(buf_sel == SEL_SBUF) 
	{
		printk("\r\n Error! HD Spot should use RecBuf!");
		return;
	}
	
	SpotMode[p] = SPOTMODE_HD_PB5;
	
	sPort[p].en = 1;
	sPort[p].rotate_en = 0;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 2;	 // downcounter (min resolution)
	sPort[p].step = 1;
	sPort[p].bsize = 1;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;

	sPort[p].buf[0] = R_PB5;
  
	LoadBuffersForRecSpot(no, (_RecSpotBuf *)pTbBuf_13_D1_Frame); 

  TWMux_SetRecPortForSpot(no, rTbPort_4HD_PB5);

	printspotinfo(p);
}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT HD,SD Mix mode by quad
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      Rotate On : (PB1,CH1,CH2,CH3)->(PB1,PB2,PB3,PB4)->(PB3,CH4,CH5,CH6)->(PB2,CH7,CH8,CH1)->..
 * \note 
 */
void InitSpot_HDMix_Quad_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel)
{
  const u8 HDMIX_SPOT_PORT_TB[] = {R_PB1, R_1, R_2, R_3,
  	                               //R_PB2, R_PB3, R_PB4, R_PB5, 
  	                               R_PB1, R_PB2, R_PB3, R_PB4,
  	                               R_PB2, R_4, R_5, R_6,
  	                               R_PB3, R_7, R_8, R_1,
  	                               //R_PB4, S_2, S_3, S_4,
  	                               //R_PB3, S_5, S_6, S_7,
  	                               };
  int i;  	                               
  _twSpot_port rTbPort = { rp, ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , sPort[p].buf, p };

	printk("\n[%d]++(InitSpot_HDMix_Quad_Rotate)", no);

  if(buf_sel == SEL_SBUF) 
	{
		printk("\r\n Error! HD Spot should use RecBuf!");
		return;
	}
	
	SpotMode[p] = SPOTMODE_HDMIX;
	
	sPort[p].en = 1;
	sPort[p].rotate_en = 1;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 2;	 // downcounter (min resolution)
	sPort[p].step = 4;
	sPort[p].bsize = 16;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;


	for(i=0; i<sPort[p].bsize; i++) {
		sPort[p].buf[i] = HDMIX_SPOT_PORT_TB[i];
  }
  
	LoadBuffersForRecSpot(no, (_RecSpotBuf *)rTbBuf_Mix_CIF_Frame); 

  TWMux_SetRecPortForSpot(no, rTbPort);

	printspotinfo(p);
}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT HD quad
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      Rotate off : (PB1,PB2,PB3,PB4)
 * \note 
 */
void InitSpot_HD_Quad(u8 no, u8 p, u8 rp, u8 buf_sel)
{
  const u8 HDMIX_SPOT_PORT_TB[] = {R_PB1, R_PB2, R_PB3, R_PB4};
  _twSpot_port rTbPort = { rp, ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , sPort[p].buf, p };
  int i;  	                               

	printk("\n[%d]++(InitSpot_HDMix_Quad_Rotate)", no);

  if(buf_sel == SEL_SBUF) 
	{
		printk("\r\n Error! HD Spot should use RecBuf!");
		return;
	}
	
	SpotMode[p] = SPOTMODE_HDQUAD;
	
	sPort[p].en = 1;
	sPort[p].rotate_en = 0;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 2;	 // downcounter (min resolution)
	sPort[p].step = 4;
	sPort[p].bsize = 4;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;


	for(i=0; i<sPort[p].bsize; i++) {
		sPort[p].buf[i] = HDMIX_SPOT_PORT_TB[i];
  }
  
	LoadBuffersForRecSpot(no, (_RecSpotBuf *)rTbBuf_Mix_CIF_Frame); 

  TWMux_SetRecPortForSpot(no, rTbPort);

	printspotinfo(p);
}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT 4 HD Rotate
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      4HD, Rotate On : (1)->(2)->(3)->(4)-> ..
 * \note PB should dedicate to use SPOT.
 */
void InitSpot_4_HD_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel)
{
	u8 i;
  const u8 HD_SPOT_PORT_TB[] = {R_PB1, R_PB2, R_PB3, R_PB4, R_PB5};
  _twSpot_port rTbPort_4_HD = { rp, ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , sPort[p].buf, p };

	printk("\r\n++(InitSpot_4_HD_Rotate)");

  if(buf_sel == SEL_SBUF) 
	{
		printk("\r\n Error! HD Spot should use RecBuf!");
		return;
	}
	
	SpotMode[p] = SPOTMODE_HD;
	
	sPort[p].en = 1;
	sPort[p].rotate_en = 1;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 2;	 // downcounter (min resolution)
	sPort[p].step = 1;
	sPort[p].bsize = 4;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;

	for(i=0; i<sPort[p].bsize; i++) {
		sPort[p].buf[i] = HD_SPOT_PORT_TB[i];
  }
  
	LoadBuffersForRecSpot(no, (_RecSpotBuf *)rTbBuf_9_12_D1_Frame); 

  TWMux_SetRecPortForSpot(no, rTbPort_4_HD);

	printspotinfo(p);
}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT D1 Rotate
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      D1, Rotate On : (1)->(2)->(3)->(4)-> ..
 */
void InitSpot_16_D1_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel)
{
	u8 i;
  _twSpot_port rTbPort_16_D1 = { rp, ON,	D1,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , sPort[p].buf, p };
  
	printk("\r\n++(InitSpot_16_D1_Rotate)");

	SpotMode[p] = SPOTMODE_D1;

	sPort[p].en = 1;
	sPort[p].rotate_en = 1;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 2;	 // downcounter (min resolution)
	sPort[p].step = 1;
	sPort[p].bsize = 4;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;

	if(buf_sel == SEL_SBUF) 
    LoadBuffersForRecSpot(no, (_RecSpotBuf *)sTbBuf_1_8_D1_Frame); 
  else 
    LoadBuffersForRecSpot(no, (_RecSpotBuf *)rTbBuf_1_8_D1_Frame); 
  
	//Set rPort
	for(i=0; i<sPort[p].bsize; i++) {
		sPort[p].buf[i] = NORMAL_REC_PORT_TB[i] | buf_sel;
  }
  
  TWMux_SetRecPortForSpot(no, rTbPort_16_D1);
  
	printspotinfo(p);

}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT 16CIF
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      16CIF, Rotate Off : (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
 */
void InitSpotAll_16CIF_OneStop(u8 no, u8 p, u8 rp, u8 buf_sel)
{
	u8 i;
  const u8 QCIF_SPOT_PORT_TB[] = {S_1, S_3, S_9, S_11};
 _twSpot_port rTbPort_16_QCIF = { rp, ON,	QCIF,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , sPort[p].buf, p };

	printk("\n[%d]++(InitSpotAll_16CIF_OneStop)",no);

	if(buf_sel != SEL_SBUF)  {
		printk("\n Error! 16CIF should use SpotBuf!");
		return;
	}

	SpotMode[p] = SPOTMODE_16CIF;
	
	sPort[p].en = 1;
	sPort[p].rotate_en = 0;
	sPort[p].step = 4;
	sPort[p].bsize = 16;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;

	for(i=0; i<sPort[p].bsize; i++) {
		sPort[p].buf[i] = QCIF_SPOT_PORT_TB[i];
	}

	LoadBuffersForRecSpot(no, (_RecSpotBuf *)sTbBuf_QCIF_Frame); 
	
  TWMux_SetRecPortForSpot(no, rTbPort_16_QCIF);

	printspotinfo(p);

}

//--------------------------------------------------------------------------
/*! 
 * \brief
 *    Init SPOT 9CH
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      9CH, Rotate OFF : (1,2,3,4,5,6,7,8,9)
 */
void InitSpotAll_9CH_OneStop(u8 no, u8 p, u8 rp, u8 buf_sel)
{
	u8 i;
 _twSpot_port rTbPort_9CH = { rp, ON,	CH_9,	IFRAME,	RCLK_27M,	PORT_8BIT,	1 , sPort[p].buf, p };

	printk("\r\n++(InitSpotAll_9CH_OneStop)");

	if(buf_sel != SEL_SBUF)  {
		printk("\r\n Error! 9CH should use SpotBuf!");
		return;
	}

	SpotMode[p] = SPOTMODE_9CH;
	
	sPort[p].en = 1;
	sPort[p].rotate_en = 0;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 2;	 // downcounter (min resolution)
	sPort[p].step = 1;
	sPort[p].bsize = 5;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;

  sPort[p].buf[0] = NORMAL_REC_SPOTBUF_PORT_TB[0];
	for(i=1; i<5; i++) {
		sPort[p].buf[i] = NORMAL_REC_SPOTBUF_PORT_TB[i+8];
	}

	LoadBuffersForRecSpot(no, (_RecSpotBuf *)sTbBuf_9CH_Frame); // 9 Buffer using

  TWMux_SetRecPortForSpot(no, rTbPort_9CH);

	printspotinfo(p);

}

//---------------------------------------------------------------------
/*! 
 * \brief
 *    Init QUAD 16 channel rotate
 *
 * \param [in] no : chip id 
 * \param [in] p  : desire output SPOT port 
 * \param [in] rp : use Record port 
 * \param [in] buf_sel : select SPOT_buf (1/0) 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      Quad, Rotate On : (1,2,3,4)->(5,6,7,8)->(9,10,11,12)->(13,14,15,16)-> .. \n
 *      By updating REC Port Table
 */
void InitSpot_Quad_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel)
{
	u8 i;
 _twSpot_port rTbPort_QUAD = { rp, ON,	QUARD,	IFRAME,	RCLK_27M,	PORT_8BIT,	4 , sPort[p].buf, p };
	
	printk("\r\n++(InitRecSpot_Quad_Rotate)");

	SpotMode[p] = SPOTMODE_QUAD;

	sPort[p].en = 1;
	sPort[p].rotate_en = 1;
	sPort[p].rotate_timer = 2;	 
	sPort[p].rotate_cnt = 10;	 // downcounter (min resolution)
	sPort[p].step = 4;
	sPort[p].bsize = 16;
	sPort[p].curbinx = 0;
  sPort[p].rport = rp;

	for(i=0; i<sPort[p].bsize; i++) {
		sPort[p].buf[i] = NORMAL_REC_PORT_TB[i]|buf_sel;
	}

	if(buf_sel == SEL_SBUF)  {
		LoadBuffersForRecSpot(no, (_RecSpotBuf *)sTbBuf_QUAD_Frame); 
	}
	else {
		LoadBuffersForRecSpot(no, (_RecSpotBuf *)rTbBuf_All_CIF_Frame); 
	  sPort[p].bsize = 8;
	}
		
  TWMux_SetRecPortForSpot(no, rTbPort_QUAD);

	printspotinfo(p);

}

//---------------------------------------------------------------------
/*! 
 * \brief
 *    Spot Init
 *
 * \param [in] id  : chip id 
 * \param [in] e1  : SPOT1 on/off 
 * \param [in] e2  : SPOT2 on/off 
 *
 * \return
 *    <NONE>
 * \par Description:
 */
void SpotInit(u8 id, u8 e1, u8 e2)
{
	u32 p;
	
	printk("\n[%d] Init Spot", id); 
	sPort[0].en = e1;
	sPort[1].en = e2;

	for(p=0; p<2; p++) {
 			if(sPort[p].en == 1)
			{
					switch(p){	
					//case 0:	InitSpotAll_16CIF_OneStop(HostBusNo, SPOT1, PORT5, SEL_SBUF); break;
					//case 0:	InitSpot_Quad_Rotate(HostBusNo, SPOT1, PORT5, SEL_RBUF); break;
					case 0: 
							if( CurDisplayMode == MODE_4CH_HDSPOT_DEMO) 
									InitSpot_4_HD_Rotate_by_PB5(HostBusNo, SPOT1, PORT5, SEL_PBBUF);
							else 
								  InitSpot_16_D1_Rotate(HostBusNo, SPOT1, PORT5, SEL_RBUF); break;
					//case 1: InitSpotAll_9CH_Rotate(HostBusNo, SPOT2, PORT6, SEL_SBUF); break;
					case 1:	InitSpotAll_16CIF_OneStop(HostBusNo, SPOT2, PORT6, SEL_SBUF); break;
					//case 1:	InitSpot_16_D1_Rotate(HostBusNo, SPOT2, PORT6, SEL_SBUF); break;
					}
			}
		}

}

void printspotinfo(u8 p)
{
	u8 i;

  printk("\n --- Info Spot #%d ", p);
  printk("\n\trot_en:%d rot_timer:%dsec ", sPort[p].rotate_en, sPort[p].rotate_timer);
  printk("\n\tstep:%d bsize:%d curbinx:%d \n\tbuf[]=",sPort[p].step,sPort[p].bsize,sPort[p].curbinx);
	for(i=0; i<sPort[p].bsize; i++) {
		printk("%02x ", sPort[p].buf[i]);
  }
}
/*
void WaitSpotVsync(void)
{
	u32 i;
	
	for(i=0; i<0xffffff; i++)
	{
		//if(ReadP(SPOTVSYNC) == 0x01) break;
		delay(1);
	}
	
}
*/
#ifndef __APP__
void ActionSpotPortRotate(u8 id, u8 p)
{
	//u32 i;
	u8 inx;
	//dprintk("\r\n++(CheckSpotRotate)");

	if( sPort[p].rotate_en == 0 ) return;

	if( sPort[p].rotate_cnt == 0 ) 	{

		printk("\r\n +(ActionSpotPortRotate)- ");

		sPort[p].rotate_cnt = sPort[p].rotate_timer;
		
		// Update Spot Table			
		sPort[p].curbinx = sPort[p].curbinx + sPort[p].step;
		sPort[p].curbinx %= sPort[p].bsize;
    inx = sPort[p].curbinx;
		TWMux_SetRecPortTable_Update(id, sPort[p].rport, &sPort[p].buf[inx], 0, sPort[p].step-1);
			
	  printspotinfo(p);

	}
	else {  
		sPort[p].rotate_cnt--;
		//check HD No Video
		
	}

}

void CheckSpotPortRotate(u8 id)
{

	return ;
#if 0
	u32 ct;

	ct = get_tic01();
	if((ct%sRotateTime)!=0) 	return;

	if(fgActiveSpotRotate_0==1)  ActionSpotPortRotate(id, SPOT1);
	if(fgActiveSpotRotate_1==1)  ActionSpotPortRotate(id, SPOT2);
#endif
}
#endif

//---------------------------------------------------------------------
/*! 
 * \brief
 *    SPOT test 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *    spot init <p1> <p2>  : 0=OFF, 1=ON             \n                                                                                                                                                               
 *    spot mode <sp(0-1)> <rp(4-8)> <m> <sbuf(1)>    \n
 *            m: 0:quad,1:QCIF,2:d1,3:hd,4:9CH");   \n                                                                                                                           
 *    spot bufoff  : All Spot buffer write disable    \n
 *    spot rot <port> <0/1> : Spot <port> rotate On/Off(1/0)
 */
int SpotTest(int argc, char **argv)
{
	// command format: 
	if ( !strcmp( (char *)argv[1], "init" ) ) {
		u8 p1, p2;
		p1=1; p2=0;
		p1 =  (Asc2Bin(argv[2])==1)?1:0;
		p2 =  (Asc2Bin(argv[3])==1)?1:0;
		
		SpotInit(HostBusNo, p1, p2);
	}

	else if ( !strcmp( (char *)argv[1], "mode" ) ) {
		u8 sp, rp, buf_sel;
		sp =  Asc2Bin(argv[2]);
		if(sp>1) sp=0;
		rp =  Asc2Bin(argv[3]);
		if((rp<4)||(rp>8)) rp=4;
		rp -= 4;	
		if(	Asc2Bin(argv[5])== 1)
			buf_sel = 0x10;	
		else buf_sel = 0;	

		if ( !strcmp( (char *)argv[4], "0" ) ) {
			InitSpot_Quad_Rotate(HostBusNo, sp, rp,buf_sel);
		}
		else if ( !strcmp( (char *)argv[4], "1" ) ) {
			InitSpotAll_16CIF_OneStop(HostBusNo, sp, rp,buf_sel);
		}
		else if ( !strcmp( (char *)argv[4], "2" ) ) {
			InitSpot_16_D1_Rotate(HostBusNo, sp, rp,buf_sel);
		}
		else if ( !strcmp( (char *)argv[4], "3" ) ) {
			InitSpot_4_HD_Rotate(HostBusNo, sp, rp,SEL_PBBUF);
		}
		else if ( !strcmp( (char *)argv[4], "4" ) ) {
			InitSpotAll_9CH_OneStop(HostBusNo, sp, rp,SEL_SBUF);
		}
		else if ( !strcmp( (char *)argv[4], "5" ) ) {
			InitSpot_4_HD_Rotate_by_PB5(HostBusNo, sp, rp,SEL_PBBUF);
		}
		else
			printk("\r\nUsage- spot mode <sp(0-1)> <rp(4-8)> <m> <sbuf(1)>\n\t- m: 0:quad,1:QCIF,2:d1,3:hd,4:9CH, 5:PB5");

	}

	else if ( !strcmp( (char *)argv[1], "rot" ) ) {
		
		if ( !strcmp( (char *)argv[2], "time" ) ) {
			
			sRotateTime = Asc2Bin(argv[3])*1000;
			if((sRotateTime < 1000) && (sRotateTime>10000))
					sRotateTime = 1000;
			printk("\r\n Rotation Time base change to %d msec", sRotateTime);
		}
		else {
			// spot rot 0 1  
			switch( Asc2Bin(argv[2])) {
			case 0: fgActiveSpotRotate_0 = Asc2Bin(argv[3]);	break;
			case 1: fgActiveSpotRotate_1 = Asc2Bin(argv[3]);	break;
		}
		printk("\r\n Active Spot #%d Rotate = %d", Asc2Bin(argv[2]), Asc2Bin(argv[3]));
	}
	}

	else {
		printk("\r\n spot init <p1> <p2>  : 0=OFF, 1=ON");
		printk("\r\n spot mode <sp(0-1)> <rp(4-8)> <m> <sbuf(1)>   - m: 0:quad,1:QCIF,2:d1,3:hd,4:9CH");
		printk("\r\n spot bufoff  : All Spot buffer write disable ");
		printk("\r\n spot rot <port> <0/1> : Spot <port> rotate On/Off(1/0)");
		printk("\r\n spot rot time <sec> : Spot rotate time(if 0 then will be 1)");

	}

	return 1;

}

/** @} */ 
