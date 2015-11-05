#ifndef _TW2828_HOST_H 
#define _TW2828_HOST_H

#include "tw2828_config.h"
#include "tw2828_reg.h"

void host_io_init (void);
void host_io_uninit (void);



void WriteReg(u32 addr, u8 dat);
void WriteRegWord(u32 addr, u32 dat);
u8 ReadReg(u32 addr);
u32 ReadRegWord(u32 addr);

#define WriteP(a,b)  WriteReg(a,b)
#define ReadP(a)     ReadReg(a)

void WriteReg2(u32 addr, u8 dat);
#define WriteP2(a,b)  WriteReg2(a,b)

#endif

