#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "tw2968.h"

#define DEVICE "/dev/tw2968dev"

int main(void)
{
	int fd, ret;
	tw2968_work_mode video_mode;
	tw2968_video_norm video_norm;	
	tw2968_video_size video_size;
	tw2968_video_scaler video_scaler;	

	fd = open(DEVICE, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Error open file %s\n", DEVICE);
		return -1;
	}

	video_norm.mode = TW2968_PAL;
	video_norm.chip = 0x0;	
	ret = ioctl(fd, TW2968_SET_VIDEO_NORM, &video_norm);
	if (ret < 0) {
		fprintf(stderr, "Error set norm\n");
	}
	
	video_mode.chip = 0x0;
	video_mode.mode = TW2968_4D1_MODE;
	video_mode.mode = TW2968_1D1_MODE;
//	video_mode.mode = TW2968_2D1_MODE;
	ret = ioctl(fd, TW2968_SET_WORK_MODE, &video_mode);
	if (ret < 0) {
		fprintf(stderr, "Error set work mode\n");
	}
	
	video_size.chip = 0x0;
	video_size.size = TW2968_SIZE_D1;
	ret = ioctl(fd, TW2968_SET_VIDEO_SIZE, &video_size);
	if (ret < 0) {
		fprintf(stderr, "Error set video size\n");
	}

	video_scaler.chip = 0x0;
	video_scaler.scaler_value = 0x0;
	ret = ioctl(fd, TW2968_SET_VIDEO_SCALER, &video_scaler);
	if (ret < 0) {
		fprintf(stderr, "Error set video scaler\n");
	}
		
	return 0;
}

