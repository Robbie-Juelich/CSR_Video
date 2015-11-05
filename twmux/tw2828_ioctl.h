#ifndef _TW2828_IOCTL_H
#define _TW2828_IOCTL_H


#if defined(__KERNEL__)
	#include <linux/types.h>
#else
	#include <sys/ioctl.h>
	#include <stdint.h>
	#define DEVICE "/dev/tw2828dev"
#endif

struct reg_val {
    uint16_t reg;
    uint8_t  val;
};

struct channel_onoff {
    uint8_t id;
    uint8_t on;
};

struct mem_val {
	uint32_t start;
	uint32_t size;
	uint8_t *buf;
};

struct mem_valxy {
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
};


struct rec_channel {
	uint32_t cam_id;
	uint32_t rec;
};


#define GPS    0
#define MIL    1
#define ENTRIES 2
#define NUM_CHARS  32


struct info_chars {
	unsigned char array[NUM_CHARS];
	int type;
	int len;
};

struct windows_cam_mode {
	unsigned char cam_id[8];
	int mode_type;	
};

#define TW2828_1CH 0x0
#define TW2828_4CH 0x1
#define TW2828_6CH 0x2
#define TW2828_8CH 0x3


#define TW2828_IOC_MAGIC            't'

#define TW2828CMD_READ_REG           _IOWR(TW2828_IOC_MAGIC, 1, int) 
#define TW2828CMD_WRITE_REG          _IOW(TW2828_IOC_MAGIC, 2, int)
#define TW2828CMD_CHANNEL_ONOF       _IOW(TW2828_IOC_MAGIC, 3, int)
#define TW2828CMD_READ_MEM           _IOR(TW2828_IOC_MAGIC, 4, int)
#define TW2828CMD_READ_MEMXY         _IOR(TW2828_IOC_MAGIC, 5, int)
#define TW2828CMD_REC_SELECT_CHANNEL _IOR(TW2828_IOC_MAGIC, 6, int)
#define TW2828CMD_UPDATE_INFOCHARS       _IOW(TW2828_IOC_MAGIC, 7, int)
#define TW2828CMD_MODE_CHNUM        _IOW(TW2828_IOC_MAGIC, 8, int)

#ifndef __KERNEL__
int tw2828_update_chars(const int fd, struct info_chars *ichars);
#endif

#endif

