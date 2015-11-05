
/*! 
 *************************************************************************************
 * \file  <tw2828_spot.c>
 *
 * \brief
 *    spot 
 *    (Remember, Spot has only sbuf and encoder output)
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    - Heejeong Ryu  <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
 
#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"

/*! @defgroup SPOT /tw2828API/SPOT
*   @ingroup tw2828API
*/
/*!
\addtogroup SPOT
\{
*/

extern void RecPortEnable(u8 num, u8 on);
extern u8 SetRecBufferSelect(u8 p, u8 tbno, u8 bufno);
extern void SetRecPortQueMax(u8 p, u8 qmax);
extern void Reset_Enc_S(void);
extern void SetRecPortContol(u8 i, u8 en, u8 format, u8 clk, u8 res, u8 data_bit);
extern void SetRec_New_FRSC_Control(u8 rp, u8 no, u8 en);

//---------------------------------------------------------------------
// Rec Spot
//---------------------------------------------------------------------

void SetQCIF_OSD(u8 rp, u8 en)
{
  		
  if(en) {
	if(rp<4)
		WriteP(REC_QCIF_OSD_L, ReadP(REC_QCIF_OSD_L) | (0x01<<rp));
	else 
		WriteP(REC_QCIF_OSD_H, ReadP(REC_QCIF_OSD_H) | 0x10);
  }
  else {
	if(rp<4)
		WriteP(REC_QCIF_OSD_L, ReadP(REC_QCIF_OSD_L) & ~(0x01<<rp));
	else 
		WriteP(REC_QCIF_OSD_H, ReadP(REC_QCIF_OSD_H) & 0xef);
  }
}

void Set9CH_OSD(u8 rp, u8 en)
{
  if(en) {
	if(rp<4)
		WriteP(REC_QCIF_OSD_L, ReadP(REC_QCIF_OSD_L) | (0x10<<rp));
	else 
		WriteP(REC_QCIF_OSD_H, ReadP(REC_QCIF_OSD_H) | 0x020);
  }
  else {
	if(rp<4)
		WriteP(REC_QCIF_OSD_L, ReadP(REC_QCIF_OSD_L) & ~(0x10<<rp));
	else 
		WriteP(REC_QCIF_OSD_H, ReadP(REC_QCIF_OSD_H) & ~0x20);
  }
}

void SetQCIFControl(u8 ch, u8 en)
{
  if(en) {
	if(ch<8)
		WriteP(SPT_QCIF_ON_L, ReadP(SPT_QCIF_ON_L) | (0x01<<ch));
	else 
		WriteP(SPT_QCIF_ON_H, ReadP(SPT_QCIF_ON_H) | (0x01<<(ch-8)));
  }
  else {
	if(ch<8)
		WriteP(SPT_QCIF_ON_L, ReadP(SPT_QCIF_ON_L) & ~(0x01<<ch));
	else 
		WriteP(SPT_QCIF_ON_H, ReadP(SPT_QCIF_ON_H) & ~(0x01<<(ch-8)));
  }
}

void Set9CHControl(u8 ch, u8 en)
{
  if(en) {
	if(ch<8)
		WriteP(SPT_9CH_ON_L, ReadP(SPT_9CH_ON_L) | (0x01<<ch));
	else 
		WriteP(SPT_9CH_ON_H, ReadP(SPT_9CH_ON_H) | (0x01<<(ch-8)));
  }
  else {
	if(ch<8)
		WriteP(SPT_9CH_ON_L, ReadP(SPT_9CH_ON_L) & ~(0x01<<ch));
	else 
		WriteP(SPT_9CH_ON_H, ReadP(SPT_9CH_ON_H) & ~(0x01<<(ch-8)));
  }
}

void SetRecBoundryControl(u8 rp, u8 en)
{
	rp+=4; 
  if(en) {
	if(rp<8)
		WriteP(REC_OSD_BNDRY_EN_0, ReadP(REC_OSD_BNDRY_EN_0) | (0x01<<rp));
	else 
		WriteP(REC_OSD_BNDRY_EN_1, ReadP(REC_OSD_BNDRY_EN_1) | (0x10<<(rp-8)));
  }
  else {
	if(rp<8)
		WriteP(REC_OSD_BNDRY_EN_0, ReadP(REC_OSD_BNDRY_EN_0) & ~(0x01<<rp));
	else 
		WriteP(REC_OSD_BNDRY_EN_1, ReadP(REC_OSD_BNDRY_EN_1) & ~(0x10<<(rp-8)));
  }
}


void SetRec2Spot(u8 rp, u8 spot)
{
	//REC_OSD for SPOT status
	WriteP(REC_SPOT_STATUS_ON, ReadP(REC_SPOT_STATUS_ON) | (0x01<<rp) );

	rp+=4;
	if(spot==SPOT1) {
		WriteP(SPT1_DNS_H1, (u8)(ReadP(SPT1_DNS_H1)|0x80));
		WriteP(SPT12_SRC, (u8)((ReadP(SPT12_SRC)&0xf0)|rp));
	}
	else {
		WriteP(SPT2_DNS_H1+(spot-1)*0x30, (u8)(ReadP(SPT2_DNS_H1)|0x80));
		WriteP(SPT12_SRC, (u8)((ReadP(SPT12_SRC)&0x0f)|(rp<<4)));
	}
	
}

void SpotBufferEnable(u8 p, u8 on)
{
	if(on)
		WriteP(SPT1_BUFCTRL_B+p, ReadP(SPT1_BUFCTRL_B+p)|0x80);
	else
		WriteP(SPT1_BUFCTRL_B+p, ReadP(SPT1_BUFCTRL_B+p)&0x7f);
}

void DisableAllSpotBuf(void)
{
	u8 i;
	for(i=0; i<16; i++) {
	    SpotBufferEnable(i, 0);
	}
}

void SetSpotBufChannel(u8 no, u8 ch)
{
	u8 val;

	//dprintf("\r\n-@(SetSpotBufChannel)- sBuf#%d - Ch:%d", no,ch);

	val = ReadP(SPT1_BUFCTRL_A+no) & 0xf0;
	WriteP(SPT1_BUFCTRL_A+no, val|ch);
}

void SpotDacOnOff(u8 on)
{
	if(on)
		WriteP(0xf9b, 0x1);
	else
		WriteP(0xf9b, 0x0);
}

void SetColorMask(u8 y, u8 cb, u8 cr)
{
	WriteP(0xEC0, y);
	WriteP(0xEC1, cb);
	WriteP(0xEC2, cr);
}


/*! 
  \brief
		Load Spot Buffer data
	\param [in] host
	\param [in] no    : spot buffer no
	\param [in] s     : Setting infomation
	\retval 
	        <NONE>   
	\par Update history:
		JULY/08/2012 - Created  \n
	\note
		TW2828 has 16 SPOT Buffers.  
*/
void TWMux_LoadSpotBuf(u8 host, u8 no, _RecSpotBuf s)
{
	//printf("\r\n- [%d] sBuf#%d, (Ch,En,format,Res,Hs,Vs)-(%d,%d,%02x,%02x,", host, no, s.chno, s.en, s.format, s.res);
	//printf("%02x,%02x)", s.hposi, s.vposi );

	WriteP(SPT1_BUFCTRL_A+no, (s.format<<6)|(s.res<<4)|s.chno);
	WriteP(SPT1_BUFCTRL_B+no, (s.en<<7)|(s.hposi<<3)|s.vposi);
	
	SetQCIFControl(no, s.qcif_en);
	Set9CHControl(no, s.ch9_en);

}
void DisMaskHDNoVideo(u8 rp, u8 step, u8 posi)
{
	WriteP(0xcbd, 0x0);
	WriteP(0xcbc, 0x1f);
	switch(step) {
	case 1 : 
		WriteP(0xe79+0x10*rp, 0);	break;
	case 4 : 
	  switch(posi) {
	  case 0:  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xfe); 	break;
	  case 1:  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xfd); 	break;
	  case 2:  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xef); 	break;
	  case 3:  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xdf); 	break;
	  }
	break;
				
	}
	
	WriteP(0xcbd, 0x1f);

}

/*! 
  \brief
		Spot No Video Check for HD input
	\param [in] host
	\param [in] p : pb rport (0-3)
	\param [in] step : D1, Quad
	\retval <NONE>   
	\par Description:
		SPOT doesn't have no video mask feature. We should use workaround solution. \n
		We should get no vifdeo information from Motion detection. \n
		We have to make screen with SPOT MASK function in case of no video comming.          
	\note
		PB5 doesn't have Motion. We cannot cover it for PB5!!  
*/

 
void MaskHDNoVideo(u8 rp, u8 p, u8 step, u8 posi, u8 check)
{
	WriteP(0xcbd, 0x0);
	WriteP(0xcbc, 0x1f);

  #ifdef SUPPORT_TW2828_HDNOVIDEO_CHECK
	check = ReadP(0x2be) & (0x01<<p);
	#endif
  // printf("---- MaskHDNoVideo: %d, %d, 0x%02x", p, step, check);
  
	SetColorMask(0x10, 0xe0, 0x80); //blue

	switch(step) {
	case 1 : 
	  	WriteP(0xe78+0x10*rp, 0x03);
	  	WriteP(0xe70+0x10*rp, 0xf0); 	
	  	WriteP(0xe74+0x10*rp, 0xf0); 	
	  	WriteP(0xe76+0x10*rp, 0xf0); 	
			if(check)  WriteP(0xe79+0x10*rp, 0x53); 	
	    else       WriteP(0xe79+0x10*rp, 0); 
	break;
	case 4 : 
	  WriteP(0xe78+0x10*rp, 0x11);
	  WriteP(0xe70+0x10*rp, 0xf0);
	  WriteP(0xe71+0x10*rp, 0xf0);
	  WriteP(0xe74+0x10*rp, 0xf0);
	  WriteP(0xe75+0x10*rp, 0xf0);
	  switch(posi) {
	  case 0:  if(check)  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) | 0x01); 	
	  	       else       WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xfe); 	break;
	  case 1:  if(check)  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) | 0x02);  
	  	       else       WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xfd); 	break;
	  case 2:  if(check)  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) | 0x10);  
	  	       else       WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xef); 	break;
	  case 3:  if(check)  WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) | 0x20);  
	  	       else       WriteP(0xe79+0x10*rp, ReadP(0xe79+0x10*rp) & 0xdf); 	break;
	  }
	break;
				
	}
	
	WriteP(0xcbd, 0x1f);
	//WriteP(0xcbd, 0x0);
	
}


/*! 
  \brief
		Load Record Port for Spot
	\param [in] host
	\param [in] rport : rport info
	\retval <NONE>   
	\note
		TW2828 has 16 SPOT Buffers.  
*/
void TWMux_SetRecPortForSpot(u8 id, _twSpot_port rport)
{
	u8 i, pb, maskflg=0;

	//printk("\n- %d - (%d,%d,%d,%d,%d) - %d", rport.rp, rport.en, rport.format, rport.clk, rport.res, rport.databit, rport.sp);
	 
	RecPortEnable(rport.rp, OFF);// Must off while changing rport.
	
	WriteP(0xe79, 0);

  if(rport.res == QCIF) {
	 	for(i=0; i<16; i++) 
			SetRec_New_FRSC_Control(rport.rp, S_1+i, 1);
  }
  else if(rport.res == CH_9) {
	 	for(i=0; i<9; i++) 
			SetRec_New_FRSC_Control(rport.rp, S_1+i, 1);
  }
  else {
		for(i=0; i<rport.qmax; i++) {
			SetRec_New_FRSC_Control(rport.rp, rport.que[i], 1);
  	}
  }
  
	// Set Buf
	if(rport.que)	{
		for(i=0; i<rport.qmax; i++)	{
			SetRecBufferSelect(rport.rp, i, rport.que[i]);
			if((rport.que[i] >= R_PB1) && (rport.que[i] <= R_PB4)) {
	  		pb = rport.que[i] - R_PB1;
	  		if(tw2828devs[id].ipbtype[pb] == NOVIDEO) maskflg = 0;
	  		else maskflg = 1;	
				MaskHDNoVideo(rport.rp, rport.que[i]-R_PB1,  rport.qmax, i, maskflg);
			}
			else
				DisMaskHDNoVideo(rport.rp, rport.qmax, i);
					
		}
	}

	SetRecPortQueMax(rport.rp, rport.qmax);
	SetRecPortContol(rport.rp,rport.en, rport.format, rport.clk, rport.res, rport.databit);
	
	SetRecBoundryControl(rport.rp, ON);
	
	RecPortEnable(rport.rp, ON);
	SetRec2Spot(rport.rp, rport.sp);
	
	Reset_Enc_S();

	SpotDacOnOff(1);

}


/** @} */

