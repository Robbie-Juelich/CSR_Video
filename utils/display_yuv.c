#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <linux/videodev2.h>
#include "config.h"

int width = WIDTH;
int height = HEIGHT;
int format = FORMAT;
int stride;

unsigned char *main_buffer;

SDL_Surface *pscreen;
SDL_Overlay *overlay;
SDL_Rect drect;
Uint32 SDL_VIDEO_Flags = SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_RESIZABLE;
int image_size; 
int video_size;
unsigned char *tmpbuf;	

void int_handle(int sigio)
{
	exit(0);
}

FILE* open_yuv(const char *file)
{
	FILE *fp;

	if (file == NULL) return NULL;
	fp = fopen (file, "r");
	if (fp == NULL) return NULL;
	
	fseek(fp, 0, SEEK_END);
	video_size = ftell(fp);
	if (video_size % image_size != 0x0 ) { 
		fprintf(stderr, "size error\n");
	}
	fseek(fp, 0, SEEK_SET);
	return fp;
}

int read_yuv(FILE *fp, unsigned char *buffer)
{
	int ret;
	ret = fread (buffer, image_size, 1, fp);
	if (ret < 0) {
		fprintf(stderr,  "read yuv error\n");
		return 0;
	}
	return image_size;
}

int main(int argc, char **argv)
{
	int i, ret;
	int type;
	char driver[128];
	char* file_name = argv[1];
	FILE *fp;

	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr,"Couldnt initialization\n");
		return -1;
	}

	if (SDL_VideoDriverName(driver, sizeof(driver))) {
		printf("Video driver: %s\n", driver);
	}

	if (!(SDL_VIDEO_Flags & SDL_HWSURFACE)){
		SDL_VIDEO_Flags |= SDL_SWSURFACE;
	}
	pscreen = SDL_SetVideoMode(width, height, 0, SDL_VIDEO_Flags);
	if(pscreen ==NULL) return -1;
	// SDL_YV12_OVERLAY  yuv 420
	// SDL_IYUV_OVERLAY  yuv 444

	// SDL_YUY2_OVERLAY  yuv 422
	// SDL_UYVY_OVERLAY
	// SDL_YVYU_OVERLAY
	// YuY2 is YUYV	 are the same
	if (format == V4L2_PIX_FMT_YUV420) {
		type = SDL_YV12_OVERLAY;
	}
	else {
		type = SDL_UYVY_OVERLAY;
	}
	overlay = SDL_CreateYUVOverlay(width, height, type, pscreen);
    if(overlay ==NULL) return -1;
	printf("Created %dx%dx%d %s %s overlay\n",
		 overlay->w,overlay->h,overlay->planes, 
         overlay->hw_overlay?"hardware":"software", 
         overlay->format==SDL_YV12_OVERLAY?"YV12": 
         overlay->format==SDL_IYUV_OVERLAY?"IYUV": 
         overlay->format==SDL_YUY2_OVERLAY?"YUY2": 
         overlay->format==SDL_UYVY_OVERLAY?"UYVY": 
         overlay->format==SDL_YVYU_OVERLAY?"YVYU": 
         "Unknown"); 

	for(i=0; i<overlay->planes; i++) { 
		printf("plane %d: pitch=%d\n", i, overlay->pitches[i]); 
	} 

	image_size = set_image_size(width, height, format);

	fp = open_yuv(file_name);
	if (fp == NULL) {
		fprintf(stderr, "error read yuv file\n");
	}
    drect.x = 0;
    drect.y = 0;
    drect.w = pscreen->w;
    drect.h = pscreen->h;

	signal(SIGINT, int_handle);
	
	i  = 0;
	while (1) {
		char frame[64];
		if (video_size > 0) {
			sprintf(frame, "Frame %d", i++);
			SDL_WM_SetCaption(frame, NULL);
			SDL_LockYUVOverlay(overlay);
			read_yuv(fp,  (unsigned char*)overlay->pixels[0]);
			SDL_DisplayYUVOverlay(overlay, &drect);
			SDL_UnlockYUVOverlay(overlay);
			video_size -= image_size;
		}
		//SDL_Delay(100);
		SDL_Delay(1000);
	}
	fclose(fp);
}

