#include "tw2828_hostio.h"

void do_test_pattern(void)
{
    u8 val;
    val = ReadP(0x485);
    WriteP(0x485, val | (0x1 << 2));

    val = ReadP(0x3c5);
    WriteP(0x3c5, (val | (1 << 5) | (1 << 4)) & (~(1 << 6)));
    //WriteP(0x3c5, val | (1 << 5) | (1 << 4) | (1 << 6));
    val = ReadP(0x6b4);
    WriteP(0x6b4, (val & (~0x1)) | 0x2 | 0x4 );
}

void do_rec_test_pattern(void)
{
    u8 val;
    val = ReadP(0xc4d);
    WriteP(0xc4d, (val | 0x4) | 0x2);  // set bit 2
}

void undo_rec_test_pattern(void)
{
    u8 val;
    val = ReadP(0xc4d);
    WriteP(0xc4d, (val & (~0x4)) | 0x2);  // set bit 2
}

void enable_channel_id_1bit(void)
{
    u8 val;
    val = ReadP(0x3c5);
    val = (val & (~0x3)) & (~(0x1 << 2));
    WriteP(0x3c5, val);
}


void enable_channel_id_2bit(void)
{
    u8 val;
    val = ReadP(0x3c5);
    val = (val & (~0x3)) | ((0x1 << 2));
    WriteP(0x3c5, val);
}

void undo_test_pattern(void)
{
    u8 val;
    val = ReadP(0x485);
    WriteP(0x485, (val &(~(0x1 << 2)) ) & (~(0x1 << 3)));

    val = ReadP(0x6b4);
    WriteP(0x6b4, (val & (~0x1)));
}

