
/*! 
 *************************************************************************************
 * \file  memory.c
 *
 * \brief
 *    memory read/write test, DCM calibration test
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
#include <config.h>
#include <stdio.h>

#include "twmux.h"

#include <disp.h>
#include <rec.h>
#include <mse.h>
#include <system.h>
#include <tw2828_hostif.h>
#include <time.h>
#include <string.h>
#include <monitor.h>
#include <mdbox.h>
#include <osd.h>
#include <mem_map.h>
#include <spot.h>
#include "link.h"
#endif

#include <linux/string.h>
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"
/** @defgroup Memory_Test /tw2828Test/Memory_Test
*   @ingroup tw2828Test
*   @addtogroup Memory_Test
*   @{
*/

//-----------------------------------------------------------------
/*
const BYTE memorytesttbl[] = {
//	0,1,2,3,4,5,6,7,8,9,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
};
*/

int TestMemoryWrite(int argc, char **argv)
{
	u32 addr;
	u32 dat;
	u8 buf[16];
	int i;
	
  // display memory access
	// mem r addr 
	// mem w addr xxxxxxxx
  // mem
	
	addr = (unsigned int)Asc2DWord( argv[2] );

	if(argc == 4) {
		dat = (u32)Asc2DWord( argv[3] );
		printk("\ndat=%08x",dat);
		for(i=0;i<16;i+=4){
			buf[i+3]=(u8)dat;
			buf[i+2]=(u8)(dat>>8);
			buf[i+1]=(u8)(dat>>16);
			buf[i]=(u8)(dat>>24);
		}
	}

	if ( !strcmp( (char *)argv[1], "r" ) ) {
		ReadMemory( addr, 2); 
	}
	else if ( !strcmp( (char *)argv[1], "w" ) ) {
		WriteMemory( addr, buf, 2);
	}
	else {
		printk("\r\nUsage: ");
		printk("\r\n mem r <addr>        : read memory ");
		printk("\r\n mem w <addr> <data> : write memory data - xxxxxxxx");

	}

	return 1;
}

/*! 
 * \brief
 *    Memory write test - Write data from IMAGE_RAM_START into display memory area
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *     dram <x> <y>
 */
int Test_HostDRAMWrite(int argc, char **argv)
{
	u32 x, y;

  
	x = (unsigned int) Asc2Word(argv[1]);
	y = (unsigned int) Asc2Word(argv[2]);

	TWMux_WriteDisplayToMemory( IMAGE_RAM_START+0, x, y );

	return 0;
}

/*! 
 * \brief
 *    DCM Calibration - Eye Training 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *     dcme
 */
int TestDcmEyeCalibration(int argc, char **argv)
{
	int host;
	
	// dcme [hostbusno]
	if(argc<2) {
		
		DCM_EyeCalibration();
	}
	else 
	{
		host = HostBusNo;
		SetHostBus((int) Asc2Word(argv[1]));

		DCM_EyeCalibration();
		
		SetHostBus(host);
	}
	
	return 1;
}

/*! 
 * \brief
 *    DCM Calibration - Gate Training 
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *     dcmg
 */
int TestDcmGateCalibration(int argc, char **argv)
{
	int host;
	
	// dcmg [hostbusno]
	if(argc<2) {
		
		DCM_EyeCalibration();
	}
	else 
	{
		host = HostBusNo;
		SetHostBus((int) Asc2Word(argv[1]));

		DCM_GateCalibration();
		
		SetHostBus(host);
	}
	return 1;
}

/*! 
 * \brief
 *    Set grobal Mclk for TW2828 A, B
 *
 * \param [in] argc 
 * \param [in] **argv 
 *
 * \return
 *    <NONE>
 * \par Usage:
 *     mclk [<0/1/2>]   : 0=378000, 1=351000, 2=337500, 3=333000
 */
extern int TWMux_ChangeMclk(u8 id, u32 mclk);
extern int TWMux_GetMclk(u8 id);

int SetMClock(int argc, char **argv)
{
	int mclk,i;
	int mclk_tb[]= {378000, 351000, 337500, 333000};
	
	if(argc<2) {
		
		mclk = TWMux_GetMclk(HostBusNo);
    printk("\nCurrent MCLK = %d \nUsage: mclk <0/1/2/3> :", mclk ); 
    for(i=0; i<4; i++)
    printk("%d=%d ", i,mclk_tb[i]); 
 
	}
	else 
	{
		i = (int) Asc2Bin(argv[1])%3;
		TWMux_ChangeMclk(HostBusNo, mclk_tb[i]);
	}
	return 1;
}

/** @} */ 
