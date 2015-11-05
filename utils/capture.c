#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include "config.h"

#define BUFCOUNT 3

char *device = "/dev/video0";
int format = FORMAT;
unsigned char* mem[BUFCOUNT];
//unsigned char* mem_buffer;
int width = WIDTH;
int height = HEIGHT;
int image_size;

int main(int argc, char** argv)
{
	int camerafd;
	struct v4l2_format fmt;
	struct v4l2_requestbuffers reqbuf;
	struct v4l2_buffer buf;
	int type;
	int ret;
	int i;

	camerafd = open(device, O_RDWR);
	if (camerafd < 0) {
		fprintf(stderr, "[Error] open device error\n");
		return -1;
	}
	image_size = set_image_size(width, height, format);
	// mem_buffer = (unsigned char*)malloc(image_size);
	
	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;
	fmt.fmt.pix.pixelformat = format;
	ret = ioctl(camerafd, VIDIOC_S_FMT, &fmt);
	if (ret < 0) {
		fprintf(stderr, "error set format\n");
		return -1;
	}

	ret = ioctl(camerafd, VIDIOC_G_FMT, &fmt);
	if (ret < 0) {
		fprintf(stderr, "error get format\n");
		return -1;
	}
	printf("[OK] Format:  type[%x], width[%u], height[%u], \n"
		 	"pixelformat[%c%c%c%c], field[%x]. bytesperline[%d],\n"
			"size[%d], colorspace[%x]\n",
		fmt.type, fmt.fmt.pix.width, fmt.fmt.pix.height,
		fmt.fmt.pix.pixelformat & 0xff,
		(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
		(fmt.fmt.pix.pixelformat >> 16) & 0xFF,
		(fmt.fmt.pix.pixelformat >> 24) & 0xFF,
		fmt.fmt.pix.field, fmt.fmt.pix.bytesperline, 
		fmt.fmt.pix.sizeimage, fmt.fmt.pix.colorspace);
	
	memset (&reqbuf, 0, sizeof(struct v4l2_requestbuffers));
	reqbuf.count = BUFCOUNT;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP;
	/* Request buffer */
	ret = ioctl (camerafd, VIDIOC_REQBUFS, &reqbuf);
	if( ret < 0 ){
		fprintf(stderr, "[Error]error request buffer\n");
	}
	memset(&buf, 0, sizeof(struct v4l2_buffer));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	for (buf.index = 0x0; buf.index < BUFCOUNT; ++buf.index) {
		/* Put all the buffer in queue */
		ret = ioctl(camerafd, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) {
			fprintf(stderr,"[Error]error query reqbuf [%x]\n", buf.index);
			return -1;
		}
		// mmap
		mem[buf.index] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
			MAP_SHARED, camerafd, buf.m.offset);

		ret = ioctl(camerafd, VIDIOC_QBUF, &buf);
		if (ret < 0) {
			fprintf(stderr, "[Error]error queue buf [%x]\n", buf.index);
			return -1;
		}
	}
	// to start
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;	
	ret = ioctl(camerafd, VIDIOC_STREAMON, &type);
	if(ret<0){
		fprintf(stderr, "[Error] Unable to start capture: %s\n",
				strerror(errno));
		return -1;
	}
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	//  if buffer have data then de-queue the buffer 
	for(i = 0; i < BUFCOUNT; ++i) {
		int size;
		FILE *fp;
		char filename[64];
		ret = ioctl(camerafd, VIDIOC_DQBUF, &buf);	
		if (ret < 0) {
			fprintf(stderr,"[Error] Unable to dequeue buffer %s\n",
					strerror(errno));
			return -1;
		}
		if (buf.bytesused> image_size) {
	        size = image_size;
	    }
		else {
			size = buf.bytesused;
		}
		/* copy the data */
		//memcpy(mem_buffer, mem[buf.index], size);

		sprintf(filename, "%i.yuv", i);
		fp = fopen(filename, "w+");
		if (fp == NULL) {
			fprintf(stderr, "error open file %s\n", filename);
		}
		ret = fwrite(mem[buf.index], size, 1, fp);
		if (ret < 0) {
			fprintf(stderr, "error write to file %s\n", filename);
		}
		fclose(fp);
/*
		ret = ioctl(camerafd, VIDIOC_QBUF, &buf);	
		if(ret < 0){
			fprintf(stderr,"[Error] Unable to requeue buffer %s\n", 
				strerror(errno));
			return -1;
		}
*/
	}
	close(camerafd);
	return 0;
}

