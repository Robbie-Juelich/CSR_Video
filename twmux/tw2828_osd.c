
/*! 
 *************************************************************************************
 * \file  <tw2828_osd.c>
 *
 * \brief
 *    display osd, rec/spot osd
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    Heejeong Ryu <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 * \Note 
 *    Modified to support Asian font(22x22)
 * \Revision history
 *
 */

#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"

#define FONTu8	4424// 24(22pixel) * 22 line * 2 bit * 64 font / 16 (2u8)
#define PICu8		512 // 32 pixel * 32 line * 2 bit * 4 picture / 16 (2u8)
#define _END_ 0xff
#define RECCHARSIZE		16
#define _SPACE_ 0x00
#define MAXCOLOR	21


/*! @defgroup OSD /tw2828API/OSD 
*   @ingroup tw2828API
*/
/*!
\addtogroup OSD
\{
*/

/*! 
 * \brief
 *    Load Display Font
 *
 * \param [in] id
 *             chip number     
 * \param [in] ptr
 *             OSD Data Image pointer table     
 * 
 * \return
 *        <NONE> 
 * \note
 *        (1) Totoal 64 char. \n
 *        (2) Each font size is 22x22 pixel fixed. When hardware can read changable read size depand on register setting. \n
 *        (3) One pixel is 2bit 4 Color, rgb based.        \n
 */
void TWMux_DisplayOsd_LoadOSDFontData(u8 no, _OSD_HEADER ptr)
{
    int addr=0,k,y,i=0;	

	if(ptr.osdfont) {
		   
	  for(k=0; k<64; k++){
	   for(y=0; y<22; y++ ){
	   	addr = k*22 + y; 

			WriteP(OSD_RAM_ADDR_L, (u8)(addr));
			WriteP(OSD_RAM_ADDR_H, (u8)(addr>>8));
			WriteP(OSD_RAM_DATA_0, ptr.osdfont[i+5]);
			WriteP(OSD_RAM_DATA_1, ptr.osdfont[i+4]);
			WriteP(OSD_RAM_DATA_2, ptr.osdfont[i+3]);
			WriteP(OSD_RAM_DATA_3, ptr.osdfont[i+2]);
			WriteP(OSD_RAM_DATA_4, ptr.osdfont[i+1]);
			WriteP(OSD_RAM_DATA_5, ptr.osdfont[i]);
			i+=6;
			udelay(10);  // by zzc
	  }
	 }
	}
	addr++;
#if 0	 
	if(ptr.osdpic) // Load OSD Picture data table
	{
		for(i=0; i<PICu8; i++) {
			WriteP(OSD_RAM_ADDR_L, (u8)(addr));
			WriteP(OSD_RAM_ADDR_H, (u8)((addr)>>8));
			WriteP(OSD_RAM_DATA_3, (u8)(ptr.osdpic[i]<<4));
			WriteP(OSD_RAM_DATA_4, (u8)(ptr.osdpic[i]>>4));
			WriteP(OSD_RAM_DATA_5, (u8)(ptr.osdpic[i]>>12));
			addr++;
		}
	}
#endif

	if(ptr.colortb) // Load OSD lookup table
	{
		for(i=0; i<MAXCOLOR; i++)
			WriteP(OSD_FONT_COLOR_R1+i, (u8)ptr.colortb[i+3]);
	}
}

void TWMux_DisplayOsd_LoadOSDDispData(u8 no,  _DrawOsd *ptrOsd)
{
	int i;
	_DrawOsd *ptr;
	
	ptr = (_DrawOsd *)ptrOsd;
	
	while(ptr->Str)
	{
	  i = 0;	
		while(ptr->Str[i]!=_END_){
			WriteP(OSD_DISP_RAM_ADDR_L, (u8)ptr->addr+i);
			WriteP(OSD_DISP_RAM_ADDR_H, (u8)((ptr->addr+i)>>8));
			WriteP(OSD_DISP_RAM_DATA, (u8)ptr->Str[i]);
			i++;
			udelay(10);  // by zzc
		}
	
		ptr++;
	}
	
	WriteP(OSD_EN_CTRL, ReadP(OSD_EN_CTRL)|0x01);
}


void TWMux_DisplayOsd_OnOff(u8 no, u8 on)
{
	if(on)
		WriteP(OSD_EN_CTRL, ReadP(OSD_EN_CTRL)|0x01);
	else
		WriteP(OSD_EN_CTRL, ReadP(OSD_EN_CTRL)&0xfe);

}

//---------------------------------------------------------------
void TWMux_RecOsd_LoadOSDFontData(u8 no, _OSD_HEADER ptr) 
{
    int addr,k,y,i=0;	

	if(ptr.osdfont) {

	  for(k=0; k<64; k++){
	   for(y=0; y<22; y++ ){
	   	addr = k*22 + y; 

			WriteP(REC_OSD_RAM_ADDR_L, (u8)(addr));
			WriteP(REC_OSD_RAM_ADDR_H, (u8)(addr>>8));
			WriteP(REC_OSD_RAM_DATA_0, ptr.osdfont[i+5]);
			WriteP(REC_OSD_RAM_DATA_1, ptr.osdfont[i+4]);
			WriteP(REC_OSD_RAM_DATA_2, ptr.osdfont[i+3]);
			WriteP(REC_OSD_RAM_DATA_3, ptr.osdfont[i+2]);
			WriteP(REC_OSD_RAM_DATA_4, ptr.osdfont[i+1]);
			WriteP(REC_OSD_RAM_DATA_5, ptr.osdfont[i]);
			i+=6;
			udelay(10);
	  }
	 }
  }	 

	if(ptr.colortb) // Load OSD lookup table
	{
		for(i=0; i<3; i++) {
			WriteP(REC_OSD_FONT_COLOR_CB+i*3, (u8)ptr.colortb[i*3+3]);
			WriteP(REC_OSD_FONT_COLOR_Y+i*3, (u8)ptr.colortb[i*3+4]);
			WriteP(REC_OSD_FONT_COLOR_CR+i*3, (u8)ptr.colortb[i*3+5]);
		}
	}

}

void TWMux_RecOsd_LoadOSDDispData( u8 no, _DrawOsd *ptrOsd)
{
	int i=0;
	_DrawOsd *ptr;
	
	ptr = (_DrawOsd *)ptrOsd;
	
	while(ptr->Str)
	{
		i = 0;
		while(ptr->Str[i]!=_END_){
			WriteP(REC_OSD_DISP_RAM_ADDR_L, (u8)ptr->addr+i);
			WriteP(REC_OSD_DISP_RAM_ADDR_H, (u8)((ptr->addr+i)>>8));
			WriteP(REC_OSD_DISP_RAM_DATA, (u8)ptr->Str[i]);
			i++;
		}
		while(i<RECCHARSIZE){
		  WriteP(REC_OSD_DISP_RAM_ADDR_L, (u8)ptr->addr+i);
		  WriteP(REC_OSD_DISP_RAM_ADDR_H, (u8)((ptr->addr+i)>>8));
		  WriteP(REC_OSD_DISP_RAM_DATA, _SPACE_);
		  i++;
	  }
		ptr++;
		udelay(10);
 }
 
 WriteP2(0xe00, 0xff);
 WriteP2(0xe44, 0x0f);

}
/** @} */

