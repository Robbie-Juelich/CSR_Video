/*
	To read/write 2828 registers
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "tw2828_ioctl.h"

int main(int argc, char **argv)
{
	int fd, ret;
	struct reg_val rval;	
	unsigned int reg_addr;
	unsigned int val;	

	if(argc < 3) {
		printf("Usage:\n%s  reg_addr r/w [value]\n", argv[0]);
		return 0;	
	}


	fd = open(DEVICE, O_RDWR);
	if(fd < 0){
		printf("Error on opening device %s\n", DEVICE);
		return 0;
	}

	sscanf(argv[1],"%x",&reg_addr);
	rval.reg = (unsigned short)(reg_addr & 0xffff);

	if (argv[2][0] == 'w') {
		if ( argc != 4) {
			printf("Usage:\n%s  reg_addr r/w [value]\n", argv[0]);
			return 0;
		}
		sscanf(argv[3], "%x", &val);
		rval.val = (unsigned char)(val & 0xff);
		ret = ioctl(fd, TW2828CMD_WRITE_REG, &rval);
		if (ret < 0) {
			printf("write %s device error\n", DEVICE);
			return -1;
		}
		printf("write %s device, write address:0x%04x,write value:0x%02x\n",
				 DEVICE, rval.reg, rval.val);
	}
	else if (argv[2][0] == 'r') {
		ret = ioctl(fd, TW2828CMD_READ_REG, &rval);
		if (ret < 0) {
			printf("read %s device error\n", DEVICE);
			return -1;
		}
		printf("read %s device, read address:0x%03x,read value:%02x\n",
				 DEVICE, rval.reg, rval.val);
	}
	else {
		printf("Usage:\n%s  reg_addr r/w [value]\n", argv[0]);
	}
	close(fd);
	return 0;
}

