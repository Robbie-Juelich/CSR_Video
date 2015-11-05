
/*! 
 *************************************************************************************
 * \file  tw2828_freeze.c
 *
 * \brief
 *    freeze function
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

/*! @defgroup Freeze /tw2828API/Freeze
*   @ingroup tw2828API
*/
/*!
\addtogroup Freeze
\{
*/

/*!TWMux_SetFreezeChannelMain
  \brief
		Freeze main window channel 
	\retval <NONE>
	\param  [in] id      : chip id
	\param  [in] pbmode  : Live, or PB Normal mode = 0, PB Auto mode = 1
	\param  [in] a       : buffer number
	\param  [in] on      : Enable
	\note Freeze function is managed by buffer number, not channel number.
*/
void TWMux_SetFreezeChannelMain(u8 id, u8 pbmode, u8 a, u8 on)
{
	if(pbmode==0)
	{
	  if(a<20)
	  {
		  if(on==1)
			WriteP( (WIN0_CTRL+a), ReadP(WIN0_CTRL+a) | 0x08);
		  else
			WriteP( (WIN0_CTRL+a), ReadP(WIN0_CTRL+a) & 0xf7);
	  }
	}
	else
	{
		if(on==1) {
			if( a<8)	
				WriteP(FREEZE_PB_L, ReadP(FREEZE_PB_L)|(0x01<<a));
			else 
				WriteP(FREEZE_PB_H, ReadP(FREEZE_PB_H)|(0x01<<(a-8)));
		}
		else
		{
			if(a<8)
				WriteP(FREEZE_PB_L, ReadP(FREEZE_PB_L)&~(0x01<<a));
			else 
				WriteP(FREEZE_PB_H, ReadP(FREEZE_PB_H)&~(0x01<<(a-8)));
		}
	}

	printk("\r\n Freeze main automode:%d bufno:%d on/off:%d", pbmode, a, on);
}

/*!TWMux_SetFreezeChannelRec 
  \brief
		Freeze Rec buffer
	\retval <NONE>   
	\param  [in] a       : buffer number
	\param  [in] on      : Enable
*/
void TWMux_SetFreezeChannelRec(u8 id, u8 a, u8 on)
{
		if(on==1) {
			if( a<8)	
				WriteP(FREEZE_REC_L, ReadP(FREEZE_REC_L)|(0x01<<a));
			else 
				WriteP(FREEZE_REC_H, ReadP(FREEZE_REC_H)|(0x01<<(a-8)));

			printk("\r\n Freeze rec bufno:%d ofn", a);
		}
		else
		{
			if(a<8)
				WriteP(FREEZE_REC_L, ReadP(FREEZE_REC_L)&~(0x01<<a));
			else 
				WriteP(FREEZE_REC_H, ReadP(FREEZE_REC_H)&~(0x01<<(a-8)));

			printk("\r\n Freeze rec bufno:%d off", a);
		}
}

/*! 
  \brief
		Freeze Spot buffer
	\retval 
	      <NONE>   
	\param  [in] a       : buffer number
	\param  [in] on      : Enable
	\par Update history:
		JUNE/20/2012 - Changed for ASIC
*/
void TWMux_SetFreezeChannelSpot(u8 id, u8 a, u8 on)
{
		if(on==1) {
			if( a<8)	
				WriteP(FREEZE_SPOT_L, ReadP(FREEZE_SPOT_L)|(0x01<<a));
			else 
				WriteP(FREEZE_SPOT_H, ReadP(FREEZE_SPOT_H)|(0x01<<(a-8)));

			printk("\r\n Freeze spot bufno:%d on", a);
		}
		else
		{
			if(a<8)
				WriteP(FREEZE_SPOT_L, ReadP(FREEZE_SPOT_L)&~(0x01<<a));
			else 
				WriteP(FREEZE_SPOT_H, ReadP(FREEZE_SPOT_H)&~(0x01<<(a-8)));

			printk("\r\n Freeze spot bufno:%d off", a);
		}

}

/** @} */


