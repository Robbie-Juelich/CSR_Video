#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/string.h>
#include "tw2828_config.h"
#include "tw2828_hostio.h"
#include "tw2828_twmux.h"
#include "test_disp.h"
#include "tw2828_logodata.c"
#include "tw2828_chars_data.c"

#define MainWin37FreezeEn()		WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL) | 0x04) 
#define MainWin37FreezeDis()	WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL)&(~0x04)) 
#define MainWin37BoundryEn()	WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL) | 0x02)
#define MainWin37BoundryDis()   WriteP( (WIN37_CTRL), ReadP(WIN37_CTRL) & 0xfd)

struct one_digit_yuv_data {
	unsigned char *header;
	unsigned char *data;
}tmp_data;

unsigned char pre_gps[NUM_CHARS] = {
	'0', '0', '0', 0xb0, '0', '0', '\'', '0', '0', '\"', 'E', 
	' ', ' ', ' ',
	'0', '0', '0', 0xb0, '0', '0', '\'', '0', '0', '\"', 'N', 0 };
unsigned char pre_mil[NUM_CHARS] = {
	'0', '0', '0', '0', '.', '0', '0','0', 'K', 'm', 
	' ', ' ', ' ', ' ',
	'0', '0', '0', 'K', 'm', '/', 'h', 0};

void update_entry_one_indiv(int idx, unsigned char *array, 
			int len, bool change_mode)
{
	int i;
	int x, y;
    unsigned char *ptr1, *ptr2;
	_Char_Info_Entry* entry = &(cur_logo->entries[idx]);
	unsigned char *pre_chars;

	if(idx == GPS) {
		pre_chars = pre_gps;
	} else if (idx == MIL) {
		pre_chars = pre_mil;
	} else {
		printk(KERN_ERR "[%s] id Error\n", __func__);
		return;
	}
	for (i = 0; i < len && i < NUM_CHARS; ++i) {
		if (pre_chars[i] == array[i] && change_mode == false) {
			continue;
		}
		ptr1 = chars_data[array[i]];
		ptr2 = tmp_data.data;
		memcpy(ptr2, ptr1, CHAR_SIZE);	
		pre_chars[i] = array[i];

		x = cur_logo->x0 + entry ->x + i * CHAR_WIDTH;
		y = cur_logo->y0 + entry ->y; 
//		printk("x0 y0  [%d %d]\n", cur_logo->x0, cur_logo->y0);
//		printk("x1 y1  [%d %d]\n", entry->x,entry->y);
//		printk("x y  [%d %d]\n", x, y);
		//MainWin37BoundryDis();
		MainWin37FreezeDis();
		TWMux_WriteDisplayToMemory((unsigned int)tmp_data.header,
			x,  y + DISPLAYBASE);
		MainWin37BoundryEn();
		MainWin37FreezeEn(); 
	}
	
}
#if 0
void update_entry_one_whole(_Digit_Info_Entry *entry, unsigned char *array)
{
	int i, j;
	int flag = 0;
	
	unsigned char *ptr1, *ptr2;
	
	int stride1 = CHAR_WIDTH * 2;
	int stride2 = stride1 * NUM_CHARS + DOT_WIDTH * 2 - 4;  // ?? 
	//printk("----------------%d\n", stride2);
	for (i = 0; i <  NUM_CHARS ; ++i) {
		if (entry->pre_digit[i] == array[i]) continue;
		printk("i %d, val: %d\n", i, array[i]);
		flag = 1;
//		if ( NUM_INT - 1 == i) 
//			ptr1 = digitdata_dot_0_9[array[i]];
//		else
			ptr1 = digitdata_0_9[array[i]];
		ptr2 = entry->data + CHAR_WIDTH * i * 2 + 
				((i >= NUM_INT) ? DOT_WIDTH : 0) * 2;
		for (j = 0; j <  CHAR_HEIGHT; ++j) {
			memcpy(ptr2, ptr1,  stride1);
			ptr1 += stride1;
			ptr2 += stride2;
		}
		entry->pre_digit[i] = array[i];
	}

	if (flag) {
		MainWin37FreezeDis();
		TWMux_WriteDisplayToMemory((unsigned int)entry->header, 
			entry->x + cur_logo->x0, entry->y + cur_logo->y0 + DISPLAYBASE);
		MainWin37FreezeEn(); // Keep the order.
	}
}
#endif

void change_logo(int mode)
{
	if (mode == TW2828_8CH) {
		cur_logo = &logo_info[LOGO_9ch];
	}
	else if (mode == TW2828_4CH) {
		cur_logo = &logo_info[LOGO_4ch];
	}
	else if (mode == TW2828_6CH) {
		cur_logo = &logo_info[LOGO_6ch];
	} 
	else if (mode == TW2828_1CH) {
		cur_logo = &logo_info[LOGO_1ch];
	} 
	else {
		printk(KERN_ERR "-----------------------error mode to set logo\n");
		return;
	}
	
}

void update_logo_info(int mode)
{
	change_logo(mode);
	update_entry_one_indiv(GPS , pre_gps, strlen(pre_gps), true);	
	update_entry_one_indiv(MIL , pre_mil, strlen(pre_mil), true);		
}

void init_logo_info(MDPS_t mode)
{
	int i,m, chars_size, char_size;
	int pixel_stride;
	int ids[] = {LOGO_4ch, LOGO_6ch, LOGO_9ch};

	pixel_stride = CHAR_WIDTH * NUM_CHARS;
	chars_size = pixel_stride * CHAR_HEIGHT * 2;
	yuvdata_header[2] = (pixel_stride) & 0xff;
	yuvdata_header[3] = (pixel_stride >> 8) & 0xff;
	yuvdata_header[4] = CHAR_HEIGHT & 0xff;
	yuvdata_header[5] = (CHAR_HEIGHT >> 8) & 0xff;

	cur_logo = NULL;
	for (m = 0; m < 3; ++m) {
		cur_logo = &logo_info[ids[m]];
		for (i = 0; i < ENTRIES; ++i) {
		#if 0
			cur_logo->entries[i].header = 
					kmalloc(chars_size + HEADER_SIZE, GFP_KERNEL);
			cur_logo->entries[i].data = cur_logo->entries[i].header + HEADER_SIZE;
			memset(cur_logo->entries[i].data, 0x0, chars_size);
			memcpy(cur_logo->entries[i].header, yuvdata_header, HEADER_SIZE);
		#endif
			//memset(&(cur_logo->entries[i].pre_chars), 0xff, NUM_CHARS);
	//		memset(pre_gps, 0xff, NUM_CHARS);
	//		memset(pre_mil, 0xff, NUM_CHARS);
		}
	}
	
	char_size = CHAR_WIDTH * CHAR_HEIGHT * 2;
	tmp_data.header = 
		kmalloc(char_size + HEADER_SIZE, GFP_KERNEL);	
	tmp_data.data = tmp_data.header + HEADER_SIZE;
	yuvdata_header[2] = CHAR_WIDTH & 0xff;
	yuvdata_header[3] = (CHAR_WIDTH >> 8) & 0xff;
	yuvdata_header[4] = CHAR_HEIGHT & 0xff;
	yuvdata_header[5] = (CHAR_HEIGHT >> 8) & 0xff;
	memcpy(tmp_data.header, yuvdata_header, HEADER_SIZE);

	if (mode == MODE_9CH) {
		cur_logo = &logo_info[LOGO_9ch];
		printk("--------------------------------------zzc 9ch selected\n");
	}
	else if (mode == MODE_4CH) {
		cur_logo = &logo_info[LOGO_4ch];
		printk("--------------------------------------zzc 4ch selected\n");
	}
	else if (mode == MODE_6CH) {
		cur_logo = &logo_info[LOGO_6ch];
		printk("--------------------------------------zzc 6ch selected\n");
	} 
	else {
		printk(KERN_ERR "-----------------------error mode to set logo\n");
	}
}

void TWMux_Update_Chars(int idx, unsigned char chars[], int len)
{
	update_entry_one_indiv(idx , chars, len, false);
//	update_entry_one_whole(&(cur_logo->entries[idx]) , value);
}

void free_logo_info(void)
{
	int i;
/*	for (i = 0; i < ENTRIES; ++i) {
		kfree(cur_logo->entries[i].header);
    }
*/
	kfree(tmp_data.header);
}

