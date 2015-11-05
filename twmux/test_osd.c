

/*! 
 *************************************************************************************
 * \file  osd.c
 *
 * \brief
 *    display osd, rec/spot osd
 *
 * \date
 *    02/20/2012
 * \note 
 *    Modified to support Asian font(22x22)
 *
 * \author
 *    Heejeong Ryu <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
#if 0
#include <config.h>
#include <stdio.h>
#include <osd.h>
#include <tw2828_hostif.h>
#include <string.h>
#include <link.h>

#include "twmux.h"

#include "osddata_tbl.h"
#endif

#include <linux/kernel.h>
#include <linux/string.h>
#include "tw2828_config.h"
#include "tw2828_twmux.h"
#include "test_osd.h"
#include "test_osddata_tbl.h"

/** @defgroup OSD_Test /tw2828Test/OSD_Test
*   @ingroup tw2828Test
*   @addtogroup OSD_Test
*   @{
*/


void InitOSD(u8 no, int i)
{
	u8 host;

	printk("\n[%d] Load Display OSD Font %s", no, i==FONT_22x22?"FONT_22x22":"FONT_8x10");
	
	host = HostBusNo;
	SetHostBus(no);
	
	if(i==FONT_22x22)
		TWMux_DisplayOsd_LoadOSDFontData(no, SimpleOsdData_22x22); 
	else
		TWMux_DisplayOsd_LoadOSDFontData(no, SimpleOsdData_8x10); 

	//TWMux_DisplayOsd_LoadOSDDispData(no, (_DrawOsd *)CH_32_OSD_Tbl);
#if defined(VIDEO_2D1)
	TWMux_DisplayOsd_LoadOSDDispData(no, (_DrawOsd *)CH_8_OSD_Tbl_2d1);
#else
	#error "Notice! we use 2d1 by default" 
	TWMux_DisplayOsd_LoadOSDDispData(no, (_DrawOsd *)CH_8_OSD_Tbl_4d1);
#endif
	SetHostBus(host);
}

void InitRecOSD(u8 no, int i)
{
	u8 host;

	printk("\n[%d] Load Rec OSD Font %s", no, i==FONT_22x22?"FONT_22x22":"FONT_8x10");
	
	host = HostBusNo;
	SetHostBus(no);
	
	if(i==FONT_22x22)
		TWMux_RecOsd_LoadOSDFontData(no, SimpleYUVOsdData_22x22); 
	else
		TWMux_RecOsd_LoadOSDFontData(no, SimpleYUVOsdData_8x10);  

	//TWMux_RecOsd_LoadOSDDispData(no, (_DrawOsd *)REC_CH_16_OSD_Tbl);
#if defined(VIDEO_2D1)
	TWMux_RecOsd_LoadOSDDispData(no, (_DrawOsd *)REC_CH_8_OSD_Tbl_2d1);
#else
	#error "Notice! we use 2d1 by default" 
	TWMux_RecOsd_LoadOSDDispData(no, (_DrawOsd *)REC_CH_8_OSD_Tbl_4d1);
#endif
	
	SetHostBus(host);
}

void UpdateOSD(u8 winno, u32 ch)
{
    int i = 0;
#if defined(VIDEO_2D1)
    const _DrawOsd *ptr = &CH_8_OSD_Tbl_2d1[ch];
#else
	#error "Notice! we use 2d1 by default" 
    const _DrawOsd *ptr = &CH_8_OSD_Tbl_4d1[ch];
#endif
	int j = (winno / 2) * 4 + winno % 2;

	u32 addr = ADDR1 + j * OSDCHARSIZE;
    while(ptr->Str[i]!=_END_){
        //WriteP(OSD_DISP_RAM_ADDR_L, (u8)ptr->addr+i);
        //WriteP(OSD_DISP_RAM_ADDR_H, (u8)((ptr->addr+i)>>8));
        WriteP(OSD_DISP_RAM_ADDR_L, (u8)addr+i);
        WriteP(OSD_DISP_RAM_ADDR_H, (u8)((addr+i)>>8));
        WriteP(OSD_DISP_RAM_DATA, (u8)ptr->Str[i]);
        ++i;
    }
	WriteP(OSD_EN_CTRL, ReadP(OSD_EN_CTRL)|0x01);
}
/*! 
 * \brief
 *    Simple OSD test 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Description:
 *      Init OSD for main or record \n
 *      (1) Load Font data  \n
 *      (2) Load Display data \n
 * \par Usage:
 *      osd main <1/0>   : 1=22x22, 0=8x10 \n
 *      osd rec  <1/0>   : 1=22x22, 0=8x10 
 */
int OsdTest(int argc, char **argv)
{
	if ( !strcmp( (char *)argv[1], "main" ) ) {
		if ( !strcmp( (char *)argv[2], "1" ) ) 
			InitOSD(HostBusNo, 1);
		else
			InitOSD(HostBusNo, 0);
	}
	else if ( !strcmp( (char *)argv[1], "rec" ) ) {
		if ( !strcmp( (char *)argv[2], "1" ) ) 
			InitRecOSD(HostBusNo,1);
		else
			InitRecOSD(HostBusNo, 0);
	}
	  else
	  {
			printk("\nUsage:");
			printk("\nosd main : init main simple osd" );
			printk("\nosd rec : init rec simpleosd");
	  }

	return 1;
}

/** @} */ 
