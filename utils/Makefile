CROSS_COMPILE = arm-fsl-linux-gnueabi-
CFLAGS = -g
CC = $(CROSS_COMPILE)gcc 
CC2 = gcc $(CFLAGS)
EXE = capture display_yuv mxc_v4l2_capture tw2828reg-rw i2c-rw

all: capture display_yuv reg

reg: tw2828reg-rw.c i2c-rw.c
	$(CC)  tw2828reg-rw.c -o tw2828reg-rw
	$(CC)  i2c-rw.c -o i2c-rw
	
capture: config.h  capture.c mxc_v4l2_capture.c
	$(CC)  capture.c -o capture
	$(CC)  mxc_v4l2_capture.c -o mxc_v4l2_capture
display_yuv:  display_yuv.c config.h
	$(CC2)  display_yuv.c -lSDL -g3 -o display_yuv

clean:
	rm -rf $(EXE)
