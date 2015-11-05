/*
	To read/write tw2968 registers
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


#define DEVICE "/dev/i2c-1"

int main(int argc,char **argv)
{
 	struct i2c_rdwr_ioctl_data work_queue;
 	unsigned int idx;
	int fd;
	unsigned int slave_address,reg_address;
	unsigned char buf[3],val;
	int i,ret,write_flag=0;

	
	if (argc < 3){
		printf("Usage:\n%s reg_addr r/w [value]\n",argv[0]);
		return 0;
	}

	fd = open(DEVICE, O_RDWR);
	if(fd < 0){
		printf("Error on opening the device file\n");
		return 0;
	}
	
	slave_address = 0x28;
	sscanf(argv[1],"%x",&reg_address);
	buf[0] = reg_address;

	if (argv[2][0]=='w'){
		write_flag=1;
		sscanf(argv[3],"%x",&i);
		buf[1] = i&0xff;	
	}
	
	work_queue.nmsgs = 2;
	work_queue.msgs = (struct i2c_msg*)malloc(work_queue.nmsgs*sizeof(struct i2c_msg));
	if (!work_queue.msgs){
		printf("Memory alloc error\n");
		close(fd);
		return 0;
	}
	
	ioctl(fd,I2C_TIMEOUT,2);
	ioctl(fd,I2C_RETRIES,1);
	
	if (write_flag){
		work_queue.nmsgs = 1;
		work_queue.msgs[0].len = 2;
		work_queue.msgs[0].addr = slave_address;
		work_queue.msgs[0].buf = buf;
	}else{
		work_queue.msgs[0].len = 1;
		work_queue.msgs[0].addr = slave_address;
		work_queue.msgs[0].buf = buf;
		
		work_queue.msgs[1].len = 1;
		work_queue.msgs[1].flags = I2C_M_RD;
		work_queue.msgs[1].addr = slave_address;
		work_queue.msgs[1].buf	= &val;
	}

	ret = ioctl(fd,I2C_RDWR,(unsigned long)&work_queue);
	if (ret < 0)
		printf("read/write i2c device error\n");
	else if (write_flag)
		printf("write i2c device,slave address:%x,write address:%x,write value:%x\n",slave_address,reg_address,buf[1]);
	
	else
		printf("read i2c device,slave address:%x,read address:%x,read value:%x\n",slave_address,reg_address,val);

	close(fd);
	
	return ;
}	

