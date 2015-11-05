
#include <linux/kernel.h>
#include <linux/delay.h>
#include "tw2828_config.h"

BYTE Asc1Bin(BYTE asc)
{
    BYTE bin=0;

    if(asc>='0' && asc <='9') bin = (asc - '0');
    if(asc>='a' && asc <='f') bin = (asc - 'a' + 0x0a);
    if(asc>='A' && asc <='F') bin = (asc - 'A' + 0x0a);

    return bin;
}

BYTE Asc2Bin(BYTE *s)
{
    BYTE bin;

    bin = 0;
    while(*s != '\0' && *s !=' ') {
        bin = bin<<4;
        bin = bin + Asc1Bin(*s);
        s++;
    }
    return (bin);
}

WORD Asc2Word(BYTE *s)
{
    WORD bin;

    bin = 0;
    while(*s != '\0' && *s !=' ') {
        bin = bin<<4;
        bin = bin + Asc1Bin(*s);
        s++;
    }
    return (bin);
}

DWORD Asc2DWord(BYTE *s)
{
    DWORD bin;

    bin = 0;
    while(*s != '\0' && *s !=' ') {
        bin = bin<<4;
        bin = bin + Asc1Bin(*s);
        s++;
    }
    return (bin);
}



void delay(int msec)
{
    mdelay(msec);
}

