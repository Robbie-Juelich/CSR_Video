
/*! 
 *************************************************************************************
 * \file  tw2828_rec.c
 *
 * \brief
 *    Record
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

/*! @defgroup Record /tw2828API/Record 
*   @ingroup tw2828API
*/
/*!
\addtogroup Record
\{
*/

extern void Reset_Enc_S(void);
extern void SetQCIF_OSD(u8 rp, u8 en);
extern void Set9CH_OSD(u8 rp, u8 en);
void SetRec_New_FRSC_Control(u8 rp, u8 no, u8 en);
extern void MaskHDNoVideo(u8 rp, u8 p, u8 step, u8 posi, u8 maskflg);
extern void DisMaskHDNoVideo(u8 rp, u8 step, u8 posi);

extern _RecAllInfo rTb_Rec_4xD1_27M_8b_IFrame_rport5_8;

#define SetRecSPOTOSD_Status(rp,en) 	WriteP(REC_SPOT_STATUS_ON, (ReadP(REC_SPOT_STATUS_ON) & ~(0x01<<rp)) | (en<<rp) )


void SetFieldSwitchControl(u8 ch, u8 en)
{
  if(en) {
	if(ch<8)
		WriteP(RECFIELDSWITCH1, ReadP(RECFIELDSWITCH1) | (0x01<<ch));
	else 
		WriteP(RECFIELDSWITCH2, ReadP(RECFIELDSWITCH2) | (0x01<<(ch-8)));
  }
  else {
	if(ch<8)
		WriteP(RECFIELDSWITCH1, ReadP(RECFIELDSWITCH1) & ~(0x01<<ch));
	else 
		WriteP(RECFIELDSWITCH2, ReadP(RECFIELDSWITCH2) & ~(0x01<<(ch-8)));
  }
}

void SetFieldSwitchOffAll(void)
{
	u8 ch;
	for(ch=0; ch<16; ch++)
		SetFieldSwitchControl(ch, OFF);
}

void SetNewFieldSwitchOffAll(void)
{
	WriteP(RECNEW_FLI_SW_MD, 0);
	WriteP(RECNEW_FLI_RD, 0);
	WriteP(RECNEW_FLI_SW_EN, 0);
	WriteP(REC_FORMATTER, 0);
}

void SetNewFieldSwitchOnAll(void)
{
	WriteP(RECNEW_FLI_SW_MD, 0xff);
	WriteP(RECNEW_FLI_RD, 0xff);
	WriteP(RECNEW_FLI_SW_EN, 0xff);
	WriteP(REC_FORMATTER, 0xff);
}

void RecLimitStartWindow(u8 en)
{
	// enable is 0
	if(en)
		WriteP(REC_MISC_CTRL, ReadP(REC_MISC_CTRL) & 0xbf);
	else
		WriteP(REC_MISC_CTRL, ReadP(REC_MISC_CTRL) | 0x40);

}

void RecBufferDisableAll(void)
{
	u8 i;

	for(i=0; i<16; i++)
		WriteP(RECBUFCTRL1_B+i, ReadP(RECBUFCTRL1_B+i)&0x7f);

}

//---------------------------------------------------
void RecPortEnable(u8 p, u8 on)
{
	if(on) {
		if(p<4)	WriteP(RECPORTCTRL5+p*6, ReadP(RECPORTCTRL5+p*6)|0x01);
		else if(p==4)WriteP(RECNETPORTCTL, ReadP(RECNETPORTCTL)|0x01);
	}
	else{
		if(p<4)	WriteP(RECPORTCTRL5+p*6, ReadP(RECPORTCTRL5+p*6)&0xfe);
		else if(p==4)	WriteP(RECNETPORTCTL, ReadP(RECNETPORTCTL)&0xfe);
	}
}

void RecPortDisable(u8 p)
{
	if(p<4)	WriteP(RECPORTCTRL5+p*6, 0x00);
	else if(p==4)	WriteP(RECNETPORTCTL, 0x00);
}

void RecPortDisableAll(void)
{
	u8 p;

	for(p=0; p<5; p++)
		RecPortDisable(p);
}

void RecPort16bitEnable(u8 p, u8 on)
{
	u8 val;

  if(p==4) return;
  	
	val = ReadP(REC16BITMODECRTL);
	if(on) {
		WriteP(REC16BITMODECRTL, val | 0x02<<(p<<1));
	}
	else {
		WriteP(REC16BITMODECRTL, val & ~(0x02<<(p<<1)));
	}
}

void RecPort16bitDisable(void)
{
		WriteP(REC16BITMODECRTL, 0x00);
}

u8 SetRecBufferSelect(u8 p, u8 tbno, u8 bufno)
{
  if(p<4)	{ //Tw2828 only has Rec5 to Rec9.
	
		//dprintk("\r\n- Port#%d, (addr[%04x]:data[%04x])-(%02x:%02x)", p,(RECPORTTBSEL5_ADDR+p*6),(RECPORTTBSEL5_DATA+p*6),tbno,bufno );

		WriteP(RECPORTTBSEL5_ADDR+p*6, tbno);
		WriteP(RECPORTTBSEL5_DATA+p*6, bufno);

	}
	else if(p==4) {
		WriteP(RECNETPORTTBSEL_ADDR, tbno);
		WriteP(RECNETPORTTBSEL_DATA, bufno);
	}

	//dprintk("\r\n- Port#%d, (tb#%02x:buf#%02x)", p,tbno,bufno );

	return TRUE;

}


int TWMux_SetRecPortTable_Update(u8 id, u8 p, u8 *buf, u8 inx_st, u8 inx_ed)
{
	u8 k,i, pb, maskflg;


	for(k=inx_st,i=0; k<=inx_ed; k++,i++)	{
		if(p<4)	{
			WriteP(RECPORTTBSEL5_ADDR+(p*6), k);
			WriteP(RECPORTTBSEL5_DATA+(p*6), buf[i]);
		}
		else if(p==4) {
			WriteP(RECNETPORTTBSEL_ADDR, k);
			WriteP(RECNETPORTTBSEL_DATA, buf[i]);
		}
		//printk("\n- Port#%d, (tb#%02x:buf#%02x)", p,k,buf[i]);

	  if((buf[i] >= R_PB1) && (buf[i] <= R_PB4)) {
	  	pb = buf[i] - R_PB1;
	  	if(tw2828devs[id].ipbtype[pb] == NOVIDEO) maskflg = 0;
	  	else maskflg = 1;	
			MaskHDNoVideo(p, buf[i]-R_PB1,  inx_ed-inx_st+1, i, maskflg);
		}
		else
			DisMaskHDNoVideo(p, inx_ed-inx_st+1, i);

		SetRec_New_FRSC_Control(p, buf[i], 1);
	}

  // Enable Update
	if(p<4)	{
			WriteP(RECPORTTBSEL5_DATA+(p*6), buf[i]|0x20); //
	}
	else if(p==4) {
			WriteP(RECNETPORTTBSEL_DATA, buf[i]|0x20);
	}

	return TRUE;

}

void SetRecPortHDDE(void)
{
	//if(GET_VDO)
	{							//... NTSC
		WriteP(RECPORT_HDE1, 0x78);	// 1920/16
		WriteP(RECPORT_VDE1, 0x87);	// 1080/8

		WriteP(RECPORT_HDE5, 0x78);
		WriteP(RECPORT_VDE5, 0x87);
	}
}

void SetRecPortQueMax(u8 p, u8 qmax)
{
	// Set Que max
	if(p<4){
		WriteP(RECPORT_TB_MAX5+(p*6), qmax-1);
	}
	else if(p==4)
		WriteP(RECNETPORT_TB_MAX, qmax-1);

}

void SetRecPortContol(u8 i, u8 en, u8 format, u8 clk, u8 res, u8 data_bit)
{
	u8 val=0, k;

  //printk("\n %d - (en,format,clk,res)-(%d,%d,%d,%d)", i,en,format,clk,res);
  
	// format
	if( i<4) {
		val = (format<<5)&0x20;
		k = ReadP(REC_IFRAME) & ~(0x01<<i); // Interlaced Frame Interleave
		if((format==IFRAME) || (format==FIELDSW)) k = k|(0x10<<i);
		WriteP(REC_IFRAME, k);
	}
	else if(i==4)
	{
		val = (format<<5)&0x20;
		k = ReadP(0xc4d) & 0xf7; 
		if((format==IFRAME) || (format==FIELDSW)) k = k|0x08;
		WriteP(0xc4d, k);
	}	

	//res
	switch(res){
	case D1    : val = val|0x00; break;
	case HALFD1: val = val|0x10; break;
	case QUARD : val = val|0x18; break; 
	case D1_4  : val = val|0x98; break; // Enable H/V split
	case CIF   : val = val|0x40; break;
	case BT1120: val = val|0xc0; break; // 6D1
	case CH_9  : val = val|0x18; break; 
	case QCIF  : val = val|0x18; break; 
	}

	//en
	val = val|en;

	//clk
	val = val|(clk<<1);

	if(i<4){
		WriteP(RECPORTCTRL5+(i*6), val);
	}
	else if(i==4) // Network port
		WriteP(RECNETPORTCTL, val);

	if(res == BT1120) SetRecPortHDDE();

  if(res == CH_9) {
		SetQCIF_OSD(i, ON);
		Set9CH_OSD(i, ON);
  } 
  else if(res == QCIF) {
		SetQCIF_OSD(i, ON);
		Set9CH_OSD(i, OFF);
  }
  else	{
		SetQCIF_OSD(i, OFF);
		Set9CH_OSD(i, OFF);
	}

	// Add for field mode 
	if(format==FIELD) { 
		//WriteP(0xc4d, ReadP(0xc4d)|0x80); 
		//WriteP(0xc4d, ReadP(0xc4d)|0x40); 
	}
	else{
		//WriteP(0xc4d, ReadP(0xc4d)&0x7f);
		//WriteP(0xc4d, ReadP(0xc4d)&0xbf);
	}

	// New Field Switch
	k = ReadP(REC_FORMATTER) & ~(0x01<<i); // // Formatter
	if(format==FIELDSW) k = k|(0x01<<i);
	WriteP(REC_FORMATTER, k);
		
	k = ReadP(RECNEW_FLI_RD) & ~(0x01<<i); 
	if(format==FIELDSW) k = k|(0x01<<i);
	WriteP(RECNEW_FLI_RD, k);

	k = ReadP(RECNEW_FLI_SW_MD) & ~(0x01<<i); 
	if(format==FIELDSW) k = k|(0x01<<i);
	WriteP(RECNEW_FLI_SW_MD, k);
		
	k = ReadP(RECNEW_FLI_SW_EN) & ~(0x01<<i); 
	if(format==FIELDSW) k = k|(0x01<<i);
	WriteP(RECNEW_FLI_SW_EN, k);

  //data bit
  if(data_bit==PORT_16BIT)
  	RecPort16bitEnable(i, 1);
  else 
  	RecPort16bitEnable(i, 0);

}

void SetRecPort(u8 i, _RecPortInfo rport)
{
	u32 k;
	if(rport.en == OFF) {
		RecPortEnable(i, OFF);
		return;
	}

	
	#ifdef DEBUG_REC
	dprintk("\r\n- Port#%d-(On,format,Res,Clk,QMax)-(%d,%02x,%02x,", i,rport.en,rport.format,rport.res,rport.clk );
	dprintk("%d)", rport.qmax );
	#endif

	RecPortEnable(i, OFF);// Must off while changing que.

	// Set Que
	if(rport.que)	{
		for(k=0; k<rport.qmax; k++)	{
			SetRec_New_FRSC_Control(i, rport.que[k], 1);
			if(SetRecBufferSelect(i, k, rport.que[k]) == FALSE) break;
		}
	}

	// Set Que max
	SetRecPortQueMax(i, rport.qmax);

	// Set control bit
	SetRecPortContol(i,rport.en, rport.format, rport.clk, rport.res, rport.databit);

	SetRecSPOTOSD_Status(i, OFF);
	if(rport.en == ON)
		RecPortEnable(i, ON);
}

void SetRecPin(u8 i, _RecPinInfo rpin) 
{
	u8 val=0;

	#ifdef DEBUG_REC	
	dprintk("\r\n- Pin#%d-(Clk,ClkIvt,8b/16b,DataPort )-(%01x,%01x,%01x,%01x)", i,rpin.clk,rpin.clkinvert,rpin.databit,rpin.dataport );
	#endif
		
	if(rpin.clkinvert == CLK_INVERT) val = 0x08;	
	if(rpin.databit != PIN_8BIT)    val = val|0x04;
	val = val|rpin.clk;
	val = val|(rpin.dataport<<4);
	printk("write 0x%x to %x rpin.clk %x\n", val, RECPINCTL1+i, rpin.clk);
	WriteP(RECPINCTL1+i, val);  // notice 0xc4f

	switch(i){
	case 0:	WriteP(RECPINPORTSEL1, (ReadP(RECPINPORTSEL1)&0xf0)|rpin.dataport);		break;
	case 1:	WriteP(RECPINPORTSEL1, (ReadP(RECPINPORTSEL1)&0x0f)|(rpin.dataport<<4));		break;
	case 2:	WriteP(RECPINPORTSEL2, (ReadP(RECPINPORTSEL2)&0xf0)|rpin.dataport);		break;
	case 3:	WriteP(RECPINPORTSEL2, (ReadP(RECPINPORTSEL2)&0x0f)|(rpin.dataport<<4));		break;
	}

	WriteP(REC_CLKMODE,0);	//108Mhz

}

void Clear_New_FRSC_All(void)
{
	int i;
	
	for(i=R_1; i<R_8; i++) {
		SetRec_New_FRSC_Control(PORT5, i, 0);
		SetRec_New_FRSC_Control(PORT6, i, 0);
		SetRec_New_FRSC_Control(PORT7, i, 0);
		SetRec_New_FRSC_Control(PORT8, i, 0);
		SetRec_New_FRSC_Control(PORT9, i, 0);
	}
	for(i=S_1; i<S_16; i++) {
		SetRec_New_FRSC_Control(PORT5, i, 0);
		SetRec_New_FRSC_Control(PORT6, i, 0);
		SetRec_New_FRSC_Control(PORT7, i, 0);
		SetRec_New_FRSC_Control(PORT8, i, 0);
		SetRec_New_FRSC_Control(PORT9, i, 0);
	}
	for(i=R_PB1; i<R_PB5; i++) {
		SetRec_New_FRSC_Control(PORT5, i, 0);
		SetRec_New_FRSC_Control(PORT6, i, 0);
		SetRec_New_FRSC_Control(PORT7, i, 0);
		SetRec_New_FRSC_Control(PORT8, i, 0);
		SetRec_New_FRSC_Control(PORT9, i, 0);
	}
}

/*! 
  \brief
		Set New Record Frame Rate Control  
	\retval 
	        <NONE>   
	\param [in] rp    : Record Port
	\param [in] no    : record/spot buffer no
	\param [in] en    : enable

	\par Update history:
		JULY/08/2012 - Created  \n
	\note 
		Wrong setting will cause image tearing or not correctly working! 
*/
void SetRec_New_FRSC_Control(u8 rp, u8 no, u8 en)
{
	  u8 i=0;
	
		//printk("\n Rec FRSC bf:%d -> rp:%d - %s", no, rp, en==1?"ON":"OFF");
		
		if(no<=R_8) { 
			i=no; 
			WriteP(REC_WB_FRSC, i);
			WriteP(REC_RP_FRSC, rp | 0x10);

			WriteP(BUFMAP_CTRL, (rp*3)|0x20);
			WriteP(BUFMAP_EN, ReadP(BUFMAP_EN) | (en<<no));
		}
		else if((no>=S_1) && (no<=S_16)) { 
			i=no-8; 
			WriteP(REC_WB_FRSC, i);
			WriteP(REC_RP_FRSC, rp | 0x10);

	  	if(no<S_9) {
				WriteP(BUFMAP_CTRL, (rp*3+1) | 0x20);
				WriteP(BUFMAP_EN, ReadP(BUFMAP_EN) | (en<<(no-S_1)));
			}
			else {
				WriteP(BUFMAP_CTRL, (rp*3+2) | 0x20);
				WriteP(BUFMAP_EN, ReadP(BUFMAP_EN) | (en<<(no-S_9)));
			}
		}
		else if((no>=R_PB1) && (no<=R_PB5)) { 
			i = 23 + no - 8; 
			WriteP(REC_WB_FRSC, i);
			WriteP(REC_RP_FRSC, rp | 0x10);

			WriteP(BUFMAP_CTRL, (15+rp)|0x20);
			WriteP(BUFMAP_EN, ReadP(BUFMAP_EN) | (en<<(no-8)));
			
			WriteP(RECPB_FRSC, ReadP(RECPB_FRSC)| (en<<(no-8))); //PB1-5
			
		}
		else { i=0; printk("\n Rec Buf error bf:%d -> rp:%d", no, rp); }
   	
		//printk("-- Rec FRSC buf_sel=%x bf=%d -> rp=%d", buf_sel, i, rp);
		//printk("-- Rec FRSC buf_sel=%x bf=%d -> rp=%x", buf_sel, i, ((0x01<<rp) | 0x10) );
}

/*! 
  \brief
		Load Record Buffer data for SPOT  
	\retval 
	        <NONE>   
	\par Update history:
		JULY/08/2012 - Created  \n
	\note 
		TW2828 has 8 Record Buffers.  
*/
void TWMux_LoadRecBuf(u8 i, u8 no, _RecSpotBuf r)
{
	//printk("\n- [%d]: rBuf#%d, (Ch,En,format,Res,Hs,Vs)-(%d,%d,%02x,%02x,", i, no,r.chno,r.en,r.format,r.res);
	//printk("%02x,%02x)", r.hposi, r.vposi );

	if((r.chno>=16) && (r.chno<=20)) 
		WriteP(RECBUFCTRL1_A+no, (r.format<<6)|(r.res<<4)|(r.chno&0x0f)|0x80);
	else
		WriteP(RECBUFCTRL1_A+no, (r.format<<6)|(r.res<<4)|(r.chno&0x0f));
		
	WriteP(RECBUFCTRL1_B+no, (r.en<<7)|(r.hposi<<3)|r.vposi);
}

/*! 
 * \brief
 *    Record Channel Encode
 *
 * \param [in] id  : chip no
 * \param [in] tb  : All of Report buffer/Port/Pin information
 *
 * \return
 *        <NONE> 
 * \par Description \n
 *      (1) Set Default Channel Encoder    \n
 *      (2) Disable all Rec Buffers and enable rec buf being used \n
 *      (3) Disable all Rec ports and enable rec port being used \n 
 *      (4) Set Reord Pin
 *      (5) Software Reset for Rec enc/dec
 * \note TW2828 Record can support SD, not WD1.
 * 
 */
void TWMux_LoadRecordDataAll(u8 id, _RecAllInfo tb)
{
	u8 i,p;
	_RecSpotBuf *rbuf;
	_RecPortInfo *rport;
	_RecPinInfo *rpin;

  const _ChIdEnc Default_Chid_Enc = {
	(ENC_EVEN_EN | \
	 ENC_RUN_IN_CLK_EN | \
	 ENC_DIGITAL_EN | \
	 ENC_USR_EN | \
	 ENC_ANALOG_EN | \
	 ENC_RPT_EN | \
	 ENC_AUTO_EN | \
	 ENC_ODD_EN), //ctrl
	0, //vos
	0, //fos
	0, // hos;
	0x05, //min_val
	0x3f, //max_val
  };

	//dprintk("\r\nLoad Rec Data Table");
	RecPort16bitDisable();
	RecBufferDisableAll();
	SetNewFieldSwitchOffAll();
//	Clear_New_FRSC_All();
		
	RecLimitStartWindow(1); 
	RecLimitStartWindow(0);

	for(p=0; p<5; p++)
		TWMux_SetRecordChannelIDEnc(id, p, Default_Chid_Enc);

	if( tb.buf ) {
		rbuf = (_RecSpotBuf *)tb.buf;
		for(i=0; rbuf[i].bf!=NOBUF; i++) {
//	    printk("\n- buf#%d-(%01x) %02x", i,rbuf[i].en,  ((rbuf[i].en<<7)|(rbuf[i].hposi<<3)|rbuf[i].vposi));
	    if(rbuf[i].bf<S_1 )
	    	TWMux_LoadRecBuf(id, rbuf[i].bf, rbuf[i]);
		else	
	    	TWMux_LoadSpotBuf(id, rbuf[i].bf-S_1, rbuf[i]);
		}
	}

	if( tb.port ) {
		rport = (_RecPortInfo *)tb.port;
		for(i=0; i<5; i++) 
			SetRecPort(i, rport[i]);
	}

	if( tb.pin ) {
		rpin = (_RecPinInfo *)tb.pin;
		
		for(i=0; i<4; i++ ) {
			SetRecPin(i, rpin[i]);
		}
	}

    Reset_Enc_S(); 
}

//-----------------------------------------------------------------------------
/*! 
 * \brief
 *    Record Channel Encode
 *
 * \param [in] id  : chip no
 * \param [in] p   : Report Port number (0-4)
 * \param [in] chenc : data
 *
 * \return
 *        <NONE> 
 * \note
 *         
 */

void TWMux_SetRecordChannelIDEnc(u8 id, u8 p, _ChIdEnc chenc)
{
	p+=4; 
	if((p<4) && (p>8)) return;  // port number

	#ifdef DEBUG_REC
	dprintk("\r\nSetEncChid: %d", p);
	#endif
	WriteP(REC1_CHID_CTRL+p*0x12, chenc.ctrl);
	WriteP(REC1_VOS+p*0x12, chenc.vos);
	WriteP(REC1_FOS+p, chenc.fos);
	WriteP(REC1_HOS+p*0x12, chenc.hos);
	WriteP(REC1_MIN_LEVEL+p*0x12, chenc.min_val);
	WriteP(REC1_MAX_LEVEL+p*0x12, chenc.max_val);

}

void TWMux_SetRec34_Camera(u32 rec, u32 cam_no)
{
	// the default mapping(cam and ch number is 0 based)
	// cam0 - (0) CH1  - buf1 - port5 - rec3
	// cam1 - (1) CH2  - buf2 - port6 - rec4
	// cam2 - (4) CH5  - buf3 - port7
	// cam3 - (5) CH6  - buf4 - port8
	// cam4 - (8) CH9  - buf5
	// cam5 - (9) CH10 - buf6
	// cam6 - (12)CH13 - buf7
	// cam7 - (13)CH14 - buf8
#if 1
	if (rec == 3 || rec == 4) {
		u32 regbuf = RECBUFCTRL1_A + rec - 3;
		if (cam_no < 8) {
#if defined(VIDEO_2D1)
			u32 chno = (cam_no / 2) * 4 + (cam_no % 2);
#else
			u32 chno = cam_no;// (cam_no / 2) * 4 + (cam_no % 2);
#endif
			u32 val = ReadP(regbuf) & 0xf0;	
			WriteP(regbuf, val | (chno & 0xf));
		}
	}
#else
	int port_no = 0x0; // port 5
	if (rec == 3 || rec == 4) {
        if (cam_no >= 8) {
            return;
        }

#if 1
	   	u8 buf[] = {cam_no};
//      TWMux_SetRecPortTable_Update(HostBusNo, port_no, buf, 0, 0);
        _RecPortInfo rport = 
			((_RecPortInfo*)rTb_Rec_4xD1_27M_8b_IFrame_rport5_8.port)[0];
        rport.que= buf;// the que only contains one channel;    
        SetRecPort(port_no, rport);
#else
		u32 RECPORTTBSEL_ADDR = RECPORTTBSEL5_ADDR + port_no * 6;
        u32 RECPORTTBSEL_DATA = RECPORTTBSEL5_DATA + port_no * 6;
            //u32 chno = (cam_no / 2) * 4 + (cam_no % 2);
        RecPortEnable(port_no, OFF);
        SetRec_New_FRSC_Control(port_no, cam_no, 1);
        WriteP(RECPORTTBSEL_ADDR, 0x0);
        WriteP(RECPORTTBSEL_DATA, cam_no);
        SetRecPortQueMax(port_no, 1);// que max is 1
        RecPortEnable(port_no, ON);
#endif


	}
#endif

	Reset_Enc_S();
}
/** @} */

//-------------------------------------------------------------------------
