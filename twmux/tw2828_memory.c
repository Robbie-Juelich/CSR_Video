
/*! 
 
 * \file  tw2828_memory.c
 *
 * \brief
 *    DCM, memory handling
 *
 * \date
 *    02/20/2012
 *
 * \author
 *    - Heejeong Ryu  <hjryu@intersil.com> Copyright(c) 2012 Intersil Techwell, Mipitas USA.   
 *    
 *************************************************************************************
 */
//#include "config.h"
#include <linux/kernel.h>
#include <linux/delay.h>
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"
#include "tw2828_reg.h"
/*! @defgroup Memory /tw2828API/Memory 
*   @ingroup tw2828API
*/
/*!
\addtogroup Memory
\{
*/

//#define MCLK 333000
//#define MCLK 337500
#define MCLK 378000


u32 gMCLK = MCLK;

extern void Reset_dma(void);

inline void SetMCLK(u32 clk)
{

	if(clk == 378000)
  	WriteP2(0x202, 27);    //MCLK=378.0MHz  	
	else if(clk == 351000)
  	WriteP2(0x202, 25);    //MCLK=351.0MHz  	
	else 	if(clk == 337500) //MCLK=337.5MHz
	  WriteP2(0x202, 24);
	else 	if(clk == 324000) //MCLK=324000
	  WriteP2(0x202, 23);
  delay(10);                                   	
}

inline void LoadDCMData(u32 clk)
{
	int ratio;

 WriteP(0x20b, 0xff);
 WriteP(0x20c, 0xff);
 WriteP(0x20d, 0xff);
 WriteP(0x20e, 0xff);
 WriteP(0x20f, 0xff);
 delay(10);  // by zzc
 if (clk<=333000) {	
 	//dcm init 333M
  WriteP(0x110, 0x03); // default 0x03
  WriteP2(0x112, 0x00); // default ?? 
  WriteP2(0x113, 0x00); // ??
  WriteP2(0x114, 0xc8); // Config1 = 0x608c -> 0x60BC -> 0x60c8
  WriteP2(0x115, 0x60);
  WriteP2(0x116, 0x00);
  WriteP2(0x117, 0x00);
  WriteP2(0x118, 0x7d);  // default is 0x71
  WriteP(0x119, 0x85); //0xa5);  // defaule is 0x63
  WriteP(0x11a, 0x00);
  WriteP(0x11b, 0x00);
  WriteP(0x11c, 0x02);
  WriteP(0x11d, 0x10);
  WriteP(0x11e, 0x00);
  WriteP(0x11f, 0x00);
  WriteP(0x120, 0xff);
  WriteP(0x121, 0x5f);
  WriteP(0x122, 0x00);
  WriteP(0x123, 0x00);
  WriteP(0x124, 0xae);
  WriteP(0x125, 0x09);
  WriteP(0x126, 0x00);
  WriteP(0x127, 0x00);
  WriteP(0x128, 0x04);
  WriteP(0x129, 0x02);
  WriteP(0x12a, 0x00);
  WriteP(0x12b, 0x00);
  WriteP(0x130, 0x42); //0x52);
  WriteP(0x131, 0x0a);
  WriteP(0x132, 0x00);
  WriteP(0x133, 0x00);
  WriteP(0x134, 0x00);
  WriteP(0x135, 0x00);
  WriteP(0x136, 0x00);
  WriteP(0x137, 0x00);
  WriteP(0x138, 0xc4);
  WriteP(0x139, 0x03);
  WriteP(0x13a, 0x00);
  WriteP(0x13b, 0x00);
  WriteP(0x13c, 0x44);
  WriteP(0x13d, 0x00);
  WriteP(0x13e, 0x00);
  WriteP(0x13f, 0x00);
  WriteP(0x140, 0x00);
  WriteP(0x141, 0xc2);
  WriteP(0x142, 0x00);
  WriteP(0x143, 0x00);
  WriteP(0x144, 0x31);
  WriteP(0x145, 0x13);
  WriteP(0x146, 0x00);
  WriteP(0x147, 0x00);
  WriteP(0x148, 0x24);
  WriteP(0x149, 0x1b);
  WriteP(0x14a, 0x00);
  WriteP(0x14b, 0x00);
  WriteP(0x14c, 0xe1);
  WriteP(0x14d, 0x11);
  WriteP(0x14e, 0x00);
  WriteP(0x159, 0xa0); 
  WriteP(0x140, 0x10); //start
 }	
 else  //clk==378000
 {
  WriteP(0x110, 0x03);  // default is 0x3
  WriteP2(0x111, 0x95); 	// default is 0x75
  WriteP2(0x112, 0x00);
  WriteP2(0x113, 0x00);
  WriteP2(0x114, 0xc8); // Config1 = 0x608c -> 0x60BC -> 0x60c8
  WriteP(0x115, 0x60);  // default is 0x60
  WriteP2(0x116, 0x00);  //  
  WriteP2(0x117, 0x00);
  WriteP2(0x118, 0x7d); //0xa5);
  WriteP2(0x119, 0xa5); //0xa6);
  WriteP2(0x11a, 0x00);
  WriteP2(0x11b, 0x00);
  WriteP(0x11c, 0x02);
  WriteP2(0x11d, 0x18);
  WriteP2(0x11e, 0x00);
  WriteP2(0x11f, 0x00);
  WriteP(0x120, 0xff);  // default is 0xff
  WriteP2(0x121, 0x6f);
  WriteP2(0x122, 0x00);
  WriteP2(0x123, 0x00);
  WriteP2(0x124, 0xae);
  WriteP2(0x125, 0x09);
  WriteP2(0x126, 0x00);
  WriteP2(0x127, 0x00);
  WriteP2(0x128, 0x04);
  WriteP2(0x129, 0x02);
  WriteP2(0x12a, 0x00);
  WriteP2(0x12b, 0x00); //0x52);
  WriteP2(0x130, 0x52);
  WriteP2(0x131, 0x0a);
  WriteP2(0x132, 0x00);
  WriteP2(0x133, 0x00);
  WriteP(0x134, 0x00);
  WriteP(0x135, 0x00);
  WriteP2(0x136, 0x00);
  WriteP2(0x137, 0x00);
  WriteP(0x138, 0xc4);
  WriteP(0x139, 0x03);
  WriteP2(0x13a, 0x00);
  WriteP2(0x13b, 0x00);
  WriteP(0x13c, 0x44);
  WriteP(0x13d, 0x00);
  WriteP2(0x13e, 0x00);
  WriteP2(0x13f, 0x00);
  WriteP2(0x140, 0x00); //  ?? notice will change
  WriteP2(0x141, 0xc2);
  WriteP2(0x142, 0x00);
  WriteP2(0x143, 0x00);
  WriteP2(0x144, 0x31);
  WriteP2(0x145, 0x13);
  WriteP2(0x146, 0x00);
  WriteP2(0x147, 0x00);
  WriteP2(0x148, 0x24);
  WriteP2(0x149, 0x1b);
  WriteP2(0x14a, 0x00);
  WriteP2(0x14b, 0x00);
  WriteP2(0x14c, 0xe1);
  WriteP2(0x14d, 0x11);
  WriteP2(0x14e, 0x00);
  WriteP2(0x159, 0xa0); 
  WriteP2(0x140, 0x10); //start
 }
	
		//gain ratio
	ratio = 0x40; //0x50;
	WriteP2(0x163, (u8)ratio);
	WriteP2(0x164, (u8)((ratio<<3)&0xf8)|((ratio>>8)&0x07));
	WriteP2(0x165, (u8)(ratio>>5));

	WriteP(0x150, 0x01); //cal. start
  //delay(1);
  delay(10);
 
  WriteP(0x20f, 0xaf);
  
  delay(10);
  WriteP(0x20b, 0);
  WriteP(0x20c, 0);
  WriteP(0x20d, 0);
  WriteP(0x20e, 0);
  WriteP(0x20f, 0);

  delay(20);
	
}

int InitDCM(u8 id)
{
	SetMCLK(gMCLK);
	
	LoadDCMData(gMCLK);
	
	if((ReadP(0x100)&0x01) == 0x01) {
		printk("\n[%d] MCLK=%d - DCM Init Done!", id, gMCLK);
	}
	else {
		printk("\n[%d] MCLK=%d.%03d MHz - DCM Init Error!", id, gMCLK/1000, gMCLK%1000);
    	return 1;
	}
	
	return 0;
}

int ReadMemoryCheck(u32 addr, u8 *ptr, u8 length) 
{
	u8 i, dat_l, dat_h;	//2*8 unit = 16 u8
  int ret=0;
  
	Reset_dma();

	WriteP( DRAM_ADDR_H, (u8)(addr>>16));
	WriteP( DRAM_ADDR_M, (u8)(addr>>8));
	WriteP( DRAM_ADDR_L, (u8)addr);

	 // Length = max 8.
	WriteP( DRAM_RW_CTRL, 0x40|length);

	printk("\r\nRead addr %08x(%02x)  = ", addr,length);
	
	for(i=0; i<length*8;  )
	{
		dat_h = ReadP( DRAM_DATA+i);
		i++;
		dat_l = ReadP( DRAM_DATA+i);
		i++;
		if(*ptr++ != dat_l) ret = 1; //not match
		if(*ptr++ != dat_h) ret = 1; //not match

		printk("%02x ", dat_l);
		printk("%02x ", dat_h);
	}
	
	return ret;
}

int check_dram(void)
{
const u8 mtesttbl[] = {
//	0,1,2,3,4,5,6,7,8,9,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
//	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
//	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
	  0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00,
};

	u32 addr=0;
	u8 buf[16];
	int i;
	
	for(i=0; i<16; i++)
	  buf[i] = mtesttbl[i];
	WriteMemory( addr, buf, 2);
	if(ReadMemoryCheck( addr, buf, 2)) 
		  return 1;  //not match
	
	return 0; //ok
}

int DCM_EyeCalibration(void)
{
	int ratio=0;

	WriteP(0x173, 0x2a); 
	WriteP(0x150, 0x05); //reg150.0=1
	
	if(!check_dram()) 
	{	
		printk("\r\n DCM Success!!"); 
		goto Dcm_exit;
	}

	printk("\r\n DCM Calibration start.."); 

	WriteP(0x173, 0x2a); 
	
	//Eye Training
	for(ratio=0; ratio<0x100; ratio+=0x10)
	{
		WriteP(0x15d, (u8)ratio);
		WriteP(0x15e, (u8)((ratio<<2)&0xfc)|((ratio>>8)&0x03));
		WriteP(0x15f, (u8)(ratio>>6));

	  WriteP(0x150, 0x05); //reg150.0=1

		printk("\r\n -- Eye Training ratio = %03x", ratio); 
		if(!check_dram()) 
		{	
			printk("\r\n DCM Eye Training Success!!"); 
			goto Dcm_exit;
		}
		
	}
	return -1;
	
Dcm_exit:	
	WriteP(0x173, 0x6a); 
	printk("\r\n DCM calibration exit."); 

  ratio = 0x40;
	WriteP(0x15d, (u8)ratio);
	WriteP(0x15e, (u8)((ratio<<2)&0xfc)|((ratio>>8)&0x03));
	WriteP(0x15f, (u8)(ratio>>6));
	return 0;	
}

void DCM_GateCalibration(void)
{
	int ratio=0;

  ratio = 0x40;
	WriteP(0x15d, (u8)ratio);
	WriteP(0x15e, (u8)((ratio<<2)&0xfc)|((ratio>>8)&0x03));
	WriteP(0x15f, (u8)(ratio>>6));

	WriteP(0x173, 0x2a); 
	WriteP(0x150, 0x05); //reg150.0=1
	
	if(!check_dram()) 
	{	
		printk("\r\n DCM Success!!"); 
		goto Dcm_exit;
	}

	printk("\r\n DCM Calibration start.."); 

	WriteP(0x173, 0x2a); 
	
	//Gate Training
	for(ratio=0; ratio<0x100; ratio+=0x10)
	{
		WriteP(0x163, (u8)ratio);
		WriteP(0x164, (u8)((ratio<<3)&0xf8)|((ratio>>8)&0x07));
		WriteP(0x165, (u8)(ratio>>5));

	  WriteP(0x150, 0x05); //reg150.0=1

		printk("\r\n -- Gate Training ratio = %03x", ratio); 
		if(!check_dram()) 
		{	
			printk("\r\n DCM Gate Training Success!!"); 
			goto Dcm_exit;
		}
	}
	
Dcm_exit:	
	WriteP(0x173, 0x6a); 
	printk("\r\n DCM calibration exit."); 

	  ratio = 0x40;
		WriteP(0x163, (u8)ratio);
		WriteP(0x164, (u8)((ratio<<3)&0xf8)|((ratio>>8)&0x07));
		WriteP(0x165, (u8)(ratio>>5));
	
}

/*
  \brief
		Reset dram access module 
	\retval <NONE> 
*/
void Reset_dma(void)
{
	WriteP(DGTBLK5RESET, ReadP(DGTBLK5RESET) | 0x20); // 20f[5] Reset DRAM Access
	WriteP(DGTBLK5RESET, 0); 
}

u32 WriteDataToMemoryUsingMAccess( u32 saddr, u32 dBase, u32 pitch, u32 dx, u32 dy, u32 bigend)
{
	u32 addr;
	u32 w, width;
	u32 height;
	u32 i=0, j, k, cnt;
	u8 dat;
	u8 *ptr;
	//u8 dat_l, dat_h;
	u8 length;
	u32 oldaddr, check=0xff;


	addr = dBase + (pitch*dy + dx)/4;

	width = *(u16 *)(saddr+2);
	height = *(u16 *)(saddr+4);

	ptr = (u8 *)(saddr+20);

//	printk("\n--(WriteDataToMemoryUsingMAccess) saddr: %08x, Addr %08x\n, pitch:%04x, dx:%02x dy:%02x w:%d h:%d", saddr, addr, pitch, dx, dy, width, height);
  if((width>1920) || (height>1080)) {
  	 printk("\n Image Data Error!!");
  	 return -1;
  }
//	 bigend = 0x1;  
	if(bigend) {
		WriteP(DRAM_BIGENDIAN_EN, 0x01);
//		printk("\r\n--Big Endian");
	}
	else {
		WriteP(DRAM_BIGENDIAN_EN, 0x00);
//		printk("\r\n--Little Endian");
	}

 
	for (i=0; i<height ; i++ )	
	{

	  oldaddr = addr;		
	  k = 0;
	  w = width; 
	 // w = 816;//test
	  	
	  while(w) { 
			  
		addr = oldaddr + k*32;	// 32=4*8unit
		k++;
		
		Reset_dma();	

		WriteP(DRAM_ADDR_H, (u8)(addr>>16));
		WriteP(DRAM_ADDR_M, (u8)(addr>>8));
		WriteP(DRAM_ADDR_L, (u8)addr);

		if(w>=128) { length = 32;   w-=128; }   // 128pixel (256 u8)
		else	     { length = w/4;  w=0;    }	// w*2/8;
		
		delay(1);
		//WriteP(DRAM_RW_CTRL, length);
		WriteP(DRAM_RW_CTRL, 0xc0|length);

		//printk("\r\nAddr %08x, h:%d, l:%d  w:%d ", addr, i, length, w);
		
		for(j=0; j<(length*8); j++)		
		{
			dat = *ptr++;
			//if(bigend) 
			//	dat = ((dat<<8)&0xff00) | ((dat>>8)&0x00ff);

			//dat = 0x10;
			//if(i==0)
			//if(check==0xff)
			//printk("%04x ", dat);
			//WritePWord(DRAM_DATA,dat);
			WriteP(DRAM_DATA,dat);
		}
		
		cnt = 3;
		while(cnt--) {
			check = ReadP(DRAM_STATUS);
			if(check==1) {  // done
				break;
			}
			else { // need more time?
				printk("*-*-\t");
				delay(10);
			}
		}

		if( cnt ==0 ) {
			printk("\n---- men error******************\n");
			//return 0; //error
		}

       } // while  	

	   addr = oldaddr + (pitch/4); //512 // 4096/8u8unit  

	}

	WriteP(DRAM_RW_CTRL, 0x00);	// Stop

	return width;
}

void WriteMemory(u32 addr, u8 *ptr, u8 length)
{
	u8 dat_l,dat_h; 	// Length : 8 u8 unit (4Pixel) = Max 63*8 = 504 u8
	u32 i;

	Reset_dma();

	WriteP( DRAM_ADDR_H, (u8)(addr>>16));
	WriteP( DRAM_ADDR_M, (u8)(addr>>8));
	WriteP( DRAM_ADDR_L, (u8)addr);

	WriteP( DRAM_RW_CTRL, 0xc0|length);

	printk("\r\nWrite addr %08x(%02x) = ", addr,length);

	for(i=0; i<(length*8); i+=2)
	{
		dat_l = *ptr++;
		dat_h = *ptr++;

		printk("%02x ", dat_l);
		printk("%02x ", dat_h);
		WriteP( DRAM_DATA,dat_h);
		WriteP( DRAM_DATA,dat_l);
	}

}

void ReadMemory1(u32 addr, u8 length, u8 *buf)
{
	u8 i, dat_l, dat_h;

//	if (buf == NULL) return;
	Reset_dma();

	WriteP( DRAM_ADDR_H, (u8)(addr>>16));
	WriteP( DRAM_ADDR_M, (u8)(addr>>8));
	WriteP( DRAM_ADDR_L, (u8)addr);

	WriteP( DRAM_RW_CTRL, 0x40|length);
	for (i = 0; i < length * 8; ) {
		dat_h = ReadP( DRAM_DATA+i);
		i++;
		if (buf)
			buf[i] = dat_h;
		dat_l = ReadP( DRAM_DATA+i);
		i++;
		if (buf)
			buf[i] = dat_l;

    //    printk("%02x ", dat_h);
    //    printk("%02x ", dat_l);
	}
}

void ReadMemory(u32 addr, u8 length) 
{
	u8 i, dat_l, dat_h;	//2*8 unit = 16 u8

	// Note: Read Data port : 0x004 - 0x0043 //(32 * WORD)
	// * Read length <= 2

	Reset_dma();

	WriteP( DRAM_ADDR_H, (u8)(addr>>16));
	WriteP( DRAM_ADDR_M, (u8)(addr>>8));
	WriteP( DRAM_ADDR_L, (u8)addr);

	 // Length = max 8.
	WriteP( DRAM_RW_CTRL, 0x40|length);

	printk("\r\nRead addr %08x(%02x) = ", addr,length);
	
	for(i=0; i<length*8;  )
	{
		dat_h = ReadP( DRAM_DATA+i);
		i++;
		dat_l = ReadP( DRAM_DATA+i);
		i++;

		printk("%02x ", dat_l);
		printk("%02x ", dat_h);
	}
	
}

void TWMux_WriteDisplayToMemory(u32 sAddr, u32 x, u32 y)
{
	u32 base, pitch;

	base = 0;
	pitch = ReadP(DISPMEMPITCH_H) & 0x80; pitch<<=1;
	pitch |= ReadP(DISPMEMPITCH);
//	printk("[%s] pitch is %d\n", __func__, pitch);
	WriteDataToMemoryUsingMAccess(sAddr, base, pitch*16, x ,y, 0);
}

int TWMux_ChangeMclk(u8 id, u32 mclk)
{
	 if((mclk < 4000000) && (mclk>250000)){
     gMCLK = mclk;
   	 printk("\nInput MCLK = %d.%03d MHz", gMCLK/1000, gMCLK%1000); 
   }
   else {
   	 printk("\nInput MCLK error! Current MCLK = %d.%03d MHz", gMCLK/1000, gMCLK%1000); 
   }
   
   return gMCLK;
}

int TWMux_GetMclk(u8 id)
{
   return gMCLK;
}

/** @} */

