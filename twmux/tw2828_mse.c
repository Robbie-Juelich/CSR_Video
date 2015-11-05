
/*! 
 *************************************************************************************
 * \file  <tw2828_mse.c>
 *
 * \brief
 *    Load Mouse Cursor image 
 *    (TW2828 has only Mouse Pointer Image Display, doesn't support PS2 feature.)
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    Heejeong Ryu hjryu@intersil.com Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
#include <linux/kernel.h>
#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"
#include "tw2828_msedata.h"

#define MaxMouseData		256 //32*16/4 *2mouse   (8*32)
#define MaxOneMouseData		128 //32*16/4   

u8 MouseNo = 0; //! Default = 0, select mouse sram
u8 MnMouseImgNo=0xff;

extern void Reset_dma(void);

/*! @defgroup Mouse /tw2828API/Mouse 
*   @ingroup tw2828API
*/
/*!
\addtogroup Mouse
\{
*/

void LoadMouseImage(u8 *ptr)
{

	int i=0,j;
	
	while (i<MaxMouseData)	{
		WriteP(MouseImgLoc, (u8)i);
		for(j=0;j<4;j++)
		  WriteP(MouseImgData, *ptr++);
		WriteP(MouseWriteEn, 0x00);
		i++;
	}

	printk("Load Mouse Cursor Image to SRAM\n\r");

}

void SetMseImageAddrBase(u32 addr)
{
	WriteP(MSEBASE_H, (u8)(addr>>16));
	WriteP(MSEBASE_M, (u8)(addr>>8));
	WriteP(MSEBASE_L, (u8)addr);
}

u32 GetMseImageAddrBase(void)
{
	u32 a=0;
	a = ReadP(MSEBASE_H);  a = a<<8;
	a |= ReadP(MSEBASE_M); a = a<<8;
	a |= ReadP(MSEBASE_L);
	
	printk("Get Mouse Address = %08x", a); 
	return a;
}

void LoadMouseImageByMAccess(u32 srcaddr)
{

	const u8 *ptr; 
	u32 addr;//, oldaddr;
	u32 i=0, j, check=0xff, cnt, checkcnt;
	u16 dat;
	u8 length;


   // mouse base address = (2048*2048*4 - mouse_size)/4
   // a = (4194304) - (8192/4);
	addr = (TOTALDSPMEMORYSIZE/4) - (8192/4) ;

	SetMseImageAddrBase(addr);
	ptr = (u8 *)srcaddr;

	#ifdef DEBUG_MOUSE
	dprintk("\r\n--(LoadMouseImageByMAccess) Base Addr %08x", addr);
	#endif

	Reset_dma();	

	for (i=0; i<16 ; i++ )	
	{
	  cnt = 512;  //u8 1 mouse size = 16*32 = 512 u8.
	  	
	  while(cnt) { 
			  
		WriteP(DRAM_ADDR_H, (u8)(addr>>16));
		WriteP(DRAM_ADDR_M, (u8)(addr>>8));
		WriteP(DRAM_ADDR_L, (u8)addr);

		if(cnt>=256) { length = 32;   cnt-=256; } //32*8
		else	     { length = cnt/4;  cnt=0;  }	// cnt*2/8;
		
		delay(10);
		WriteP(DRAM_RW_CTRL, 0xc0|length);


		for(j=0; j<(length*8); j++)		
		{
			dat = *ptr++;
			WriteP(DRAM_DATA,dat);
		}
		
		checkcnt=2;
		while(checkcnt--) {
			check = ReadP(DRAM_STATUS);
			if(check==1) { 
				//printk("*");
				break;
			}
			else {
				printk("*");
			}
		}

		addr = addr + 32;	// 32 length * 8 u8 = 256 u8 : 32=4*8unit

       } // while  	

	}

	WriteP(DRAM_RW_CTRL, 0x00);	// Stop

	if(check==1)
		printk("\n-(LoadMouseImageByMAccess) Load Mouse Cursor Image done.");
	else
		printk("\n-(LoadMouseImageByMAccess) Load Mouse Cursor Image fail!");


}

void SetMnMouseImagePosition(int x, int y)
{
	WriteP(MN_MSE_REG_UPDATE, 0x00);

	// mse 1
	{
		WriteP( MN_MSE0_HPOS_L, x&0xff);  //mouse h pos
		WriteP( MN_MSE0_HPOS_H, ((x&0xff00)>>8));  //mouse h pos
		WriteP( MN_MSE0_VPOS_L, y&0xff);  //mouse v pos
		WriteP( MN_MSE0_VPOS_H, ((y&0xff00)>>8));  //mouse v pos
	}

	// mse 2
	{
		WriteP( MN_MSE1_HPOS_L, x&0xff);  //mouse h pos
		WriteP( MN_MSE1_HPOS_H, ((x&0xff00)>>8));  //mouse h pos
		WriteP( MN_MSE1_VPOS_L, y&0xff);  //mouse v pos
		WriteP( MN_MSE1_VPOS_H, ((y&0xff00)>>8));  //mouse v pos
	}

	WriteP(MN_MSE_REG_UPDATE, 0x01);

}

void MnMouseEnable(u32 no)
{
	if(no)  WriteP(MN_MOUSECTRL, ReadP(MN_MOUSECTRL)|0x40);	// mouse cursor 1 on
	else	WriteP(MN_MOUSECTRL, ReadP(MN_MOUSECTRL)|0x04);	// mouse cursor 0 on
}

void MnMouseDisable(u32 no)
{
	if(no)	WriteP(MN_MOUSECTRL, ReadP(MN_MOUSECTRL)&0xbf);	// mouse cursor off
	else	WriteP(MN_MOUSECTRL, ReadP(MN_MOUSECTRL)&0xfb);	// mouse cursor off
}

void MnChangeMseImg(u8 no, u8 index)
{
	u32 i=0xff;
	u8 val=0;

	MnMouseImgNo = index;

	while(i--)
	{
		val = ReadP(MN_MSEIMGCTRL);
		if(val == 0 ) break;
		delay(10);
	}

	if((val&0x01) == 1) printk("\r\n --- Main Mouse Load image UPDATE = BUSY!! (Reg46f:0x%02x)",val);
	
	if(no==0)
		WriteP(MN_MSEIMGSEL, index&0x0f);	// mouse buf sel + index
	else
		WriteP(MN_MSEIMGSEL, 0x10|(index&0x0f));	// mouse buf sel + index


	WriteP(MN_MSEIMGCTRL, 0x01);

	//printk("\r\n --- MnMse Img #%d-%d",no,index);

}

u8 ChangeMouseNumber(u8 no)
{
	if(no) {
			MouseNo = 1;
			MnMouseDisable(0);
			MnMouseEnable(1);
	}
	else {
			MouseNo = 0;
			MnMouseDisable(1);
			MnMouseEnable(0);
	}

	return 1;
}

/*!
 ************************************************************************
 * \brief
 *    download mouse image to tw2828 memory
 * \param 
 *    <NONE>
 ************************************************************************
 */
u16 InitMse(u8 id)
{
	u16	 ret = 0; 

	printk("\n[%d] Init TW2828 Mouse Image", id);

	LoadMouseImageByMAccess((u32)&MseCursorImgDataTbl);
  delay(10);
	MnMouseEnable(0);
	MnChangeMseImg(0,MseCursor_arrow);

	return ret;
}
/** @} */
