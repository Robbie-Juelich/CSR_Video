/*! 
 *************************************************************************************
 * \file  rec.c
 *
 * \brief
 *    record application and test
 *
 * \date
 *    02/20/2012
 *    
 * \note 
 *    tw2828 record cann't support 960H.
 *
 * \author
 *    Heejeong Ryu <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *************************************************************************************
*/
#if 0
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "tw2828_hostif.h"
#include "disp.h"

#include "twmux.h"
#include "rec.h"
#include "spot.h"
#include "rec_tbl.h"
#endif

#include <linux/kernel.h>
#include <linux/string.h>
#include "tw2828_hostio.h"
#include "tw2828_config.h"
#include "tw2828_twmux.h"
#include "tw2828_test.h"
#include "test_rec_tbl.h"
/** @defgroup Record_Test /tw2828Test/Record_Test
*   @ingroup tw2828Test
*   @addtogroup Record_Test
*   @{
*/

//---------------------------------------------------------------
// Rec Control Application
//---------------------------------------------------------------


void SetEncChidInit(u8 id)
{

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
//	4, //pixel_w
	0x05, //min_val
	0x3f, //max_val
  };
	u8 p;
	
	for(p=0; p<5; p++)
		TWMux_SetRecordChannelIDEnc(id, p, Default_Chid_Enc);
}

void InitRecforMdsp(u8 id, u8 mode)
{
	
	printk("\n[%d] InitRecforMdsp", id);

   switch(mode){
   case MODE_20CH:	
   case MODE_6CH:	
   case MODE_12CH_CASCADE_Master:		
   		TWMux_LoadRecordDataAll(id, rTb_Rec_4xD1_27M_8b_IFrame_rport5_8);
			break;

   case MODE_12CH:	
   		TWMux_LoadRecordDataAll(id, rTb_Rec_4D1_54M_16b_IFrame);
			break;

   case MODE_16PB_4LIVE:
   case MODE_32CH_ONLY:	
   case MODE_32CH_ONLY_2:	
	  	TWMux_LoadRecordDataAll(id, rTb_Rec_D1_108M_8b_Frame);
		break;

   case MODE_16QUADPB:	
   case MODE_32CH:	
		  TWMux_LoadRecordDataAll(id, rTb_Spot_16Quad_54M_8b_IFrame);
   break;

   case MODE_2_8D1PB_4LIVE:	
		  TWMux_LoadRecordDataAll(id, rTb_Rec_2_8D1_108M_16b_Frame);
   break;

   case MODE_16D1_FS_NEW:
   case MODE_32CH_ONLY_3:		 
		TWMux_LoadRecordDataAll(id, rTb_Rec_16D1_108M_8b_NewFieldSwitch);
		//SetNewFieldSwitchOnAll();
   break;
   }

}

//-------------------------------------------------------------------------
int RecTest(int argc, char **argv)
{

	// command format: rec 1d1 iframe 8b

	if ( !strcmp( (char *)argv[1], "1d1" ) ) {

		if ( !strcmp( (char *)argv[2], "iframe" ) ) {
		
			TWMux_LoadRecordDataAll(HostBusNo, rTb_Rec_4xD1_27M_8b_IFrame_rport5_8);
		}
	}
/*
	//------------------------------------------------
	else if ( !strcmp( (char *)argv[1], "2d1" ) ) {

	  if ( !strcmp( (char *)argv[3], "8b" ) ) {

	  	
		if ( !strcmp( (char *)argv[2], "field" ) ) {
//			TWMux_LoadRecordDataAll(HostBusNo, rTb_Rec_2xD1_54M_8b_Field);
		}
	  }
	  else  if ( !strcmp( (char *)argv[3], "16b" ) ) {

		if ( !strcmp( (char *)argv[2], "field" ) ) {
//			TWMux_LoadRecordDataAll(HostBusNo, rTb_Rec_2xD1_54M_16b_Field);
		}
	  }
	}

	*/
	//------------------------------------------------
	else if ( !strcmp( (char *)argv[1], "4d1" ) ) {
	//	TWMux_LoadRecordDataAll(HostBusNo, rTb_Rec_4D1_108M_8b_Frame);
	}

	//------------------------------------------------
	return TRUE;

}
//---------------------------------------------------------------

/** @} */ 
