#include "tw2828_logo.h"
#include "tw2828_logodata9ch.c"
#include "tw2828_logodata4ch.c"
#include "tw2828_logodata6ch.c"
#include "tw2828_logodata1ch.c"

#define LOGO_4ch  0x0
#define LOGO_6ch  0x1
#define LOGO_9ch  0x2
#define LOGO_1ch  0x3

_Logo_Info *cur_logo;
_Logo_Info logo_info [] = {
	[LOGO_1ch] = {
		.logo_data = logodata_1ch,
		.x0 = 0,
		.y0 = 1000,
		.width = 1920,
		.height = 80,
		.entries[0] = {
			.x = 600,
			.y = 40,
			.header = NULL,
			.data = NULL,
		},
		.entries[1] = {
			.x = 1200,
			.y = 40,
			.header = NULL,
			.data = NULL,
		},
	},
	[LOGO_4ch] = {
		.logo_data = logodata_4ch,
		.x0 = 1440,
		.y0 = 0,
		.width = 480,
		.height = 1080,
		.entries[0] = {
			.x = 10,
			.y = 850,
			.header = NULL,
			.data = NULL,
		},
		.entries[1] = {
			.x = 10,
			.y = 890,
			.header = NULL,
			.data = NULL,
		},
	},
	[LOGO_6ch] = {
		.logo_data = logodata_6ch,
		.x0 = 0,
		.y0 = 960,
		.width = 1920,
		.height = 120,
		.entries[0] = {
			.x = 1000,
			.y = 30,
			.header = NULL,
			.data = NULL,
		},
		.entries[1] = {
			.x = 1000,
			.y = 70,
			.header = NULL,
			.data = NULL,
		},
	},
	[LOGO_9ch] = {
		.logo_data = logodata_9ch,
		.x0 = 1280,
		.y0 = 720,
		.width = 640,
		.height = 360,
		.entries[0] = {
			.x = 50,
			.y = 150,
			.header = NULL,
			.data = NULL,
		},
		.entries[1] = {
			.x = 50,
			.y = 190,
			.header = NULL,
			.data = NULL,
		},
	},
};

#define HEADER_SIZE	20
#define CHAR_WIDTH	20
#define CHAR_HEIGHT	28
#define CHAR_SIZE	1120

unsigned char yuvdata_header[HEADER_SIZE] = { 
	0x00, 0x00, CHAR_WIDTH & 0xff, (CHAR_WIDTH  >> 8) & 0xff,
	CHAR_HEIGHT & 0xff, (CHAR_HEIGHT >>8) & 0xff, 0x12, 0x00,
	0x00, 0x61,	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

