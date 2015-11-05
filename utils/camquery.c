/*
Query the capability of the camera
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/videodev2.h>

#define DEVICE "/dev/video0"

int main()
{
	int fd, ret;
	struct v4l2_capability cap;
	struct v4l2_fmtdesc fmtd;
	struct v4l2_format fmt;
	struct v4l2_frmsizeenum fsize;
	int format = V4L2_PIX_FMT_YUYV;

	fd = open(DEVICE, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "[Error] open device error\n");
		return -1;
	}

	memset(&cap, 0 ,sizeof(struct v4l2_capability));
	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0){
		fprintf(stderr, "[Error]error query capability\n");
		return -1;
	}
		
	fmtd.index = 0;
	fmtd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	// V4L2_PIX_FMT_YUYV
	printf("\nSupported format:\n");
	while((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtd)) == 0){
		fmtd.index++;
		printf("\tpixelformat = \"%c%c%c%c\", description = \"%s\" \n",
			fmtd.pixelformat & 0xFF,
			(fmtd.pixelformat >> 8) & 0xFF,
			(fmtd.pixelformat >> 16) & 0xFF,
			(fmtd.pixelformat >> 24) & 0xFF,
			fmtd.description);
//		if(fmtd.pixelformat == format){
//			printf("find format");
//		}
	}

	fsize.index = 0;
	printf("\nSupported frame size:\n");
	while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fsize) >= 0) {
		printf("\t%dx%d\n", fsize.discrete.width, fsize.discrete.height);
		fsize.index++;
	}

	printf("\nCurrent format:\n");
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if ((ret = ioctl(fd, VIDIOC_G_FMT, &fmt)) == 0) {
		printf("\t Width = %d", fmt.fmt.pix.width);
		printf("\t Height = %d", fmt.fmt.pix.height);
		printf("\t Image size = %d\n", fmt.fmt.pix.sizeimage);
		printf("\t pixelformat = %c%c%c%c\n", 
			fmt.fmt.pix.pixelformat & 0xff, 
			(fmt.fmt.pix.pixelformat >> 8)  & 0xff, 
			(fmt.fmt.pix.pixelformat >> 16) & 0xff, 
			(fmt.fmt.pix.pixelformat >> 24) & 0xff);
	}
	close(fd);
	return 0;
}

