#ifndef _TW2828_DIGIT_H
#define _TW2828_DIGIT_H

#include "test_disp.h"
#include "tw2828_ioctl.h"

#define LOGO_4ch  0x0
#define LOGO_6ch  0x1
#define LOGO_9ch  0x2


typedef struct Char_Info_Entry {
	int x;
	int y;
	unsigned char *header;
	unsigned char *data;
	void* priv;
	unsigned char pre_chars[NUM_CHARS];
} _Char_Info_Entry;

typedef struct Logo_Info {
	int x0;
	int y0;
	int width;
	int height;
	unsigned char *logo_data;
	_Char_Info_Entry entries[ENTRIES];
} _Logo_Info;

void free_logo_info(void);
void init_logo_info(MDPS_t m);
void TWMux_Update_Chars (int idx, unsigned char chars[], int len);

#endif
