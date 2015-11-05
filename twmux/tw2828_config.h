#ifndef _TW2828_CONFIG_H
#define _TW2828_CONFIG_H

#include <linux/delay.h>

#define SUPPORT_SXGA60
#define SUPPORT_HD1080P
#define SUPPORT_1920x1200
#define SUPPORT_1680x1050
#define SUPPORT_720P
#define SUPPORT_1080I
#define SUPPORT_1080I50
#define SUPPORT_1600x900
#define SUPPORT_HD1080P50
#define SUPPORT_720P50

#define FALSE  0
#define TRUE  -1 


#define ON      1
#define OFF     0

#define YES     0xfe    
#define NO      0

#define UP      1
#define DN      0

#define NIL         0xff

#define IMAGE_RAM_START 0

#define DSP1Gb      
//#define DSP512Mb      
//#define DSP256Mb      // Use 256MB SDRAM for DSP path.
//#define DSP128Mb


#ifdef DSP128Mb
    #define TOTALDSPMEMORYSIZE  (2048*2048*4)
    #define PAGEMEMORYSIZE      (2048*2048)
#elif defined DSP256Mb
    #define TOTALDSPMEMORYSIZE  (2048*4096*4)
    #define PAGEMEMORYSIZE      (2048*4096)
#elif defined DSP512Mb
    #define TOTALDSPMEMORYSIZE  (2048*8192*4)
    #define PAGEMEMORYSIZE      (2048*8192)
#elif defined DSP1Gb
    #define TOTALDSPMEMORYSIZE  (2048*8192*2*4)
    #define PAGEMEMORYSIZE      (2048*8192*2)
    #define PAGEMEMORYSIZE1     (2880*5825*2)
#endif



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

extern u8 HostBusNo;

extern BYTE Asc1Bin(BYTE asc);
extern BYTE Asc2Bin(BYTE *s);
extern void delay(int msec);
extern WORD Asc2Word(BYTE *s);
extern DWORD Asc2DWord(BYTE *s);

#define SetHostBus(a) HostBusNo=(a&0x1)
#define GetHostBus()  HostBusNo


#endif
