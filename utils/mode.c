/**
To change displa mode 4ch/6CH/9ch
*/


#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tw2828_ioctl.h"


int main(int argc, char** argv)
{
	int fd, ret, mode, i;
	struct windows_cam_mode win_mode;

	for (i = 0 ; i < 8; ++i){
		win_mode.cam_id[i] = i & 0xff;	
	}
	
	if(argc != 2) {
		fprintf(stderr, "%s <mode_id> (0->1ch, 1->4ch, 2->6ch, 3->8ch)\n", argv[0]);
		return -1;
	}
		
	sscanf(argv[1],"%d", &mode);
	if (mode > 3 || mode < 0) {
		fprintf(stderr, "mode error\n");
		return -1;
	}

	fd = open(DEVICE, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "open device error\n");
		return -1;
	}

	printf("set mode to %d\n", mode);
	win_mode.mode_type = mode;
	//if (mode != 3) 
		win_mode.cam_id[0] = 0;
	ret = ioctl(fd, TW2828CMD_MODE_CHNUM, &win_mode);
	if (ret < 0) {
		fprintf(stderr, "8CH device error\n");
		return -1;
	}
/*
	sleep(5);
	ret = ioctl(fd, TW2828CMD_MODE_CHNUM, TW2828_6CH);
	if (ret < 0) {
		fprintf(stderr, "6CH device error\n");
		return -1;
	}
*/	
	return 0;
}
