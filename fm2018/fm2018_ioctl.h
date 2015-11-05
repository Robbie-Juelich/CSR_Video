#ifndef _FM2018_IOCTL_H
#define _FM2018_IOCTL_H


#if defined(__KERNEL__)
    #include <linux/types.h>
#else
    #include <sys/ioctl.h>
    #include <stdint.h>
    #define DEVICE "/dev/fm2018dev"
#endif

struct reg_val {
    uint16_t reg;
    uint16_t val;
	uint8_t  write;
};

#define FM2018_IOC_MAGIC            'f'

#define FM2018CMD_REG           _IOWR(FM2018_IOC_MAGIC, 1, int) 
//#define FM2018CMD_WRITE_REG          _IOWR(FM2018_IOC_MAGIC, 2, int)

#endif
