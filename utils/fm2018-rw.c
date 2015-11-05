/*
To read/write fm2018 registers
*/

#include <stdio.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "fm2018_ioctl.h"


int main(int argc,char **argv)
{
	int fd;
	unsigned int reg_address, value;
	int ret,write_flag=0;
	struct reg_val regval;
	
	if (argc < 3){
		printf("Usage:\n%s reg_addr r/w [value]\n",argv[0]);
		printf("  Read 0x1E30 with default 0x0233\n");
		return 0;
	}

	fd = open(DEVICE, O_RDWR);
	if(fd < 0){
		printf("Error on opening the device file\n");
		return 0;
	}
	
	sscanf(argv[1],"%x",&regval.reg);
	regval.write = 0x0;

	if (argv[2][0]=='w'){
		write_flag=1;
		sscanf(argv[3],"%x",&regval.val);
		regval.write = 0x1;
	}
	
//	if (write_flag){
//		printf("user to write %x to %x\n", regval.val, regval.reg);
//	}	
	ret = ioctl(fd, FM2018CMD_REG, &regval);

	if (ret < 0) {
		printf("read/write i2c device error\n");
		return -1;
	}

	if (write_flag)
		printf("fm2018 write, address:0x%04x,write value:0x%04x\n", 
			regval.reg, regval.val);
	else
		printf("fm2018 read,  address:0x%04x,read value:0x%04x\n",
			regval.reg, regval.val);

	close(fd);
	
	return 0;
}	

